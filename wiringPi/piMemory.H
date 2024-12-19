// ======================================================================== //
//                                                                          //
// piMemory.H                                                               //
//                                                                          //
// ======================================================================== //
// Memory management class for the Raspberry Pi                             //
// Maps devices into program memory                                         //
// Provides access to device memory through gpioPtr() and pwmPtr() methods  //
// ======================================================================== //

#ifndef __WIRING_PI_piMemory_H
#define __WIRING_PI_piMemory_H

#include "wiringPi.H"

namespace WiringPi
{
    struct memorySpec
    {
        const int fd;
        const bool usingGpioMem;
    };

    template <const int model>
    class piMemory
    {
    public:
        [[nodiscard]] piMemory()
            : devMem_(readDeviceMemory()),
              piGpioBase_(piGpioBase<pin_t>(devMem_.usingGpioMem)),
              GPIO_BASE_(piGpioBase_ + 0x00200000),
              GPIO_PWM_(piGpioBase_ + 0x0020C000),
              GPIO_CLOCK_BASE_(piGpioBase_ + 0x00101000),
              GPIO_PADS_(piGpioBase_ + 0x00100000),
              GPIO_TIMER_(piGpioBase_ + 0x0000B000),
              gpio_(mapHardware<gpio_t>(GPIO_BASE_)),
              pwm_(mapHardware<pwm_t>(GPIO_PWM_)),
              clk_(mapHardware<clk_t>(GPIO_CLOCK_BASE_)),
              pads_(mapHardware<pads_t>(GPIO_PADS_)),
              timer_(mapTimer(GPIO_TIMER_)),
              timerIrqRaw_(timer_ + timer::irqRaw())
        {
            // Check that the maps succeeded
            assert((gpio_ != MAP_FAILED));
            assert((pwm_ != MAP_FAILED));
            assert((clk_ != MAP_FAILED));
            assert((pads_ != MAP_FAILED));
            assert((timer_ != MAP_FAILED));
        };

        // Return the base address of the GPIO memory mapped hardware IO
        [[nodiscard]] inline constexpr pin_t gpioBase() const
        {
            return piGpioBase_;
        }

        [[nodiscard]] inline constexpr pin_t GPIO_PADS() const
        {
            return GPIO_PADS_;
        }

        [[nodiscard]] inline constexpr pin_t GPIO_CLOCK_BASE() const
        {
            return GPIO_CLOCK_BASE_;
        }

        [[nodiscard]] inline constexpr pin_t GPIO_BASE() const
        {
            return GPIO_BASE_;
        }

        [[nodiscard]] inline constexpr pin_t GPIO_TIMER() const
        {
            return GPIO_TIMER_;
        }

        [[nodiscard]] inline constexpr pin_t GPIO_PWM() const
        {
            return GPIO_PWM_;
        }

        // Pointer to the GPIO hardware
        [[nodiscard]] inline constexpr volatile gpio_t *gpioPtr() const volatile
        {
            return gpio_;
        }

        // Pointer to the PWM hardware
        [[nodiscard]] inline constexpr volatile pwm_t *pwmPtr() const volatile
        {
            return pwm_;
        }

        // Pointer to the clock hardware
        [[nodiscard]] inline constexpr volatile pwm_t *clkPtr() const volatile
        {
            return clk_;
        }

        // Pointer to the pads hardware
        [[nodiscard]] inline constexpr volatile pwm_t *padsPtr() const volatile
        {
            return pads_;
        }

        // Returns 1 if wiringPi is using GPIO memory
        // Returns 0 if wiringPi is not using GPIO memory
        [[nodiscard]] inline constexpr bool usingGpioMemory() const
        {
            return devMem_.usingGpioMem;
        }

    private:
        // Check access to device memory
        const struct memorySpec devMem_;

        // Pi memory addresses
        const pin_t piGpioBase_;
        const pin_t GPIO_BASE_;
        const pin_t GPIO_PWM_;
        const pin_t GPIO_CLOCK_BASE_;
        const pin_t GPIO_PADS_;
        const pin_t GPIO_TIMER_;

        // Map individual hardware components
        template <typename T0, typename T1>
        [[nodiscard]] volatile T0 *mapHardware(const T1 offset) const
        {
            return static_cast<volatile T0 *>(mmap(0, memory::blockSize<size_t>(), PROT_READ | PROT_WRITE, MAP_SHARED, devMem_.fd, static_cast<__off_t>(offset)));
        }

        // Map timer
        [[nodiscard]] volatile timer_t *mapTimer(const gpio_t offset) const
        {
            volatile timer_t *volatile ptr = static_cast<volatile timer_t *>(mmap(0, memory::blockSize<size_t>(), PROT_READ | PROT_WRITE, MAP_SHARED, devMem_.fd, static_cast<__off_t>(offset)));
            *(ptr + timer::control()) = 0x0000280;
            *(ptr + timer::preDiv()) = 0x00000F9;
            return ptr;
        }

        // Read the device memory
        [[nodiscard]] inline memorySpec readDeviceMemory() const
        {
            int fd_ = -1;
            bool usingGpioMem_ = false;

            constexpr const char *gpiomemGlobal = "/dev/mem";
            constexpr const char *gpiomemModule = "/dev/gpiomem";

            if (gpiomemGlobal == NULL || (fd_ = open(gpiomemGlobal, O_RDWR | O_SYNC | O_CLOEXEC)) < 0)
            {
                if (gpiomemModule && (fd_ = open(gpiomemModule, O_RDWR | O_SYNC | O_CLOEXEC)) >= 0) // We're using gpiomem
                {
                    usingGpioMem_ = true;
                }
            }

            return memorySpec{fd_, usingGpioMem_};
        }

        // Hardware pointers
        volatile gpio_t *volatile gpio_;
        volatile pwm_t *volatile pwm_;
        volatile clk_t *volatile clk_;
        volatile pads_t *volatile pads_;
        volatile timer_t *volatile timer_;
        volatile timer_t *volatile timerIrqRaw_;

        // Initialise the base address of the GPIO memory mapped hardware IO
        template <typename T>
        [[nodiscard]] inline constexpr T piGpioBase(const bool usingGpioMem) const
        {
            // Check for a valid model
            static_assert(piModelCheck(model), "Invalid Raspberry Pi model");

            if (usingGpioMem)
            {
                return 0;
            }
            // Initialise piGpioBase and piGpioPupOffset from board model - default vals 1 and 0
            switch (model)
            {
            case Pi::models::PiA():
            {
                return 0;
            }
            case Pi::models::PiB():
            {
                return 0;
            }
            case Pi::models::PiAP():
            {
                return 0;
            }
            case Pi::models::PiBP():
            {
                return 0;
            }
            case Pi::models::PiAlpha():
            {
                return 0;
            }
            case Pi::models::PiCM():
            {
                return 0;
            }
            case Pi::models::PiZero():
            {
                return 0;
            }
            case Pi::models::PiZero_W():
            {
                return GPIO::PERI::BASE_OLD<T>();
            }
            case Pi::models::Pi4B():
            {
                return 0;
            }
            case Pi::models::Pi400():
            {
                return 0;
            }
            case Pi::models::PiCM4():
            {
                return GPIO::PERI::BASE_2711<T>();
            }
            default:
            {
                return GPIO::PERI::BASE_2835<T>();
            }
            }
        }

        // // Initialise piGpioBase and piGpioPupOffset from board model - default vals 1 and 0
        // template <typename T>
        // [[nodiscard]] inline consteval T piGpioPupOffset()
        // {
        //     static_assert(piModelCheck(model), "Invalid Raspberry Pi model");

        //     switch (model)
        //     {
        //     case Pi::models::PiA():
        //     {
        //         return 0;
        //     }
        //     case Pi::models::PiB():
        //     {
        //         return 0;
        //     }
        //     case Pi::models::PiAP():
        //     {
        //         return 0;
        //     }
        //     case Pi::models::PiBP():
        //     {
        //         return 0;
        //     }
        //     case Pi::models::PiAlpha():
        //     {
        //         return 0;
        //     }
        //     case Pi::models::PiCM():
        //     {
        //         return 0;
        //     }
        //     case Pi::models::PiZero():
        //     {
        //         return 0;
        //     }
        //     case Pi::models::PiZero_W():
        //     {
        //         return GPPUD<T>();
        //     }
        //     case Pi::models::Pi4B():
        //     {
        //         return 0;
        //     }
        //     case Pi::models::Pi400():
        //     {
        //         return 0;
        //     }
        //     case Pi::models::PiCM4():
        //     {
        //         return GPPUPPDN0<T>();
        //     }
        //     default:
        //     {
        //         return GPPUD<T>();
        //     }
        //     }
        // }
    };
}

#endif
