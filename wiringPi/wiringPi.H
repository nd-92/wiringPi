// ======================================================================== //
//                                                                          //
// wiringPi.H                                                               //
//                                                                          //
// ======================================================================== //
// This class initialises the wiringPi program                              //
// Templated to allow execution in various modes                            //
// Access to hardware provided through onboard/device read/write methods    //
// ======================================================================== //

#ifndef __WIRING_PI_wiringPi_H
#define __WIRING_PI_wiringPi_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"
#include "wiringPiStaticAsserts.H"
#include "wiringPiTemplateMacros.H"
#include "wiringPiPinTables.H"
#include "wiringPiNode.H"
#include "piOnboardPins.H"
#include "piHardwareInfo.H"
#include "piMemory.H"
#include "wiringPiNode.H"
#include "piThread.H"
#include "piHiPriority.H"
#include "i2c.H"
#include "wiringPiStrings.H"
#include "softPwm.H"

namespace WiringPi
{
    // #define WIRINGPI_DEBUG 1

    static constinit std::atomic<bool> quit(false);

    void signalHandler(int)
    {
        quit.store(true);
    }

    // Templated, but at present only works correctly in GPIO mode
    template <const int wiringPiMode>
    class wiringPi
    {
    public:
        // Default constructor: specify wiringPiMode
        [[nodiscard]] wiringPi()
            : sa_(initialiseSigaction()),
              piMemory_(piMemory<piHardware_.model()>()),
              epochMilli_(initialiseEpochMilli()),
              epochMicro_(initialiseEpochMicro())
        {
            // Check for a valid mode
            setupModeCheck(mode_constant<wiringPiMode_>());

            // Check for non-GPIO mode on certain models
            computeModelCheck(pi_model_constant<piHardware_.model()>(), mode_constant<wiringPiMode_>());

            // Debug messages
#ifdef WIRINGPI_DEBUG
            // Mode info
            std::cout << "WiringPi initialised on mode " << wiringPiModes(wiringPiMode_) << std::endl;

            // Hardware info message
            std::cout << "Running on a Raspberry Pi " << piHardware_.modelString() << std::endl;
#endif
        }

        // Default destructor: reset all pins
        ~wiringPi()
        {
            // Statically ranged loop to reset pin modes to input
            resetAllPins();

            // Debug messages
#ifdef WIRINGPI_DEBUG
            std::cout << std::endl;
            std::cout << "All pins successfully reset to input on exit" << std::endl;
            std::cout << std::endl;
            std::cout << "wiringPi exited cleanly" << std::endl;
            std::cout << std::endl;
#endif
        };

        // Prints the pin layout
        inline void pinLayout() const
        {
            layoutHeader();
            writePinLine();
            layoutHeader();
        }

        // Performs the read/write test on all available pins
        [[nodiscard]] bool readWriteTest()
        {
            // Allocate an array of booleans
            std::array<bool, 64> testReturns;

            // By default set all to fail
            for (std::size_t i = 0; i < testReturns.size(); i++)
            {
                testReturns[i] = unit_test_fail();
            }

            // Perform the read/write test
            pinReadWriteTest(testReturns, pin_constant<0>());

            // Check to see if any failed
            for (std::size_t i = 0; i < testReturns.size(); i++)
            {
                if (testReturns[i] == unit_test_fail())
                {
                    return unit_test_fail();
                }
            }

            // None failed, so pass
            return unit_test_pass();
        }

        // Reads the value at a pin and converts it into a string
        template <const pin_t __pinNumber>
        [[nodiscard]] inline constexpr const std::string_view readToString(const pin_constant<__pinNumber> pinNumber) const
        {
            // If the pin is valid, read the value and convert it to a string_view
            if constexpr (pinMap_[pinNumber()] != nullPin<pin_t>())
            {
                if (digitalReadOnboard(pin_constant<pinNumber()>()) == 0)
                {
                    return "0";
                }
                else
                {
                    return "1";
                }
            }
            // Otherwise return a blank string_view
            else
            {
                return "-";
            }
        }

        [[nodiscard]] const std::string pinString(const pin_t i) const
        {
            if (i == nullPin<pin_t>())
            {
                return "  --  ";
            }
            if (i > 9)
            {
                return "  " + std::to_string(i) + "  ";
            }
            return "   " + std::to_string(i) + "  ";
        }

        // Program kill switch
        // Safer replacement for while(1)
        [[nodiscard]] inline bool run() const
        {
            return !(quit.load());
        }

        // Read an input bit
        template <const pin_t __pinNumber>
        [[nodiscard]] inline gpio_t digitalReadOnboard(const pin_constant<__pinNumber> pinNumber) const volatile
        {
            // Check that we are operating on a valid pin
            validPinCheck(pin_constant<piHardware_.numPins()>(), pin_constant<pinMap_[pinNumber()]>());

#ifdef WIRINGPI_DEBUG
            std::cout << "Reading " << onboardPins_.digitalRead(piMemory_.gpioPtr(), pin_constant<pinMap_[pinNumber()]>()) << " from pin " << pinMap_[pinNumber()] << std::endl;
#endif

            // Call the method on the onboard pins
            return onboardPins_.digitalRead(piMemory_.gpioPtr(), pin_constant<pinMap_[pinNumber()]>());
        }

        // Write an output bit
        template <const pin_t __pinNumber, const gpio_t __value>
        inline void digitalWriteOnboard(const pin_constant<__pinNumber> pinNumber, const gpio_constant<__value> value) const volatile
        {
            // Check that we are operating on a valid pin
            validPinCheck(pin_constant<piHardware_.numPins()>(), pin_constant<pinMap_[pinNumber()]>());

#ifdef WIRINGPI_DEBUG
            std::cout << "Writing " << value() << " to pin " << pinMap_[pinNumber()] << std::endl;
#endif

            // Call the method on the onboard pins
            onboardPins_.digitalWrite(piMemory_.gpioPtr(), pin_constant<pinMap_[pinNumber()]>(), pin_constant<value()>());
        }

        // Sets the mode of a pin to be input, output or PWM output
        template <const pin_t __pinNumber, const gpio_t __mode>
        inline void pinModeOnboard(const pin_constant<__pinNumber> pinNumber, const gpio_constant<__mode> mode)
        {
            // Check that we are operating on a valid pin
            validPinCheck(pin_constant<piHardware_.numPins()>(), pin_constant<pinMap_[pinNumber()]>());

#ifdef WIRINGPI_DEBUG
            std::cout << "Setting pin " << pinMap_[pinNumber()] << " to mode " << mode() << std::endl;
#endif

            // Call the method on the onboard pins
            onboardPins_.pinMode(piMemory_.gpioPtr(), pin_constant<pinMap_[pinNumber()]>(), gpio_constant<mode()>(), pin_constant<gpioToGPFSEL_[pinMap_[pinNumber()]]>(), pin_constant<gpioToShift_[pinMap_[pinNumber()]]>());
        }

        // Control the internal pull-up/down resistors on a GPIO pin
        template <const pin_t __pinNumber, const gpio_t __pud>
        inline void pullUpDnControlOnboard(const pin_constant<__pinNumber> pinNumber, const gpio_constant<__pud> pud) const
        {
            // Check that we are operating on a valid pin
            validPinCheck(pin_constant<piHardware_.numPins()>(), pin_constant<pinMap_[pinNumber()]>());

#ifdef WIRINGPI_DEBUG
            std::cout << "Setting PUD to " << pud() << " on pin " << pinMap_[pinNumber()] << std::endl;
#endif

            // Call the method on the onboard pins
            onboardPins_.pullUpDnControl(piMemory_.gpioPtr(), pin_constant<pinMap_[pinNumber()]>(), gpio_constant<pud()>(), gpio_constant<piHardware_.gpioPupOffset()>());
        }

        // Set an output PWM value
        template <const pin_t pinNumber>
        inline void pwmWriteOnboard(const pwm_t value) const
        {
            validPinCheck(pin_constant<piHardware_.numPins()>(), pin_constant<pinMap_[pinNumber]>());
            static_assert(wiringPiMode_ == WPI_MODE_GPIO(), "wiringPi not initialised in GPIO mode: pwmWriteOnboard cannot be called");
            assert((piMemory_.gpioBase() == 0) && "GPIO base address must be 0: pwmWriteOnboard cannot be called");
            assert((!piMemory_.usingGpioMemory()) && "Application is using GPIO memory. Unable to do this when using /dev/gpiomem. Try sudo?");
            onboardPins_.pwmWrite<pinMap_[pinNumber]>(piMemory_.pwmPtr(), value);
        }

        // Output the given frequency on the Pi's PWM pin
        template <const pin_t pinNumber>
        inline void pwmToneWriteOnboard(const timer_t freq) const
        {
            if (freq == 0)
            {
                pwmWriteOnboard<pinNumber>(0);
            }
            else
            {
                pwmSetRange(600000 / freq);
                pwmWriteOnboard<pinNumber>(freq / 2);
            }
        }

        // Select the native "balanced" mode, or standard mark:space mode
        template <const int mode>
        inline void pwmSetMode() const
        {
            setupModeCheck<wiringPiMode>();
            static_assert(((mode == PWM_MODE_MS<int>()) || (mode == PWM_MODE_BAL<int>())), "Invalid mode for pwmSetMode: valid modes are MS, BAL.");
            if constexpr (mode == PWM_MODE_MS<int>())
            {
                *(piMemory_.pwmPtr() + PWM_CONTROL<pwm_t>()) = PWM0_ENABLE<pwm_t>() | PWM1_ENABLE<pwm_t>() | PWM0_MS_MODE<pwm_t>() | PWM1_MS_MODE<pwm_t>();
            }
            else if constexpr (mode == PWM_MODE_BAL<int>())
            {
                *(piMemory_.pwmPtr() + PWM_CONTROL<pwm_t>()) = PWM0_ENABLE<pwm_t>() | PWM1_ENABLE<pwm_t>();
            }
        }

        // Set the PWM range register
        inline void pwmSetRange(const pwm_t range) const
        {
            setupModeCheck<wiringPiMode>();
            *(piMemory_.pwmPtr() + PWM0_RANGE<pwm_t>()) = range;
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            *(piMemory_.pwmPtr() + PWM1_RANGE<pwm_t>()) = range;
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }

        // Set the PWM clock
        template <const timer_t divisor>
        inline void pwmSetClock() const
        {
            setupModeCheck<wiringPiMode>();
            const timer_t divisor_ = reducedDivisor(divisor);

            // Preserve PWM_CONTROL
            const timer_t pwmControl = *(piMemory_.pwmPtr() + PWM_CONTROL<pwm_t>());

            // Stop PWM
            *(piMemory_.pwmPtr() + PWM_CONTROL<pwm_t>()) = 0;

            // Stop PWM clock
            *(piMemory_.clkPtr() + PWMCLK_CNTL<timer_t>()) = BCM_PASSWORD<timer_t>() | 0x01;
            // Prevents clock going sloooow
            std::this_thread::sleep_for(std::chrono::microseconds(110));

            // Wait for clock to be !BUSY
            while ((*(piMemory_.clkPtr() + PWMCLK_CNTL<timer_t>()) & 0x80) != 0)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }

            *(piMemory_.clkPtr() + PWMCLK_DIV<timer_t>()) = BCM_PASSWORD<timer_t>() | (divisor_ << 12);

            // Start PWM clock
            *(piMemory_.clkPtr() + PWMCLK_CNTL<timer_t>()) = BCM_PASSWORD<timer_t>() | 0x11;
            // Restore PWM_CONTROL
            *(piMemory_.pwmPtr() + PWM_CONTROL<pwm_t>()) = pwmControl;
        }

        // Set the frequency on a GPIO clock pin
        template <const pin_t pinNumber>
        // inline void gpioClockSet(const timer_t freq) const
        void gpioClockSet(const timer_t freq) const
        {
            setupModeCheck<wiringPiMode>();
            validPinCheck(pin_constant<piHardware_.numPins()>(), pin_constant<pinMap_[pinNumber]>());
            static_assert(gpioToClkDiv_[pinMap_[pinNumber]] != nullPin<pin_t>(), "Pin number is not a valid GPIO pin");
            static_assert(gpioToClkCon_[pinMap_[pinNumber]] != nullPin<pin_t>(), "Pin number is not a valid GPIO pin");
            assert((!piMemory_.usingGpioMemory()) && "Application is using GPIO memory. Unable to do this when using /dev/gpiomem. Try sudo?");

            const timer_t divi = std::min(19200000 / freq, static_cast<timer_t>(4095));
            const timer_t divr = 19200000 % freq;
            const timer_t divf = static_cast<timer_t>(static_cast<double>(divr) * 4096.0 / 19200000.0);

            // Stop GPIO Clock
            *(piMemory_.clkPtr() + gpioToClkCon_[pinMap_[pinNumber]]) = BCM_PASSWORD<timer_t>() | GPIO_CLOCK_SOURCE<timer_t>();

            // Wait...
            while ((*(piMemory_.clkPtr() + gpioToClkCon_[pinMap_[pinNumber]]) & 0x80) != 0)
            {
                ;
            }

            // // Set dividers
            *(piMemory_.clkPtr() + gpioToClkDiv_[pinMap_[pinNumber]]) = BCM_PASSWORD<timer_t>() | (divi << 12) | divf;

            // // Start Clock
            *(piMemory_.clkPtr() + gpioToClkCon_[pinMap_[pinNumber]]) = BCM_PASSWORD<timer_t>() | 0x10 | GPIO_CLOCK_SOURCE<timer_t>();
        }

        // Set the PAD driver value
        // Group must be 0, 1, 2
        template <const pin_t group>
        inline void setPadDrive(const pwm_t value) const
        {
            setupModeCheck<wiringPiMode>();
            static_assert(((group == 0) || (group == 1) || (group == 2)), "Invalid group for setPadDrive: valid groups are 0, 1, 2.");

            const pwm_t reducedValue = value & 7;

            const pwm_t wrVal = BCM_PASSWORD<pwm_t>() | 0x18 | reducedValue; // Drive strength 0-7
            *(piMemory_.padsPtr() + group + 11) = wrVal;
            // rdVal = *(pads + group + 11);
        }

        template <const pin_t pinNumber>
        [[nodiscard]] inline constexpr gpio_t getAlt() const
        {
            validPinCheck(pin_constant<piHardware_.numPins()>(), pin_constant<pinMap_[pinNumber]>());
            return (*(piMemory_.gpioPtr() + gpioToGPFSEL_[pinMap_[pinNumber]]) >> gpioToShift_[pinMap_[pinNumber]]) & 7;
        }

        // Returns the map of pins
        [[nodiscard]] inline constexpr const pinTable &pinMap() const
        {
            return pinMap_;
        }

        // Returns the map of pin modes
        [[nodiscard]] inline constexpr const pinTable &pinModeMap() const
        {
            return onboardPins_.pinModeMap();
        }

        [[nodiscard]] inline consteval const pinTable &sysFds() const
        {
            return sysFds_;
        }

        [[nodiscard]] inline time_t micros() const
        {
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            return (ts.tv_sec * 1000000L) + (ts.tv_nsec / 1000L) - epochMicro();
        }

        [[nodiscard]] inline time_t millis() const
        {
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            return (ts.tv_sec * 1000L) + (ts.tv_nsec / 1000000L) - epochMilli();
        }

        [[nodiscard]] inline consteval int mode() const
        {
            return wiringPiMode_;
        }

        [[nodiscard]] inline consteval const std::string_view onboardNames() const
        {
            return onboardPins_.deviceName();
        }

        [[nodiscard]] inline constexpr const nameTable physNames() const
        {
            return physNames_;
        }

        // Reset pins to input
        inline void resetPins()
        {
            resetAllPins();
        }

    private:
        // Gets the correct comparison for the pin number:
        // Returns 1, except if the pin number is 30
        template <const pin_t __pinNumber>
        [[nodiscard]] inline constexpr gpio_t firstReadComparison(const pin_constant<__pinNumber> pinNumber) const
        {
            // Pin number 30 always returns 0 on the first comparison
            if constexpr (pinNumber() == 30)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }

        // Gets the correct comparison for the pin number:
        // Returns 0, except if the pin number is 2, 3, 28 or 31
        template <const pin_t __pinNumber>
        [[nodiscard]] inline constexpr gpio_t secondReadComparison(const pin_constant<__pinNumber> pinNumber) const
        {
            // Pins 2, 3, 28 and 31 always return 1 on the second comparison
            if constexpr ((pinNumber() == 2) | (pinNumber() == 3) | (pinNumber() == 28) | (pinNumber() == 31))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        // Writes the header of the pin layout
        void layoutHeader() const
        {
            std::cout << "+------+------+---------+------+---+------++------+---+------+---------+------+------+" << std::endl;
            std::cout << "| BCM  |  Pi  | PinName | Mode | V |   Physical   | V | Mode | PinName |  Pi  | BCM  |" << std::endl;
            std::cout << "+------+------+---------+------+---+------++------+---+------+---------+------+------+" << std::endl;
        }

        // Write an individual line of the pin layout
        template <const pin_t __pinNumber = 0>
        void writePinLine(const pin_constant<__pinNumber> = pin_constant<0>()) const
        {
            const pin_t i_1 = (2 * __pinNumber);
            const pin_t i_2 = (2 * __pinNumber) + 1;
            std::cout << "|" << pinString(pinMap()[i_1])                  // Mapped pin
                      << "|" << pinString(physToWpi_[i_1])                // Phys to wPi
                      << "|" << physNames()[i_1]                          // Name
                      << "|" << modeStrings()[pinModeMap()[i_1]]          // Mode
                      << "| " << readToString(pin_constant<i_1>()) << " " // Value
                      << "|" << pinString(i_1 + 1)                        // Physical pin number
                      << "||" << pinString(i_2 + 1)                       // Physical pin number
                      << "| " << readToString(pin_constant<i_2>()) << " " // Value
                      << "|" << modeStrings()[pinModeMap()[i_2]]          // Mode
                      << "|" << physNames()[i_2]                          // Name
                      << "|" << pinString(physToWpi_[i_2])                // Phys to wPi
                      << "|" << pinString(pinMap()[i_2])                  // Mapped pin
                      << "|" << std::endl;

            // The index must be less than 19
            if constexpr (__pinNumber < 19)
            {
                writePinLine(pin_constant<__pinNumber + 1>());
            }
        }

        // Performs the read/write unit test on a particular pin
        template <const pin_t __pinNumber>
        void pinReadWriteTest(std::array<bool, 64> &testReturns, const pin_constant<__pinNumber> pinNumber)
        {
            // Check if we are on a null pin
            if constexpr (pinMap_[pinNumber()] == nullPin<pin_t>())
            {
                // If so, automatically pass
                testReturns[pinNumber()] = unit_test_pass();
            }
            // Do this if we are not on a null pin
            else
            {
                // Set the pin to output and set value to 1
                pinModeOnboard(pin_constant<pinNumber()>(), outputMode());
                digitalWriteOnboard(pin_constant<pinNumber()>(), high());

                // Set the pin to input and read the value
                pinModeOnboard(pin_constant<pinNumber()>(), inputMode());
                const gpio_t firstRead = digitalReadOnboard(pin_constant<pinNumber()>());

                // Set the pin to output and set value to 0
                pinModeOnboard(pin_constant<pinNumber()>(), outputMode());
                digitalWriteOnboard(pin_constant<pinNumber()>(), low());

                // Set the pin to input and read the value
                pinModeOnboard(pin_constant<pinNumber()>(), inputMode());
                const gpio_t secondRead = digitalReadOnboard(pin_constant<pinNumber()>());

                // Pass condition is that we returned 1 and then 0
                // Special cases:
                // On pins 2, 3, 28, 31, should return [1, 1]
                // On pin 19, should return [0, 0]
                if ((firstRead == firstReadComparison(pin_constant<pinMap_[pinNumber()]>())) & (secondRead == secondReadComparison(pin_constant<pinMap_[pinNumber()]>())))
                {
                    testReturns[pinNumber()] = unit_test_pass();
                }
                else
                {
                    std::cout << "Read/write test fail on pin " << pinNumber() << " (mapped pin " << pinMap_[pinNumber()] << ")" << ", returned [" << firstRead << ", " << secondRead << "], pass is [1, 0] " << std::endl;
                }
            }

            // Advance to the next iteration if we are still within bounds
            if constexpr (__pinNumber < MAX_PINS<pin_t>() - 1)
            {
                pinReadWriteTest(testReturns, pin_constant<__pinNumber + 1>());
            }
        }

        // Helper function to initialise sa_
        [[nodiscard]] inline const struct sigaction initialiseSigaction() const
        {
            struct sigaction sa
            {
            };
            memset(&sa, 0, sizeof(sa));
            sa.sa_handler = signalHandler;

            const int sigfillset_return = sigfillset(&sa.sa_mask);
            assert((sigfillset_return == 0) && "sigfillset returned non-zero on initialisation of wiringPi class");

            // Set actions
            const int sigaction_interrupt = sigaction(SIGINT, &sa, NULL);
            const int sigaction_terminate = sigaction(SIGTERM, &sa, NULL);
            const int sigaction_hangup = sigaction(SIGHUP, &sa, NULL);
            const int sigaction_segfault = sigaction(SIGSEGV, &sa, NULL);

            // Check for failure
            assert((sigaction_interrupt == 0) && "sigaction returned non-zero on establishing interrupt");
            assert((sigaction_terminate == 0) && "sigaction returned non-zero on establishing terminate");
            assert((sigaction_hangup == 0) && "sigaction returned non-zero on establishing hangup");
            assert((sigaction_segfault == 0) && "sigaction returned non-zero on establishing segfault");

            return sa;
        }

        // Make sure cleanup is performed on interrupt, terminate, hangup or segfault
        const struct sigaction sa_;

        // Helper method for pwmSetClock
        [[nodiscard]] inline constexpr timer_t reducedDivisor(const timer_t divisor) const
        {
            timer_t reducedDivisor = divisor;
            if (piMemory_.gpioBase() == GPIO_PERI_BASE_2711<pin_t>())
            {
                reducedDivisor = 540 * reducedDivisor / 192;
            }
            return reducedDivisor &= 4095;
        }

        // Pi hardware info
        static constexpr const piHardware piHardware_ = piHardware();

        // Pi memory addresses
        const piMemory<piHardware_.model()> piMemory_;

        // Reset all the pins to input mode
        template <pin_t __pinNumber = 0>
        void resetAllPins(const pin_constant<__pinNumber> = pin_constant<0>())
        {
            if constexpr (pinMap_[__pinNumber] != nullPin<pin_t>())
            {
                pinModeOnboard(pin_constant<__pinNumber>(), gpio_constant<OUTPUT<gpio_t>()>());
                digitalWriteOnboard(pin_constant<__pinNumber>(), gpio_constant<LOW<gpio_t>()>());
                pinModeOnboard(pin_constant<__pinNumber>(), gpio_constant<INPUT<gpio_t>()>());
            }
            if constexpr (__pinNumber < MAX_PINS<pin_t>() - 1)
            {
                resetAllPins(pin_constant<__pinNumber + 1>());
            }
        }

        // Registry of onboard pins
        onboardPins onboardPins_ = onboardPins();

        // Registry of pin threads
        // std::array<std::thread, MAX_PINS()> pinThreads_;

        // Setup info
        // static constexpr const int wiringPiSetuped_ = static_cast<int>(true);
        // const int wiringPiDebug_;
        // const int wiringPiReturnCodes_;
        static constexpr const int wiringPiMode_ = wiringPiMode;

        // Map a BCM_GPIO pin to its function selection control port. (GPFSEL 0-5)
        // Groups of 10 - 3 bits per Function - 30 bits per port
        static constexpr const pinTable gpioToGPFSEL_ = gpioToGPFSEL();

        // Define the shift up for the 3 bits per pin in each GPFSEL port
        static constexpr const pinTable gpioToShift_ = gpioToShift();

        // Pin conversion table
        static constexpr const pinTable pinMap_ = pinMapI<piHardware_.gpioLayout(), wiringPiMode_>();

        // Physical to wPi conversion table
        static constexpr const pinTable physToWpi_ = physToWpiI();

        // Offsets to the clock control register
        static constexpr const pinTable gpioToClkCon_ = gpioToClkConI();

        // Offsets to the divisor register
        static constexpr const pinTable gpioToClkDiv_ = gpioToClkDivI();

        // Physical pin names
        static constexpr const nameTable physNames_ = physNamesI();

        // System mode pin table cannot be constexpr
        const pinTable sysFds_ = sysFdsI(wiringPiMode_);

        // Timer variables and initialisers
        const time_t epochMilli_;
        const time_t epochMicro_;
        [[nodiscard]] inline time_t epochMilli() const
        {
            return epochMilli_;
        }
        [[nodiscard]] inline time_t epochMicro() const
        {
            return epochMicro_;
        }
        [[nodiscard]] inline time_t initialiseEpochMilli() const
        {
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000L);
        }
        [[nodiscard]] inline time_t initialiseEpochMicro() const
        {
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            return (ts.tv_sec * 1000000) + (ts.tv_nsec / 1000L);
        }

        // Software version info
        static constexpr std::string_view version_ = VERSION();
        static constexpr const int versionMajor_ = VERSION_MAJOR<int>();
        static constexpr const int versionMinor_ = VERSION_MINOR<int>();
    };
}

#endif
