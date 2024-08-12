#ifndef __WIRING_PI_H
#define __WIRING_PI_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"
#include "wiringPiTemplateMacros.H"
#include "wiringPiPinTables.H"
#include "wiringPiNode.H"
#include "onboardPins.H"
#include "piHardwareInfo.H"
#include "piMemory.H"
#include "wiringPiNode.H"

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
            : piMemory_(piMemory<pi_.model()>()),
              epochMilli_(initialiseEpochMilli()),
              epochMicro_(initialiseEpochMicro())
        {
            // Check for non-GPIO mode on certain models
            static_assert(!(pi_.model() == PI_MODEL_CM<int>() && wiringPiMode != WPI_MODE_GPIO()), "Only WPI_MODE_GPIO available on PI_MODEL_CM");
            static_assert(!(pi_.model() == PI_MODEL_CM3<int>() && wiringPiMode != WPI_MODE_GPIO()), "Only WPI_MODE_GPIO available on PI_MODEL_CM3");
            static_assert(!(pi_.model() == PI_MODEL_CM3P<int>() && wiringPiMode != WPI_MODE_GPIO()), "Only WPI_MODE_GPIO available on PI_MODEL_CM3P");

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
            static_assert(pi_.isOnboard(pinMap_[pinNumber]), "Pin number is not onboard (onboard pins range from 0 to 31)");
            return pinRegistry_.digitalRead<pinMap_[pinNumber]>(piMemory_.gpioPtr());
        }

        // Write an output bit
        template <const pin_t pinNumber>
        inline void digitalWriteOnboard(const gpio_t value) const volatile
        {
            static_assert(pi_.isOnboard(pinMap_[pinNumber]), "Pin number is not onboard (onboard pins range from 0 to 31)");
            pinRegistry_.digitalWrite<pinMap_[pinNumber]>(piMemory_.gpioPtr(), value);
        }

        // Sets the mode of a pin to be input, output or PWM output
        template <const pin_t pinNumber>
        inline void pinModeOnboard(const gpio_t mode)
        {
            static_assert(pi_.isOnboard(pinMap_[pinNumber]), "Pin number is not onboard (onboard pins range from 0 to 31)");
            pinRegistry_.pinMode<pinMap_[pinNumber]>(piMemory_.gpioPtr(), mode, gpioToGPFSEL_[pinNumber], gpioToShift_[pinNumber]);
        }

        // Control the internal pull-up/down resistors on a GPIO pin
        template <const pin_t pinNumber>
        inline void pullUpDnControlOnboard(const gpio_t pud) const
        {
            static_assert(pi_.isOnboard(pinMap_[pinNumber]), "Pin number is not onboard (onboard pins range from 0 to 31)");
            pinRegistry_.pullUpDnControl<pinMap_[pinNumber], pi_.gpioPupOffset()>(piMemory_.gpioPtr(), pud);
        }

        // Set an output PWM value
        template <const pin_t pinNumber>
        inline void pwmWriteOnboard(const pwm_t value)
        {
            static_assert(pi_.isOnboard(pinMap_[pinNumber]), "Pin number is not onboard (onboard pins range from 0 to 31)");
            static_assert(wiringPiMode_ == WPI_MODE_GPIO(), "wiringPi not initialised in GPIO mode: pwmWriteOnboard cannot be called");
            assert((piMemory_.gpioBase() == 0) && "GPIO base address must be 0: pwmWriteOnboard cannot be called");
            assert((!piMemory_.usingGpioMemory()) && "Application is using GPIO memory. Unable to do this when using /dev/gpiomem. Try sudo?");
            pinRegistry_.pwmWrite<pinMap_[pinNumber]>(piMemory_.pwmPtr(), value);
        }

        // Returns the map of pins
        [[nodiscard]] inline consteval const pinTable &pinMap() const
        {
            return pinMap_;
        }

        // Returns the map of pin modes
        [[nodiscard]] inline constexpr const pinTable &pinModeMap() const
        {
            return pinRegistry_.pinModeMap();
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
            return pinRegistry_.deviceName();
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
        // Pi hardware info
        static constexpr const piHardware pi_ = piHardware();

        // Pi memory addresses
        const piMemory<pi_.model()> piMemory_;

        // Reset all the pins to input mode
        void resetAllPins(std::integral_constant<pin_t, pi_.numPins()>) {}
        template <pin_t index = 0>
        void resetAllPins(std::integral_constant<pin_t, index> = std::integral_constant<pin_t, 0>())
        {
            pinModeOnboard<index>(INPUT<gpio_t>());
            resetAllPins(std::integral_constant<pin_t, index + 1>());
        }

        // Registry of onboard pins
        onboardPins pinRegistry_ = onboardPins();

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
        static constexpr const pinTable pinMap_ = pinMapI<pi_.gpioLayout(), wiringPiMode_>();

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
        static constexpr const int versionMajor_ = VERSION_MAJOR();
        static constexpr const int versionMinor_ = VERSION_MINOR();
    };
}

#endif