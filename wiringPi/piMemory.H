#ifndef __WIRING_PI_piMemory_H
#define __WIRING_PI_piMemory_H

#include "wiringPi.H"

namespace WiringPi
{
    template <const int model>
    class piMemory
    {
    public:
        [[nodiscard]] inline piMemory(const bool usingGpioMem)
            : piGpioBase_(piGpioBase(model, usingGpioMem)) {};

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

    private:
        // Pi memory addresses
        const pin_t piGpioBase_;
        const pin_t GPIO_PADS_ = piGpioBase_ + 0x00100000;
        const pin_t GPIO_CLOCK_BASE_ = piGpioBase_ + 0x00101000;
        const pin_t GPIO_BASE_ = piGpioBase_ + 0x00200000;
        const pin_t GPIO_TIMER_ = piGpioBase_ + 0x0000B000;
        const pin_t GPIO_PWM_ = piGpioBase_ + 0x0020C000;
    };
}

#endif