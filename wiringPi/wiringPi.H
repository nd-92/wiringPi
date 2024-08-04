#ifndef __WIRING_PI_H
#define __WIRING_PI_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"
#include "wiringPiTemplateMacros.H"
#include "wiringPiPinTables.H"
#include "wiringPiNode.H"
#include "onboardPins.H"
#include "nullDevice.H"

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
              gpio(mapHardware<gpio_t>(GPIO_BASE)),
              pwm(mapHardware<pwm_t>(GPIO_PWM)),
              clk(mapHardware<clk_t>(GPIO_CLOCK_BASE)),
              pads(mapHardware<pads_t>(GPIO_PADS)),
              timer(mapTimer(GPIO_TIMER)),
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
        [[nodiscard]] inline gpio_t digitalReadOnboard(const pin_t pinNumber) const
        {
            // std::cout << "Called wiringPi digitalRead on onboard device number " << deviceNo << std::endl;
            return pinRegistry_.digitalRead(gpioConstPtr(), pinNumber);
        }
        // inline gpio_t digitalReadOffboard(const size_t deviceNo) const
        // {
        //     // std::cout << "Called wiringPi digitalRead on offboard device number " << deviceNo << std::endl;
        //     return devicePtrs_[deviceNo + 64]->digitalRead();
        // }

        // Write an output bit
        inline void digitalWriteOnboard(const pin_t pinNumber, const gpio_t value)
        {
            pinRegistry_.digitalWrite(gpioPtr(), pinNumber, value);
        }
        // inline void digitalWriteOffboard(const size_t deviceNo, const gpio_t value)
        // {
        //     devicePtrs_[deviceNo + 64]->digitalWrite(value);
        // }

        // Set an output pwm value
        // inline void pwmWriteOnboard(const size_t deviceNo, const gpio_t value)
        // {
        //     devicePtrs_[deviceNo]->pwmWrite(pwmPtr(), gpioToPwmPort()[deviceNo], value);
        // }
        // inline void pwmWriteOffboard(const size_t deviceNo, const pwm_t value)
        // {
        //     devicePtrs_[deviceNo + 64]->pwmWrite(value);
        // }

        // Control the internal pull-up/down resistors on a GPIO pin
        // inline void pullUpDnControlOnboard(const size_t deviceNo, const gpio_t value)
        // {
        //     devicePtrs_[deviceNo]->pullUpDnControl(gpioPtr(), value, piGpioPupOffset_);
        // }

        // Read the analog value of a given pin
        // inline int analogReadOffboard(const size_t deviceNo) const
        // {
        //     return devicePtrs_[deviceNo + 64]->analogRead();
        // }

        // Write the analog value of a given pin
        // inline void analogWriteOffboard(const size_t deviceNo, const int value)
        // {
        //     devicePtrs_[deviceNo + 64]->analogWrite(value);
        // }

        [[nodiscard]] inline constexpr const pinTable &gpioToPwmPort() const
        {
            return gpioToPwmPort_;
        }

        [[nodiscard]] inline constexpr const pinTable &pinToGpio() const
        {
            return pinToGpio_;
        }

        [[nodiscard]] inline constexpr const pinTable &physToGpio() const
        {
            return physToGpio_;
        }

        [[nodiscard]] inline constexpr const pinTable &sysFds() const
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

        [[nodiscard]] inline constexpr int mode() const
        {
            return wiringPiMode_;
        }

        [[nodiscard]] inline constexpr const volatile gpio_t *gpioConstPtr() const
        {
            return gpio;
        }
        [[nodiscard]] inline constexpr volatile gpio_t *gpioPtr()
        {
            return gpio;
        }

        [[nodiscard]] inline constexpr const volatile pwm_t *pwmConstPtr() const
        {
            return pwm;
        }
        [[nodiscard]] inline constexpr volatile pwm_t *pwmPtr()
        {
            return pwm;
        }

        [[nodiscard]] inline consteval std::string_view onboardNames() const
        {
            return pinRegistry_.deviceName();
        }

    private:
        // Basic info
        static constexpr const int piModel_ = PI_MODEL<int>();
        static constexpr const int gpioLayout_ = GPIO_LAYOUT<int>();
        static constexpr const int piRev_ = PI_REV<int>();
        static constexpr const int piMem_ = PI_MEM<int>();
        static constexpr const int piMaker_ = PI_MAKER<int>();
        static constexpr const int piWarranty_ = PI_WARRANTY<int>();

        // Registry of devices
        static constinit onboardPins pinRegistry_;

        // Setup info
        static constexpr const int wiringPiSetuped_ = static_cast<int>(true);
        const int wiringPiDebug_;
        const int wiringPiReturnCodes_;
        static constexpr const int wiringPiMode_ = WPI_MODE_GPIO();

        [[nodiscard]] inline int checkDevMem()
        {
            int fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC);
            fd = open("/dev/gpiomem", O_RDWR | O_SYNC | O_CLOEXEC);
            return fd;
        }

        // Offset to the Pull Up Down Clock regsiter
        // This is equivalent to the gpioToPUDCLK table
        // Returns 38 on pins 0 -> 31, 39 on pins 32 -> 63
        [[nodiscard]] inline constexpr gpio_t gpioToPUDCLK(const gpio_t i)
        {
            return 38 + (i / 32);
        }

        // Map a BCM_GPIO pin to its function selection control port. (GPFSEL 0-5)
        // Groups of 10 - 3 bits per Function - 30 bits per port
        [[nodiscard]] inline constexpr size_t gpioToGPFSEL(const size_t i) const
        {
            return i / 10;
        }

        // Define the shift up for the 3 bits per pin in each GPFSEL port
        [[nodiscard]] inline constexpr size_t gpioToShift(const size_t i) const
        {
            return 3 * (i % 10);
        }

        // Helper function to set bits to output or input
        // constexpr size_t bitSetI(const gpio_t *gpioPtr, const size_t pinMode, const size_t deviceNo) const
        // {
        //     const int64_t fSel = static_cast<int64_t>(gpioToGPFSEL(deviceNo));
        //     const int64_t shift = static_cast<int64_t>(gpioToShift(deviceNo));

        //     const int64_t bitSet = (*(dynamic_cast<const int64_t *>(gpioPtr) + fSel) & ~(7 << shift));

        //     if (pinMode == OUTPUT<size_t>())
        //     {
        //         std::cout << "Pin mode is output" << std::endl;
        //         // bitSet = (*(gpioPtr + fSel) & ~(7 << shift)) | (1 << shift);
        //     }

        //     return static_cast<size_t>(bitSet);
        // }

        // Pin conversion tables
        static constexpr const pinTable pinToGpio_ = pinToGpioI(gpioLayout_);
        static constexpr const pinTable physToGpio_ = physToGpioI(gpioLayout_);
        static constexpr const pinTable gpioToPwmPort_ = gpioToPwmPortI();

        // System mode pin table cannot be constexpr
        const pinTable sysFds_ = sysFdsI(wiringPiMode_);

        static constexpr const int piGpioBase_ = piGpioBase(piModel_);
        static constexpr const int piGpioPupOffset_ = piGpioPupOffset<int>(piModel_);
        static constexpr const bool usingGpioMem_ = USING_GPIOMEM<bool>();
        static constexpr const int GPIO_PADS = piGpioBase_ + 0x00100000;
        static constexpr const int GPIO_CLOCK_BASE = piGpioBase_ + 0x00101000;
        static constexpr const int GPIO_BASE = piGpioBase_ + 0x00200000;
        static constexpr const int GPIO_TIMER = piGpioBase_ + 0x0000B000;
        static constexpr const int GPIO_PWM = piGpioBase_ + 0x0020C000;

        // Private methods for initialising const member variables

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
        [[nodiscard]] inline int isReturnCodes()
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

        // Map individual hardware components
        template <typename T0, typename T1>
        [[nodiscard]] volatile T0 *mapHardware(const T1 offset)
        {
            return static_cast<T0 *>(mmap(0, BLOCK_SIZE<size_t>(), PROT_READ | PROT_WRITE, MAP_SHARED, checkDevMem(), static_cast<__off_t>(offset)));
        }

        [[nodiscard]] const timer_t *mapTimer(const int offset)
        {
            timer_t *ptr = static_cast<timer_t *>(mmap(0, BLOCK_SIZE<size_t>(), PROT_READ | PROT_WRITE, MAP_SHARED, checkDevMem(), static_cast<__off_t>(offset)));
            *(ptr + TIMER_CONTROL<timer_t>()) = 0x0000280;
            *(ptr + TIMER_PRE_DIV<timer_t>()) = 0x00000F9;
            return ptr;
        }

        // Probably should not be using raw pointers
        volatile gpio_t *gpio;
        volatile pwm_t *pwm;
        const volatile clk_t *clk;
        const volatile pads_t *pads;
        const volatile timer_t *timer;
        const volatile timer_t *timerIrqRaw;

        const time_t epochMilli_;
        [[nodiscard]] inline time_t initialiseEpochMilli() const
        {
            struct timespec ts;

            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000L);
        }
        [[nodiscard]] inline time_t epochMilli() const
        {
            return epochMilli_;
        }

        const time_t epochMicro_;
        [[nodiscard]] inline time_t initialiseEpochMicro() const
        {
            struct timespec ts;

            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            return (ts.tv_sec * 1000000) + (ts.tv_nsec / 1000L);
        }
        [[nodiscard]] inline time_t epochMicro() const
        {
            return epochMicro_;
        }

        static constexpr std::string_view version_ = VERSION();
        static constexpr const int versionMajor_ = VERSION_MAJOR();
        static constexpr const int versionMinor_ = VERSION_MINOR();
    };
}

#endif