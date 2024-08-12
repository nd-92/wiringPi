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

    memorySpec readDeviceMemory()
    {
        int fd_;
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

    template <const int model>
    class piMemory
    {
    public:
        [[nodiscard]] inline piMemory()
            : devMem_(readDeviceMemory()),
              piGpioBase_(piGpioBase(model, devMem_.usingGpioMem)),
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
              timerIrqRaw_(timer_ + TIMER_IRQ_RAW<timer_t>())
        {
            // Check that the maps succeeded
            assert((gpio_ != MAP_FAILED));
            assert((pwm_ != MAP_FAILED));
            assert((clk_ != MAP_FAILED));
            assert((pads_ != MAP_FAILED));
            assert((timer_ != MAP_FAILED));
        };

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

        [[nodiscard]] inline constexpr volatile gpio_t *gpioPtr() const volatile
        {
            return gpio_;
        }

        [[nodiscard]] inline constexpr volatile pwm_t *pwmPtr() const volatile
        {
            return pwm_;
        }

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
            return static_cast<volatile T0 *>(mmap(0, BLOCK_SIZE<size_t>(), PROT_READ | PROT_WRITE, MAP_SHARED, devMem_.fd, static_cast<__off_t>(offset)));
        }

        // Map timer
        [[nodiscard]] volatile timer_t *mapTimer(const gpio_t offset) const
        {
            volatile timer_t *volatile ptr = static_cast<volatile timer_t *>(mmap(0, BLOCK_SIZE<size_t>(), PROT_READ | PROT_WRITE, MAP_SHARED, devMem_.fd, static_cast<__off_t>(offset)));
            *(ptr + TIMER_CONTROL<timer_t>()) = 0x0000280;
            *(ptr + TIMER_PRE_DIV<timer_t>()) = 0x00000F9;
            return ptr;
        }

        // Hardware pointers
        volatile gpio_t *volatile gpio_;
        volatile pwm_t *volatile pwm_;
        volatile clk_t *volatile clk_;
        volatile pads_t *volatile pads_;
        volatile timer_t *volatile timer_;
        volatile timer_t *volatile timerIrqRaw_;
    };
}

#endif