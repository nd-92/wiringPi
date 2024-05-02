#ifndef __WIRING_PI_H
#define __WIRING_PI_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"
#include "wiringPiTemplateMacros.H"

void printMode(const int mode)
{
    switch (mode)
    {
    case 0:
        std::cout << "Mode = WPI_MODE_PINS" << std::endl;
        break;
    case 1:
        std::cout << "Mode = WPI_MODE_GPIO" << std::endl;
        break;
    case 2:
        std::cout << "Mode = WPI_MODE_GPIO_SYS" << std::endl;
        break;
    case 3:
        std::cout << "Mode = WPI_MODE_PHYS" << std::endl;
        break;
    case 4:
        std::cout << "Mode = WPI_MODE_PIFACE" << std::endl;
        break;
    default:
        std::cout << "Mode = WPI_MODE_UNINITIALISED" << std::endl;
        break;
    }
}

namespace WiringPi
{
    template <int wpiMode>
    class wiringPi
    {
    public:
        wiringPi()
            : wiringPiDebug_(isDebug()),
              wiringPiReturnCodes_(isReturnCodes()),
              pinToGpio_(pinToGpioI(gpioLayout_)),
              physToGpio_(physToGpioI(gpioLayout_)),
              sysFds_(sysFdsI(wiringPiMode_)),
              piGpioBase_(piGpioBase(piModel_)),
              usingGpioMem_(usingGpioMem()),
              GPIO_PADS(piGpioBase_ + 0x00100000),
              GPIO_CLOCK_BASE(piGpioBase_ + 0x00101000),
              GPIO_BASE(piGpioBase_ + 0x00200000),
              GPIO_TIMER(piGpioBase_ + 0x0000B000),
              GPIO_PWM(piGpioBase_ + 0x0020C000),
              gpio(mapHardware<const int>(GPIO_PWM)),
              pwm(mapHardware<const int>(GPIO_PWM)),
              clk(mapHardware<const int>(GPIO_CLOCK_BASE)),
              pads(mapHardware<const int>(GPIO_PADS)),
              // timer(mapHardware<const int>(GPIO_TIMER, true)),
              timer(mapTimer(GPIO_TIMER)),
              timerIrqRaw(timer + TIMER_IRQ_RAW<int>()),
              _wiringPiGpio(gpio),
              _wiringPiPwm(pwm),
              _wiringPiClk(clk),
              _wiringPiPads(pads),
              _wiringPiTimer(timer),
              epochMilli_(initialiseEpochMilli()),
              epochMicro_(initialiseEpochMicro())
        {
            // if (wiringPiDebug_)
            // {
            //     std::cout << "wiringPi: wiringPiSetup called" << std::endl;
            //     std::cout << "Rev: " << PI_REV<int>() << std::endl;
            //     std::cout << "Maker: " << PI_MAKER<int>() << std::endl;
            //     std::cout << "Model: " << PI_MODEL<int>() << std::endl;
            //     std::cout << "Memory: " << PI_MEM<int>() << std::endl;
            //     std::cout << "Warranty: " << PI_WARRANTY<int>() << std::endl;
            //     std::cout << "Epoch: " << epochMilli() << " milliseconds" << std::endl;
            //     std::cout << "Epoch micro: " << epochMicro() << " microseconds" << std::endl;
            //     std::cout << "Now: " << millis() << " milliseconds" << std::endl;
            //     std::cout << "Now: " << micros() << " microseconds" << std::endl;
            //     printMode(wiringPiMode_);
            // }
        }

        ~wiringPi(){};

        inline int digitalRead(int pin) const
        {
            // struct wiringPiNodeStruct *node = wiringPiNodes;

            if ((pin & PI_GPIO_MASK<int>()) == 0) // On-Board Pin
            {
                if (wiringPiMode_ == WPI_MODE_GPIO_SYS<int>())
                {
                    return LOW<int>();
                }
                else if (wiringPiMode_ == WPI_MODE_PINS<int>())
                {
                    pin = pinToGpio_[static_cast<size_t>(pin)];
                }
                else if (wiringPiMode_ == WPI_MODE_PHYS<int>())
                {
                    pin = physToGpio_[static_cast<size_t>(pin)];
                }
                else if (wiringPiMode_ != WPI_MODE_GPIO<int>())
                {
                    return LOW<int>();
                }

                // if (PI_MODEL_5 == RaspberryPiModel)
                // {
                //     switch (gpio[2 * pin] & RP1_STATUS_LEVEL_MASK)
                //     {
                //     default: // 11 or 00 not allowed, give LOW!
                //     case RP1_STATUS_LEVEL_LOW:
                //     {
                //         return LOW;
                //     }
                //     case RP1_STATUS_LEVEL_HIGH:
                //     {
                //         return HIGH;
                //     }
                //     }
                // }
                // else
                // {
                if ((*(gpio + gpioToGPLEV_[static_cast<size_t>(pin)]) & (1 << (pin & 31))) != 0)
                {
                    return HIGH<int>();
                }
                else
                {
                    return LOW<int>();
                }
                // }
            }
            else
            {
                // if ((node = wiringPiFindNode(pin)) == NULL)
                // {
                //     return LOW;
                // }
                // return node->digitalRead(node, pin);
                return LOW<int>();
            }
        }

        inline const std::vector<int> &pinToGpio() const
        {
            return pinToGpio_;
        }

        inline const std::vector<int> &physToGpio() const
        {
            return physToGpio_;
        }

        inline const std::vector<int> &sysFds() const
        {
            return sysFds_;
        }

        inline const std::vector<int> &gpioToGPLEV() const
        {
            return gpioToGPLEV_;
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

        const int *gpioPtr() const
        {
            return gpio;
        }

        void addDevice(const size_t pinBase, const size_t numPins)
        {
            // Firstly resize to accomodate the total pin count
            deviceRegistry_.resize(pinBase + numPins);
            for (size_t i = pinBase; i < pinBase + numPins; i++)
            {
                deviceRegistry_[i] = deviceRegistry_[i] + 1;
            }
        }

        void printDeviceRegistry() const
        {
            std::cout << "========================" << std::endl;
            std::cout << "WiringPi device registry" << std::endl;
            std::cout << "========================" << std::endl;
            std::cout << "Total registered pins: " << deviceRegistry_.size() << std::endl;

            // Validate the device registry
            if (validateDeviceRegistry())
            {
                std::cout << "Device registry valid" << std::endl;
            }
            else
            {
                std::cout << "Device registry invalid" << std::endl;
            }

            // Print the device registry pins:
            std::cout << "Device pin layout:" << std::endl;
            for (size_t i = 0; i < deviceRegistry_.size(); i++)
            {
                std::cout << deviceRegistry_[i] << " ";
            }
            std::cout << std::endl;
        }

        inline bool validateDeviceRegistry() const
        {
            for (size_t i = 0; i < deviceRegistry_.size(); i++)
            {
                if (deviceRegistry_[i] > 1)
                {
                    return false;
                }
            }
            return true;
        }

    private:
        // Basic info
        static constexpr int piModel_ = PI_MODEL<int>();
        static constexpr int gpioLayout_ = GPIO_LAYOUT<int>();
        static constexpr int piRev_ = PI_REV<int>();
        static constexpr int piMem_ = PI_MEM<int>();
        static constexpr int piMaker_ = PI_MAKER<int>();
        static constexpr int piWarranty_ = PI_WARRANTY<int>();

        // Registry of devices
        std::vector<int> deviceRegistry_;

        // Setup info
        static constexpr int wiringPiSetuped_ = static_cast<int>(true);
        const int wiringPiDebug_;
        const int wiringPiReturnCodes_;
        static constexpr int wiringPiMode_ = getMode<int>(wpiMode, piModel_);

        int checkDevMem()
        {
            int fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC);
            fd = open("/dev/gpiomem", O_RDWR | O_SYNC | O_CLOEXEC);
            return fd;
        }

        const std::vector<int> pinToGpio_;
        const std::vector<int> physToGpio_;
        const std::vector<int> sysFds_;
        const std::vector<int> gpioToGPLEV_ = std::vector<int>{
            13, 13, 13, 13, 13, 13, 13, 13, //
            13, 13, 13, 13, 13, 13, 13, 13, //
            13, 13, 13, 13, 13, 13, 13, 13, //
            13, 13, 13, 13, 13, 13, 13, 13, //
            14, 14, 14, 14, 14, 14, 14, 14, //
            14, 14, 14, 14, 14, 14, 14, 14, //
            14, 14, 14, 14, 14, 14, 14, 14, //
            14, 14, 14, 14, 14, 14, 14, 14};

        const int piGpioBase_;
        static constexpr int piGpioPupOffset_ = piGpioPupOffset<int>(piModel_);
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
            case PI_MODEL_A<int>():
            case PI_MODEL_B<int>():
            case PI_MODEL_AP<int>():
            case PI_MODEL_BP<int>():
            case PI_ALPHA<int>():
            case PI_MODEL_CM<int>():
            case PI_MODEL_ZERO<int>():
            case PI_MODEL_ZERO_W<int>():
                i = GPIO_PERI_BASE_OLD<int>();
                break;
            case PI_MODEL_4B<int>():
            case PI_MODEL_400<int>():
            case PI_MODEL_CM4<int>():
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

        const std::vector<int> physToGpioI(const int layout)
        {
            if (layout == 1) // A, B, Rev 1, 1.1
            {
                return std::vector<int>{
                    -1, -1, -1, 0, -1, 1, -1, 4,    //
                    14, -1, 15, 17, 18, 21, -1, 22, //
                    23, -1, 24, 10, -1, 9, 25, 11,  //
                    8, -1, 7, -1, -1, -1, -1, -1,   //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1};
            }
            else // A2, B2, A+, B+, CM, Pi2, Pi3, Zero, Zero W, Zero 2 W
            {
                return std::vector<int>{
                    -1, -1, -1, 2, -1, 3, -1, 4,    //
                    14, -1, 15, 17, 18, 27, -1, 22, //
                    23, -1, 24, 10, -1, 9, 25, 11,  //
                    8, -1, 7, 0, 1, 5, -1, 6,       //
                    12, 13, -1, 19, 16, 26, 20, -1, //
                    21, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, 28, 29, 30, 31, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1};
            }
        }

        const std::vector<int> pinToGpioI(const int layout)
        {
            if (layout == 1) // A, B, Rev 1, 1.1
            {
                return std::vector<int>{
                    17, 18, 21, 22, 23, 24, 25, 4,  //
                    0, 1, 8, 7, 10, 9, 11, 14,      //
                    15, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1};
            }
            else // A2, B2, A+, B+, CM, Pi2, Pi3, Zero, Zero W, Zero 2 W
            {
                return std::vector<int>{
                    17, 18, 27, 22, 23, 24, 25, 4,  //
                    2, 3, 8, 7, 10, 9, 11, 14,      //
                    15, 28, 29, 30, 31, 5, 6, 13,   //
                    19, 26, 12, 16, 20, 21, 0, 1,   //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1};
            }
        }

        const std::vector<int> sysFdsI(const int mode)
        {
            if (mode == WPI_MODE_GPIO_SYS<int>())
            {
                char fName[128];
                std::vector<int> vec = std::vector<int>{
                    -1, -1, -1, -1, -1, -1, -1, -1,   //
                    -1, -1, -1, -1, -1, -1, -1, -1,   //
                    -1, -1, -1, -1, -1, -1, -1, -1,   //
                    -1, -1, -1, -1, -1, -1, -1, -1,   //
                    -1, -1, -1, -1, -100, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1,   //
                    -1, -1, -1, -1, -1, -1, -1, -1,   //
                    -1, -1, -1, -1, -1, -1, -1, -1};
                for (size_t pin = 0; pin < 64; ++pin)
                {
                    // std::cout << "Pin " << pin << std::endl;
                    sprintf(fName, "/sys/class/gpio/gpio%d/value", pin);
                    vec[pin] = open(fName, O_RDWR);
                }
                return vec;
            }
            else
            {
                return std::vector<int>{
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1, //
                    -1, -1, -1, -1, -1, -1, -1, -1};
            }
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

        const int *mapTimer(const int offset)
        {
            int *ptr = static_cast<int *>(mmap(0, BLOCK_SIZE<size_t>(), PROT_READ | PROT_WRITE, MAP_SHARED, checkDevMem(), static_cast<__off_t>(offset)));
            *(ptr + TIMER_CONTROL<int>()) = 0x0000280;
            *(ptr + TIMER_PRE_DIV<int>()) = 0x00000F9;
            return ptr;
        }

        const int *gpio;
        const int *pwm;
        const int *clk;
        const int *pads;
        const int *timer;
        const int *timerIrqRaw;

        // Base addresses
        const int *_wiringPiGpio;
        const int *_wiringPiPwm;
        const int *_wiringPiClk;
        const int *_wiringPiPads;
        const int *_wiringPiTimer;
        const int *_wiringPiTimerIrqRaw;

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
        static constexpr int versionMajor_ = VERSION_MAJOR<int>();
        static constexpr int versionMinor_ = VERSION_MINOR<int>();
    };
}

#endif