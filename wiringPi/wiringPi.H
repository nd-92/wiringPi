#ifndef __WIRING_PI_H
#define __WIRING_PI_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"
#include "wiringPiTemplateMacros.H"
#include "wiringPiPinTables.H"
#include "wiringPiNode.H"
#include "onboardPin.H"
#include "nullDevice.H"

namespace WiringPi
{
    // Templated, but at present only works correctly in GPIO mode
    template <int wpiMode>
    class wiringPi
    {
    public:
        wiringPi()
            : wiringPiDebug_(isDebug()),
              wiringPiReturnCodes_(isReturnCodes()),
              piGpioBase_(piGpioBase(piModel_)),
              usingGpioMem_(usingGpioMem()),
              GPIO_PADS(piGpioBase_ + 0x00100000),
              GPIO_CLOCK_BASE(piGpioBase_ + 0x00101000),
              GPIO_BASE(piGpioBase_ + 0x00200000),
              GPIO_TIMER(piGpioBase_ + 0x0000B000),
              GPIO_PWM(piGpioBase_ + 0x0020C000),
              gpio(mapHardware<gpio_t>(GPIO_PWM)),
              pwm(mapHardware<pwm_t>(GPIO_PWM)),
              clk(mapHardware<clk_t>(GPIO_CLOCK_BASE)),
              pads(mapHardware<pads_t>(GPIO_PADS)),
              timer(mapTimer(GPIO_TIMER)),
              timerIrqRaw(timer + TIMER_IRQ_RAW<timer_t>()),
              epochMilli_(initialiseEpochMilli()),
              epochMicro_(initialiseEpochMicro())
        {
            // Add GPIO to the device registry on pins 0 to 39
            // Should ultimately be changed to deal with other modes - use only GPIO for now
            for (size_t i = 0; i < 40; i++)
            {
                // Initialise the device
                const WiringPi::onboardPin device(i);

                // Add the device name to the name database
                pinRegistry_.push_back(1);
                devNames_.push_back(device.deviceName());

                // Add device to pointer registry
                devicePtrs_.push_back(std::make_unique<onboardPin>(device));

                // Add its occupied pins to the pinStart and pinEnd arrays
                pinStart_.push_back(i);
                pinEnd_.push_back(i);
            }

            // Add null devices to the device registry on pins 40 to 63
            for (size_t i = 40; i < 64; i++)
            {
                const WiringPi::nullDevice device(i);
                pinRegistry_.push_back(1);
                devNames_.push_back(device.deviceName());
                devicePtrs_.push_back(std::make_unique<nullDevice>(device));
                pinStart_.push_back(i);
                pinEnd_.push_back(i);
            }

            // Resize correctly - only 64 pins
            pinRegistry_.resize(64);
            devNames_.resize(64);
            devicePtrs_.resize(64);
            pinStart_.resize(64);
            pinEnd_.resize(64);
        }

        ~wiringPi(){};

        // Read an input bit
        inline gpio_t digitalReadOnboard(const size_t deviceNo) const
        {
            // std::cout << "Called wiringPi digitalRead on onboard device number " << deviceNo << std::endl;
            return devicePtrs_[deviceNo]->digitalRead(gpioConstPtr(), gpioToGPLEV(deviceNo));
        }
        inline gpio_t digitalReadOffboard(const size_t deviceNo) const
        {
            // std::cout << "Called wiringPi digitalRead on offboard device number " << deviceNo << std::endl;
            return devicePtrs_[deviceNo + 64]->digitalRead();
        }

        // Write an output bit
        inline void digitalWriteOnboard(const size_t deviceNo, const gpio_t value)
        {
            devicePtrs_[deviceNo]->digitalWrite(gpioPtr(), gpioCLRSET(deviceNo + (64 * !value)));
        }
        inline void digitalWriteOffboard(const size_t deviceNo, const gpio_t value)
        {
            devicePtrs_[deviceNo + 64]->digitalWrite(value);
        }

        // Set an output pwm value
        inline void pwmWriteOnboard(const size_t deviceNo, const int value)
        {
            devicePtrs_[deviceNo]->pwmWrite(pwmPtr(), gpioToPwmPort_[deviceNo], value);
        }
        inline void pwmWriteOffboard(const size_t deviceNo, const int value)
        {
            devicePtrs_[deviceNo + 64]->pwmWrite(value);
        }

        // Read the analog value of a given pin
        inline int analogReadOffboard(const size_t deviceNo) const
        {
            return devicePtrs_[deviceNo + 64]->analogRead();
        }

        // Write the analog value of a given pin
        inline void analogWriteOffboard(const size_t deviceNo, const int value)
        {
            devicePtrs_[deviceNo + 64]->analogWrite(value);
        }

        inline constexpr const pinTable &pinToGpio() const
        {
            return pinToGpio_;
        }

        inline constexpr const pinTable &physToGpio() const
        {
            return physToGpio_;
        }

        inline constexpr const pinTable &sysFds() const
        {
            return sysFds_;
        }

        inline void delay(const time_t howLong) const
        {
            struct timespec sleeper, dummy;

            sleeper.tv_sec = howLong / 1000;
            sleeper.tv_nsec = (howLong % 1000) * 1000000;

            nanosleep(&sleeper, &dummy);
        }

        inline void delayMicrosecondsHard(const time_t howLong) const
        {
            struct timeval tNow, tLong, tEnd;

            gettimeofday(&tNow, NULL);
            tLong.tv_sec = howLong / 1000000;
            tLong.tv_usec = howLong % 1000000;
            timeradd(&tNow, &tLong, &tEnd);

            while (timercmp(&tNow, &tEnd, <))
            {
                gettimeofday(&tNow, NULL);
            }
        }

        inline void delayMicroseconds(const time_t howLong) const
        {
            struct timespec sleeper;
            if (howLong == 0)
            {
                return;
            }
            else if (howLong < 100)
            {
                delayMicrosecondsHard(howLong);
            }
            else
            {
                sleeper.tv_sec = (howLong / 1000000);
                sleeper.tv_nsec = (howLong % 1000000) * 1000L;
                nanosleep(&sleeper, NULL);
            }
        }

        inline time_t micros() const
        {
            struct timespec ts;

            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);

            return (ts.tv_sec * 1000000L) + (ts.tv_nsec / 1000L) - epochMicro();
        }

        inline time_t millis() const
        {
            struct timespec ts;

            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);

            return (ts.tv_sec * 1000L) + (ts.tv_nsec / 1000000L) - epochMilli();
        }

        inline constexpr int mode() const
        {
            return wiringPiMode_;
        }

        inline constexpr const gpio_t *gpioConstPtr() const
        {
            return gpio;
        }
        inline constexpr gpio_t *gpioPtr()
        {
            return gpio;
        }

        inline constexpr const int *pwmConstPtr() const
        {
            return pwm;
        }
        inline constexpr int *pwmPtr()
        {
            return pwm;
        }

        template <class deviceType>
        inline void addDevice(deviceType device)
        {
            // Resize to accomodate the total pin count
            // Set pins by default to unused
            pinRegistry_.resize(std::max(device.pinBase() + device.numPins() + 64, pinRegistry_.size()), 0);

            // Add the used pins to the pin count
            for (size_t i = device.pinBase(); i < device.pinBase() + device.numPins(); i++)
            {
                pinRegistry_[i + 64] = pinRegistry_[i + 64] + 1;
            }

            // Size appropriately - don't add more than needed
            const size_t newSize = pinStart_.size() + 1;
            pinStart_.resize(newSize);
            pinEnd_.resize(newSize);
            devNames_.resize(newSize);
            devicePtrs_.resize(newSize);

            // // Add device to end
            pinStart_[newSize - 1] = device.pinBase() + 64;
            pinEnd_[newSize - 1] = device.pinMax() + 64;
            devNames_[newSize - 1] = device.deviceName();
            devicePtrs_[newSize - 1] = std::make_unique<deviceType>(device);
        }

        inline void printPinRegistry() const
        {
            std::cout << "========================" << std::endl;
            std::cout << "WiringPi device registry" << std::endl;
            std::cout << "========================" << std::endl;
            std::cout << "Total registered pins: " << pinRegistry_.size() << std::endl;

            // Validate the pin registry
            if (validatePinRegistry())
            {
                std::cout << "Device registry valid" << std::endl;
            }
            else
            {
                std::cout << "Device registry invalid" << std::endl;
            }

            // Print the device registry pins:
            std::cout << "Device pin layout:" << std::endl;
            std::cout << "{ ";
            for (size_t i = 0; i < pinRegistry_.size(); i++)
            {
                if (((i % 8) == 0) && (i > 0))
                {
                    std::cout << "} " << i - 8 << " -> " << i - 1 << std::endl;
                    std::cout << "{ ";
                }
                std::cout << pinRegistry_[i] << " ";
            }
            std::cout << "} " << pinRegistry_.size() - 8 << " -> " << pinRegistry_.size() - 1 << std::endl;

            // Print the device names and their assigned pins
            std::cout << "Offboard devices:" << std::endl;
            for (size_t i = 64; i < devNames_.size(); i++)
            {
                if (pinStart_[i] == pinEnd_[i])
                {
                    std::cout << devNames_[i] << " registered on pin " << pinStart_[i] << std::endl;
                }
                else
                {
                    std::cout << devNames_[i] << " registered on pins " << pinStart_[i] << " -> " << pinEnd_[i] << std::endl;
                }
            }
        }

        inline bool validatePinRegistry() const
        {
            // Check for pin overlap
            for (size_t i = 0; i < pinRegistry_.size(); i++)
            {
                if (pinRegistry_[i] > 1)
                {
                    // Return false if overlap
                    return false;
                }
            }
            // Return true if no overlap
            return true;
        }

        inline const std::vector<std::string> &devNames() const
        {
            return devNames_;
        }

        // inline void pwmSetRange(const int range)
        // {
        //     *(pwmPtr() + PWM0_RANGE<int>()) = range;
        //     delayMicroseconds(10);
        //     *(pwmPtr() + PWM1_RANGE<int>()) = range;
        //     delayMicroseconds(10);
        // }

        // inline void pwmToneWrite(const int pin, const int freq)
        // {
        //     // FailOnModel5();

        //     // setupCheck("pwmToneWrite");

        //     if (freq == 0)
        //     {
        //         pwmWrite(pin, 0);
        //     } // Off
        //     else
        //     {
        //         int range = 600000 / freq;
        //         pwmSetRange(range);
        //         pwmWrite(pin, freq / 2);
        //     }
        // }

    private:
        // Basic info
        static constexpr const int piModel_ = PI_MODEL<int>();
        static constexpr const int gpioLayout_ = GPIO_LAYOUT<int>();
        static constexpr const int piRev_ = PI_REV<int>();
        static constexpr const int piMem_ = PI_MEM<int>();
        static constexpr const int piMaker_ = PI_MAKER<int>();
        static constexpr const int piWarranty_ = PI_WARRANTY<int>();

        // Registry of devices
        std::vector<pin_t> pinRegistry_;
        std::vector<std::string> devNames_;
        std::vector<size_t> pinStart_;
        std::vector<size_t> pinEnd_;
        std::vector<std::unique_ptr<WiringPi::wiringPiNode>> devicePtrs_;

        // Setup info
        static constexpr const int wiringPiSetuped_ = static_cast<int>(true);
        const int wiringPiDebug_;
        const int wiringPiReturnCodes_;
        static constexpr const int wiringPiMode_ = getMode(wpiMode, piModel_);

        int checkDevMem()
        {
            int fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC);
            fd = open("/dev/gpiomem", O_RDWR | O_SYNC | O_CLOEXEC);
            return fd;
        }

        // Offset to the GPIO Clear and Set registers for each GPIO pin
        // This is equivalent to the gpioToGPCLR and gpioToGPSET tables
        // Value passed to this function should be deviceNo + (64 * !value)
        // Returns 10 -> 11 for digital value LOW, 7 -> 8 for digital value HIGH
        inline constexpr gpio_t gpioCLRSET(const pin_t i) const
        {
            return 7 + (i / 64) + (i / 32);
        }

        // Offset to the GPIO Input level registers for each GPIO pin
        // This is equivalent to the gpioToGPLEV table
        // Returns 13 on pins 0 -> 31, 14 on pins 32 -> 63
        inline constexpr gpio_t gpioToGPLEV(const pin_t i) const
        {
            return 13 + (i / 32);
        }

        // Pin conversion tables
        static constexpr const pinTable pinToGpio_ = pinToGpioI(gpioLayout_);
        static constexpr const pinTable physToGpio_ = physToGpioI(gpioLayout_);
        static constexpr const pinTable gpioToPwmPort_ = gpioToPwmPortI();

        // System mode pin table cannot be constexpr
        const pinTable sysFds_ = sysFdsI(wiringPiMode_);

        const int piGpioBase_;
        static constexpr const int piGpioPupOffset_ = piGpioPupOffset<int>(piModel_);
        const int usingGpioMem_;

        // Private methods for initialising const member variables

        // Check if using debug environment
        inline int isDebug()
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
        inline int isReturnCodes()
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

        // Initialise the base address of the GPIO memory mapped hardware IO
        inline int piGpioBase(const int model)
        {
            int i = 0;
            // Initialise piGpioBase and piGpioPupOffset from board model - default vals 1 and 0
            switch (model)
            {
            case PI_MODEL_A():
            case PI_MODEL_B():
            case PI_MODEL_AP():
            case PI_MODEL_BP():
            case PI_ALPHA():
            case PI_MODEL_CM():
            case PI_MODEL_ZERO():
            case PI_MODEL_ZERO_W():
                i = GPIO_PERI_BASE_OLD<int>();
                break;
            case PI_MODEL_4B():
            case PI_MODEL_400():
            case PI_MODEL_CM4():
                i = GPIO_PERI_BASE_2711<int>();
                break;
            default:
                i = GPIO_PERI_BASE_2835<int>();
                break;
            }

            int fd;
            if ((fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0)
            {
                if ((fd = open("/dev/gpiomem", O_RDWR | O_SYNC | O_CLOEXEC)) >= 0) // We're using gpiomem
                {
                    i = 0;
                    // usingGpioMem = true;
                }
                else
                {
                    // return failure(
                    //     WPI_ALMOST,
                    //     "wiringPiSetup: Unable to open /dev/mem or /dev/gpiomem: %s.\n"
                    //     "  Aborting your program because if it can not access the GPIO\n"
                    //     "  hardware then it most certianly won't work\n"
                    //     "  Try running with sudo?\n",
                    //     strerror(errno));
                }
            }

            return i;
        }

        inline int usingGpioMem()
        {
            int fd;
            if ((fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0)
            {
                if ((fd = open("/dev/gpiomem", O_RDWR | O_SYNC | O_CLOEXEC)) >= 0) // We're using gpiomem
                {
                    return static_cast<int>(true);
                }
                else
                {
                    failure(
                        WPI_ALMOST<int>(),
                        "wiringPiSetup: Unable to open /dev/mem or /dev/gpiomem: %s.\n"
                        "  Aborting your program because if it can not access the GPIO\n"
                        "  hardware then it most certianly won't work\n"
                        "  Try running with sudo?\n",
                        strerror(errno));
                }
            }
            return static_cast<int>(false);
        }

        const int GPIO_PADS;
        const int GPIO_CLOCK_BASE;
        const int GPIO_BASE;
        const int GPIO_TIMER;
        const int GPIO_PWM;

        // Map individual hardware components
        template <typename T0, typename T1>
        T0 *mapHardware(const T1 offset)
        {
            return static_cast<T0 *>(mmap(0, BLOCK_SIZE<size_t>(), PROT_READ | PROT_WRITE, MAP_SHARED, checkDevMem(), static_cast<__off_t>(offset)));
        }

        const timer_t *mapTimer(const int offset)
        {
            timer_t *ptr = static_cast<timer_t *>(mmap(0, BLOCK_SIZE<size_t>(), PROT_READ | PROT_WRITE, MAP_SHARED, checkDevMem(), static_cast<__off_t>(offset)));
            *(ptr + TIMER_CONTROL<timer_t>()) = 0x0000280;
            *(ptr + TIMER_PRE_DIV<timer_t>()) = 0x00000F9;
            return ptr;
        }

        // Probably should not be using raw pointers
        gpio_t *gpio;
        pwm_t *pwm;
        const clk_t *clk;
        const pads_t *pads;
        const timer_t *timer;
        const timer_t *timerIrqRaw;

        const time_t epochMilli_;
        time_t initialiseEpochMilli()
        {
            struct timespec ts;

            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000L);
        }
        inline time_t epochMilli() const
        {
            return epochMilli_;
        }

        const time_t epochMicro_;
        time_t initialiseEpochMicro()
        {
            struct timespec ts;

            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            return (ts.tv_sec * 1000000) + (ts.tv_nsec / 1000L);
        }
        inline time_t epochMicro() const
        {
            return epochMicro_;
        }

        static constexpr const char *version_ = VERSION();
        static constexpr const int versionMajor_ = VERSION_MAJOR<int>();
        static constexpr const int versionMinor_ = VERSION_MINOR<int>();
    };
}

#endif