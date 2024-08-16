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
#include "wiringPiTemplateMacros.H"
#include "wiringPiPinTables.H"
#include "wiringPiNode.H"
#include "piOnboardPins.H"
#include "piHardwareInfo.H"
#include "piMemory.H"
#include "wiringPiNode.H"
#include "piThread.H"
#include "piHiPriority.H"

namespace WiringPi
{
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
        [[nodiscard]] inline wiringPi()
            : piMemory_(piMemory<piHardware_.model()>()),
              epochMilli_(initialiseEpochMilli()),
              epochMicro_(initialiseEpochMicro())
        {
            // Check for a valid mode
            setupModeCheck<wiringPiMode>();

            // Check for non-GPIO mode on certain models
            computeModelCheck<piHardware_.model(), wiringPiMode_>();

            // Make sure cleanup is performed on interrupt, terminate, hangup or segfault
            struct sigaction sa;
            memset(&sa, 0, sizeof(sa));
            sa.sa_handler = signalHandler;
            sigfillset(&sa.sa_mask);
            sigaction(SIGINT, &sa, NULL);
            sigaction(SIGTERM, &sa, NULL);
            sigaction(SIGHUP, &sa, NULL);
            sigaction(SIGSEGV, &sa, NULL);
            std::cout << "WiringPi initialised on mode " << wiringPiModes()[wiringPiMode_] << std::endl;
        }

        ~wiringPi()
        {
            // Statically ranged loop to reset pin modes to input
            resetPins();
            std::cout << std::endl;
            std::cout << "All pins successfully reset to input on exit" << std::endl;
            std::cout << std::endl;
            std::cout << "wiringPi exited cleanly" << std::endl;
            std::cout << std::endl;
        };

        inline void readAll() const
        {
            for (pin_t i = 0; i < 32; i++)
            {
                std::cout << "Pin " << i << ", mode " << modeStrings()[pinModeMap()[i]] << std::endl;
            }
        }

        const std::string pinString(const pin_t i) const
        {
            if (i == nullPin<pin_t>())
            {
                return "    ";
            }
            if (i > 9)
            {
                return " " + std::to_string(i) + " ";
            }
            return "  " + std::to_string(i) + " ";
        }

        // Program kill switch
        // Safer replacement for while(1)
        [[nodiscard]] inline bool run() const
        {
            return !(quit.load());
        }

        // Read an input bit
        template <const pin_t pinNumber>
        [[nodiscard]] inline gpio_t digitalReadOnboard() const volatile
        {
            onboardPinCheck<piHardware_.numPins(), pinMap_, pinNumber>();
            return onboardPins_.digitalRead<pinMap_[pinNumber]>(piMemory_.gpioPtr());
        }

        // Write an output bit
        template <const pin_t pinNumber>
        inline void digitalWriteOnboard(const gpio_t value) const volatile
        {
            onboardPinCheck<piHardware_.numPins(), pinMap_, pinNumber>();
            onboardPins_.digitalWrite<pinMap_[pinNumber]>(piMemory_.gpioPtr(), value);
        }

        // Sets the mode of a pin to be input, output or PWM output
        template <const pin_t pinNumber>
        inline void pinModeOnboard(const gpio_t mode)
        {
            onboardPinCheck<piHardware_.numPins(), pinMap_, pinNumber>();
            onboardPins_.pinMode<pinMap_[pinNumber]>(piMemory_.gpioPtr(), mode, gpioToGPFSEL_[pinMap_[pinNumber]], gpioToShift_[pinMap_[pinNumber]]);
        }

        // Control the internal pull-up/down resistors on a GPIO pin
        template <const pin_t pinNumber>
        inline void pullUpDnControlOnboard(const gpio_t pud) const
        {
            onboardPinCheck<piHardware_.numPins(), pinMap_, pinNumber>();
            onboardPins_.pullUpDnControl<pinMap_[pinNumber], piHardware_.gpioPupOffset()>(piMemory_.gpioPtr(), pud);
        }

        // Set an output PWM value
        template <const pin_t pinNumber>
        inline void pwmWriteOnboard(const pwm_t value) const
        {
            onboardPinCheck<piHardware_.numPins(), pinMap_, pinNumber>();
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
            onboardPinCheck<piHardware_.numPins(), pinMap_, pinNumber>();
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

            const pwm_t wrVal = BCM_PASSWORD<pwm_t>() | 0x18 | value; // Drive strength 0-7
            *(piMemory_.padsPtr() + group + 11) = wrVal;
            // rdVal = *(pads + group + 11);
        }

        template <const pin_t pinNumber>
        [[nodiscard]] inline constexpr gpio_t getAlt() const
        {
            onboardPinCheck<piHardware_.numPins(), pinMap_, pinNumber>();
            return (*(piMemory_.gpioPtr() + gpioToGPFSEL_[pinMap_[pinNumber]]) >> gpioToShift_[pinMap_[pinNumber]]) & 7;
        }

        // Returns the map of pins
        [[nodiscard]] inline consteval const pinTable &pinMap() const
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

        [[nodiscard]] inline consteval const nameTable physNames() const
        {
            return physNames_;
        }

        // Reset pins to INPUT
        inline void resetPins()
        {
            resetAllPins();
        }

    private:
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
        void resetAllPins(std::integral_constant<pin_t, piHardware_.numPins()>) {}
        template <pin_t index = 0>
        void resetAllPins(std::integral_constant<pin_t, index> = std::integral_constant<pin_t, 0>())
        {
            pinModeOnboard<index>(INPUT<gpio_t>());
            resetAllPins(std::integral_constant<pin_t, index + 1>());
        }

        // Registry of onboard pins
        onboardPins onboardPins_ = onboardPins();

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
