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

    template <const Pi::model::type Model>
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
              timerIrqRaw_(timer_ + timer::irqRaw()) {};

        ~piMemory()
        {
            // Unmap all of the hardware components
            unmapHardware(gpio_);
            unmapHardware(pwm_);
            unmapHardware(clk_);
            unmapHardware(pads_);
            unmapHardware(timer_);
        }

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
        [[nodiscard]] T0 *mapHardware(const T1 offset) const
        {
            // Perform mmap
            T0 *ptr = static_cast<T0 *>(mmap(0, memory::blockSize<size_t>(), PROT_READ | PROT_WRITE, MAP_SHARED, devMem_.fd, static_cast<__off_t>(offset)));

            // Check that it didn't fail
            fatalAssert(ptr != MAP_FAILED, "Hardware pointer map failed");

            // Return the pointer
            return ptr;
        }

        // Map timer
        [[nodiscard]] timer_t *mapTimer(const gpio_t offset) const
        {
            timer_t *ptr = static_cast<timer_t *>(mmap(0, memory::blockSize<size_t>(), PROT_READ | PROT_WRITE, MAP_SHARED, devMem_.fd, static_cast<__off_t>(offset)));

            // Check that it didn't fail
            fatalAssert(ptr != MAP_FAILED, "Timer pointer map failed");

            *(ptr + timer::control()) = 0x0000280;
            *(ptr + timer::preDiv()) = 0x00000F9;

            return ptr;
        }

        // Unmap individual hardware components
        template <typename T>
        void unmapHardware(T *ptr) const
        {
            // I really don't like the use of the C-style cast to get rid of volatility
            assert((munmap((void *)ptr, memory::blockSize<size_t>()) != -1));
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
        volatile gpio_t *gpio_;
        volatile pwm_t *pwm_;
        volatile clk_t *clk_;
        volatile pads_t *pads_;
        volatile timer_t *timer_;
        volatile timer_t *timerIrqRaw_;

        // Initialise the base address of the GPIO memory mapped hardware IO
        template <typename T>
        [[nodiscard]] inline constexpr T piGpioBase(const bool usingGpioMem) const
        {
            // Check for a valid model
            static_assert(piModelCheck(Model), "Invalid Raspberry Pi model");

            if (usingGpioMem)
            {
                return 0;
            }

            switch (Model)
            {
            case Pi::model::PiA:
            {
                return 0;
            }
            case Pi::model::PiB:
            {
                return 0;
            }
            case Pi::model::PiAP:
            {
                return 0;
            }
            case Pi::model::PiBP:
            {
                return 0;
            }
            case Pi::model::PiAlpha:
            {
                return 0;
            }
            case Pi::model::PiCM:
            {
                return 0;
            }
            case Pi::model::PiZero:
            {
                return 0;
            }
            case Pi::model::PiZero_W:
            {
                return GPIO::PERI::BASE_OLD<T>();
            }
            case Pi::model::Pi4B:
            {
                return 0;
            }
            case Pi::model::Pi400:
            {
                return 0;
            }
            case Pi::model::PiCM4:
            {
                return GPIO::PERI::BASE_2711<T>();
            }
            default:
            {
                return GPIO::PERI::BASE_2835<T>();
            }
            }
        }
    };
}

#endif
