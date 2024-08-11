#ifndef __WIRING_PI_H
#define __WIRING_PI_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"
#include "wiringPiTemplateMacros.H"
#include "wiringPiPinTables.H"
#include "onboardPins.H"
#include "piHardwareInfo.H"

namespace WiringPi
{
    static constinit std::atomic<bool> quit(false);

    void signalHandler(int)
    {
        quit.store(true);
    }

    // Check if pin is onboard
    // Returns true only if less than 32
    // This needs to be declared outside of the class to be a constant expression
    // template <const pin_t pinNumber>
    [[nodiscard]] inline consteval bool isOnboard(const pin_t pinNumber)
    {
        return pinNumber < 32;
    }

    // Templated, but at present only works correctly in GPIO mode
    // template <int wpiMode>
    class wiringPi
    {
    public:
        [[nodiscard]] inline wiringPi()
            : gpio(mapHardware<gpio_t>(pi_.GPIO_BASE())),
              pwm(mapHardware<pwm_t>(pi_.GPIO_PWM())),
              clk(mapHardware<clk_t>(pi_.GPIO_CLOCK_BASE())),
              pads(mapHardware<pads_t>(pi_.GPIO_PADS())),
              timer(mapTimer(pi_.GPIO_TIMER())),
              timerIrqRaw(timer + TIMER_IRQ_RAW<timer_t>()),
              epochMilli_(initialiseEpochMilli()),
              epochMicro_(initialiseEpochMicro())
        {
            // Check that the maps succeeded
            assert((gpio != MAP_FAILED));
            assert((pwm != MAP_FAILED));
            assert((clk != MAP_FAILED));
            assert((pads != MAP_FAILED));
            assert((timer != MAP_FAILED));

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
            // Statically checked loop to reset pin modes to input
            resetPins();

            std::cout << "\nwiringPi exited cleanly" << std::endl;
        };

        void resetPins(std::integral_constant<pin_t, 32>) {}

        template <pin_t index = 0>
        void resetPins(std::integral_constant<pin_t, index> = std::integral_constant<pin_t, 0>())
        {
            pinModeOnboard<index>(INPUT<gpio_t>());
            // std::cout << "Reset pin " << index << std ::endl;
            resetPins(std::integral_constant<pin_t, index + 1>());
        }

        // inline void readAll() const
        // {
        //     std::cout << " +------+-----+---+--Model " << pi_.modelString() << "--+---+-----+------+ " << std::endl;
        //     std::cout << " | MODE | wPi | V | pin || pin | V | wPi | MODE | " << std::endl;
        //     std::cout << " +------+-----+---+-----++-----+---+-----+------+ " << std::endl;

        //     for (pin_t i = 0; i < 32; i += 2)
        //     {
        //         std::cout << " |";
        //         std::cout << modeStrings()[pinModeMap()[i]];
        //         std::cout << "| " << pinString(pinMap_[i]);
        //         std::cout << "| " << digitalReadOnboard(i) << " |";
        //         std::cout << pinString(i);
        //         std::cout << " || ";
        //         std::cout << pinString(i + 1);
        //         std::cout << "| " << digitalReadOnboard(i + 1) << " |";
        //         std::cout << pinString(pinMap_[i + 1]);
        //         std::cout << " |";
        //         std::cout << modeStrings()[pinModeMap()[i + 1]] << "|";
        //         std::cout << std::endl;
        //     }

        //     std::cout << " +------+-----+---+--Model " << pi_.modelString() << "--+---+-----+------+ " << std::endl;
        //     std::cout << " | MODE | wPi | V | pin || pin | V | wPi | MODE | " << std::endl;
        //     std::cout << " +------+-----+---+-----++-----+---+-----+------+ " << std::endl;
        // }

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
            static_assert(isOnboard(pinMap_[pinNumber]), "Finished properly");
            return pinRegistry_.digitalRead(gpioPtr(), pinMap_[pinNumber]);
        }

        // Write an output bit
        template <const pin_t pinNumber>
        inline void digitalWriteOnboard(const gpio_t value) const volatile
        {
            static_assert(isOnboard(pinMap_[pinNumber]), "Finished properly");
            pinRegistry_.digitalWrite(gpioPtr(), pinMap_[pinNumber], value);
        }

        // Sets the mode of a pin to be input, output or PWM output
        template <const pin_t pinNumber>
        inline void pinModeOnboard(const gpio_t mode)
        {
            static_assert(isOnboard(pinMap_[pinNumber]), "Finished properly");
            pinRegistry_.pinMode(gpioPtr(), pinNumber, mode, gpioToGPFSEL_[pinNumber], gpioToShift_[pinNumber]);
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

        [[nodiscard]] inline volatile gpio_t *gpioPtr() const volatile
        {
            return gpio;
        }

        [[nodiscard]] inline consteval const std::string_view onboardNames() const
        {
            return pinRegistry_.deviceName();
        }

        // [[nodiscard]] inline consteval const nameTable physNames() const
        [[nodiscard]] inline constexpr const nameTable physNames() const
        {
            return physNames_;
        }

    private:
        // Pi hardware info
        static constexpr const piHardware pi_ = piHardware();

        // Registry of onboard pins
        onboardPins pinRegistry_ = onboardPins();

        // Setup info
        // static constexpr const int wiringPiSetuped_ = static_cast<int>(true);
        // const int wiringPiDebug_;
        // const int wiringPiReturnCodes_;
        static constexpr const int wiringPiMode_ = WPI_MODE_PINS();

        // Offset to the Pull Up Down Clock regsiter
        // This is equivalent to the gpioToPUDCLK table
        // Returns 38 on pins 0 -> 31, 39 on pins 32 -> 63
        [[nodiscard]] inline consteval gpio_t gpioToPUDCLK(const gpio_t i)
        {
            return 38 + (i / 32);
        }

        // Map a BCM_GPIO pin to its function selection control port. (GPFSEL 0-5)
        // Groups of 10 - 3 bits per Function - 30 bits per port
        static constexpr const pinTable gpioToGPFSEL_ = gpioToGPFSEL();

        // Define the shift up for the 3 bits per pin in each GPFSEL port
        static constexpr const pinTable gpioToShift_ = gpioToShift();

        // Pin conversion table
        static constexpr const pinTable pinMap_ = pinMapI(wiringPiMode_, pi_.gpioLayout());
        static constexpr const pinTable physToGpio_ = physToGpioI(pi_.gpioLayout());

        // Physical pin names
        static constexpr const nameTable physNames_ = physNamesI();

        // System mode pin table cannot be constexpr
        const pinTable sysFds_ = sysFdsI(wiringPiMode_);

        // Check if using debug environment
        // [[nodiscard]] inline int isDebug()
        // {
        // if (getenv(ENV_DEBUG()) != NULL)
        // {
        //     return static_cast<int>(true);
        // }
        // else
        // {
        //     return static_cast<int>(false);
        // }
        // return true;
        // }

        // Check if using return codes
        // [[nodiscard]] inline int isReturnCodes() const
        // {
        // if (getenv(ENV_CODES()) != NULL)
        // {
        //     return static_cast<int>(true);
        // }
        // else
        // {
        //     return static_cast<int>(false);
        // }
        // return true;
        // }

        // Check access to device memory
        const int fd_ = checkDevMem();
        [[nodiscard]] inline int checkDevMem() const
        {
            int fd;
            constexpr const char *gpiomemGlobal = "/dev/mem";
            constexpr const char *gpiomemModule = "/dev/gpiomem";

            if (gpiomemGlobal == NULL || (fd = open(gpiomemGlobal, O_RDWR | O_SYNC | O_CLOEXEC)) < 0)
            {
                std::cout << "wiringPi: No access to /dev/mem: try /dev/gpiomem" << std::endl;
                if (!(gpiomemModule && (fd = open(gpiomemModule, O_RDWR | O_SYNC | O_CLOEXEC)) >= 0))
                {
                    return failure(false, "wiringPiSetup: Unable to open %s or %s: %s.\n"
                                          "  Aborting your program because if it can not access the GPIO\n"
                                          "  hardware then it most certianly won't work\n"
                                          "  Try running with sudo?\n",
                                   gpiomemGlobal, gpiomemModule, strerror(errno));
                }
            }

            std::cout << "wiringPi: access to " << (pi_.usingGpioMem() ? gpiomemModule : gpiomemGlobal) << " succeded (" << fd << ")" << std::endl;
            return fd;
        }

        // Map individual hardware components
        template <typename T0, typename T1>
        [[nodiscard]] volatile T0 *mapHardware(const T1 offset) const
        {
            return static_cast<volatile T0 *>(mmap(0, BLOCK_SIZE<size_t>(), PROT_READ | PROT_WRITE, MAP_SHARED, fd_, static_cast<__off_t>(offset)));
        }

        // Map timer
        [[nodiscard]] volatile timer_t *mapTimer(const int offset) const
        {
            volatile timer_t *volatile ptr = static_cast<volatile timer_t *>(mmap(0, BLOCK_SIZE<size_t>(), PROT_READ | PROT_WRITE, MAP_SHARED, fd_, static_cast<__off_t>(offset)));
            *(ptr + TIMER_CONTROL<timer_t>()) = 0x0000280;
            *(ptr + TIMER_PRE_DIV<timer_t>()) = 0x00000F9;
            return ptr;
        }

        // Hardware pointers
        volatile timer_t *volatile gpio;
        volatile timer_t *volatile pwm;
        volatile timer_t *volatile clk;
        volatile timer_t *volatile pads;
        volatile timer_t *volatile timer;
        volatile timer_t *volatile timerIrqRaw;

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