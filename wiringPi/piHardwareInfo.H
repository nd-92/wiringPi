#ifndef __WIRING_PI_piHardwareInfo_H
#define __WIRING_PI_piHardwareInfo_H

#include "wiringPi.H"

namespace WiringPi
{
    class piHardware
    {
    public:
        [[nodiscard]] inline constexpr piHardware() {};

        [[nodiscard]] inline consteval int model() const
        {
            return piModel_;
        }

        [[nodiscard]] inline consteval int gpioLayout() const
        {
            return gpioLayout_;
        }

        [[nodiscard]] inline consteval int revision() const
        {
            return piRev_;
        }

        [[nodiscard]] inline consteval int memory() const
        {
            return piMem_;
        }

        [[nodiscard]] inline consteval int maker() const
        {
            return piMaker_;
        }

        [[nodiscard]] inline consteval int warranty() const
        {
            return piWarranty_;
        }

        [[nodiscard]] inline consteval pin_t gpioBase() const
        {
            return piGpioBase_;
        }

        [[nodiscard]] inline consteval pin_t gpioPupOffset() const
        {
            return piGpioPupOffset_;
        }

        [[nodiscard]] inline consteval bool usingGpioMem() const
        {
            return usingGpioMem_;
        }

        [[nodiscard]] inline consteval pin_t GPIO_PADS() const
        {
            return GPIO_PADS_;
        }

        [[nodiscard]] inline consteval pin_t GPIO_CLOCK_BASE() const
        {
            return GPIO_CLOCK_BASE_;
        }

        [[nodiscard]] inline consteval pin_t GPIO_BASE() const
        {
            return GPIO_BASE_;
        }

        [[nodiscard]] inline consteval pin_t GPIO_TIMER() const
        {
            return GPIO_TIMER_;
        }

        [[nodiscard]] inline consteval pin_t GPIO_PWM() const
        {
            return GPIO_PWM_;
        }

        [[nodiscard]] inline consteval std::string_view modelString() const
        {
            return modelString_;
        }

        // Check if pin is onboard
        // Returns true only if less than 32
        [[nodiscard]] inline consteval bool isOnboard(const pin_t pinNumber) const
        {
            return pinNumber < numPhysicalPins_;
        }

        [[nodiscard]] inline consteval pin_t numPins() const
        {
            return numPhysicalPins_;
        }

    private:
        // Pi hardware info
        static constexpr const int piModel_ = PI_MODEL<int>();
        static constexpr const int gpioLayout_ = GPIO_LAYOUT<int>();
        static constexpr const int piRev_ = PI_REV<int>();
        static constexpr const int piMem_ = PI_MEM<int>();
        static constexpr const int piMaker_ = PI_MAKER<int>();
        static constexpr const int piWarranty_ = PI_WARRANTY<int>();
        static constexpr const pin_t piGpioBase_ = piGpioBase(piModel_);
        static constexpr const pin_t piGpioPupOffset_ = piGpioPupOffset<int>(piModel_);
        static constexpr const bool usingGpioMem_ = USING_GPIOMEM<bool>();
        static constexpr const pin_t GPIO_PADS_ = piGpioBase_ + 0x00100000;
        static constexpr const pin_t GPIO_CLOCK_BASE_ = piGpioBase_ + 0x00101000;
        static constexpr const pin_t GPIO_BASE_ = piGpioBase_ + 0x00200000;
        static constexpr const pin_t GPIO_TIMER_ = piGpioBase_ + 0x0000B000;
        static constexpr const pin_t GPIO_PWM_ = piGpioBase_ + 0x0020C000;
        static constexpr const std::string_view modelString_ = piModelStrings()[PI_MODEL<size_t>()];
        static constexpr const pin_t numPhysicalPins_ = numPhysicalPins<GPIO_LAYOUT<int>()>();
    };
}

#endif