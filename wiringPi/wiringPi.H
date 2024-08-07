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
    // Templated, but at present only works correctly in GPIO mode
    // template <int wpiMode>
    class wiringPi
    {
    public:
        [[nodiscard]] inline wiringPi()
            : wiringPiDebug_(isDebug()),
              wiringPiReturnCodes_(isReturnCodes()),
              gpio(mapHardware<gpio_t>(pi_.GPIO_BASE())),
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
        }

        ~wiringPi() {};

        // Read an input bit
        [[nodiscard]] inline gpio_t digitalReadOnboard(const pin_t pinNumber) const volatile
        {
            return pinRegistry_.digitalRead(gpioPtr(), pinMap_[pinNumber]);
        }

        // Write an output bit
        inline void digitalWriteOnboard(const pin_t pinNumber, const gpio_t value) const volatile
        {
            pinRegistry_.digitalWrite(gpioPtr(), pinMap_[pinNumber], value);
        }

        [[nodiscard]] inline consteval const pinTable &pinMap() const
        {
            return pinMap_;
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

        [[nodiscard]] inline consteval std::string_view onboardNames() const
        {
            return pinRegistry_.deviceName();
        }

    private:
        // Pi hardware info
        static constexpr const WiringPi::piHardware pi_ = piHardware();

        // Registry of onboard pins
        static constexpr const WiringPi::onboardPins pinRegistry_ = onboardPins();

        // Setup info
        static constexpr const int wiringPiSetuped_ = static_cast<int>(true);
        const int wiringPiDebug_;
        const int wiringPiReturnCodes_;
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
        [[nodiscard]] inline consteval pin_t gpioToGPFSEL(const pin_t i) const
        {
            return i / 10;
        }

        // Define the shift up for the 3 bits per pin in each GPFSEL port
        [[nodiscard]] inline consteval pin_t gpioToShift(const pin_t i) const
        {
            return 3 * (i % 10);
        }

        // Pin conversion table
        static constexpr const pinTable pinMap_ = pinMapI(wiringPiMode_, pi_.gpioLayout());

        // System mode pin table cannot be constexpr
        const pinTable sysFds_ = sysFdsI(wiringPiMode_);

        // Check if using debug environment
        [[nodiscard]] inline int isDebug()
        {
            // if (getenv(ENV_DEBUG()) != NULL)
            // {
            //     return static_cast<int>(true);
            // }
            // else
            // {
            //     return static_cast<int>(false);
            // }
            return true;
        }

        // Check if using return codes
        [[nodiscard]] inline int isReturnCodes() const
        {
            // if (getenv(ENV_CODES()) != NULL)
            // {
            //     return static_cast<int>(true);
            // }
            // else
            // {
            //     return static_cast<int>(false);
            // }
            return true;
        }

        // Check access to device memory
        const int fd_ = checkDevMem();
        [[nodiscard]] inline int checkDevMem() const
        {
            int fd;
            constexpr const char *gpiomemGlobal = "/dev/mem";
            constexpr const char *gpiomemModule = "/dev/gpiomem";

            if (gpiomemGlobal == NULL || (fd = open(gpiomemGlobal, O_RDWR | O_SYNC | O_CLOEXEC)) < 0)
            {
                std::cout << "wiringPi: No access to " << gpiomemGlobal << ": try " << gpiomemModule << std::endl;
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