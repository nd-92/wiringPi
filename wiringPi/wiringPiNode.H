// ======================================================================== //
//                                                                          //
// wiringPiNode.H                                                           //
//                                                                          //
// ======================================================================== //
// Designed to replace wiringPiNodeStruct                                   //
// Virtual modes overridden by onboard pins or devices                      //
// ======================================================================== //

#ifndef __WIRING_PI_wiringPiNode_H
#define __WIRING_PI_wiringPiNode_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"
#include "wiringPi.H"

namespace WiringPi
{
    class wiringPiNode
    {
    public:
        [[nodiscard]] inline constexpr wiringPiNode() {};

        // Read the value of a given pin, returns either HIGH or LOW
        [[nodiscard]] virtual inline gpio_t digitalRead([[maybe_unused]] const volatile gpio_t *gpioPtr) const
        {
            return 0;
        }

        // Set an output bit
        virtual inline void digitalWrite([[maybe_unused]] volatile gpio_t *gpioPtr, [[maybe_unused]] const gpio_t value) const
        {
            return;
        }

        [[nodiscard]] virtual inline gpio_t analogRead() const
        {
            return 0;
        }

        virtual inline void analogWrite() const
        {
            return;
        }

        // Sets the mode of a pin to be input, output or PWM output
        virtual inline void pinMode([[maybe_unused]] volatile gpio_t *gpioPtr, [[maybe_unused]] const pinModes::type mode, [[maybe_unused]] const pin_t fSel, [[maybe_unused]] const pin_t shift) const
        {
            return;
        }

        // Control the internal pull-up/down resistors on a GPIO pin
        virtual inline void pullUpDnControl([[maybe_unused]] const volatile gpio_t *gpioPtr, [[maybe_unused]] const gpio_t value) const
        {
            return;
        }

        // Set an output PWM value
        virtual inline void pwmWrite([[maybe_unused]] volatile gpio_t *pwmPtr, [[maybe_unused]] const gpio_t value) const
        {
            return;
        }

        // Return the device name
        [[nodiscard]] virtual inline consteval name_t deviceName() const
        {
            return "wiringPiNode";
        }

        // Return access to data
        [[nodiscard]] virtual inline constexpr unsigned int data(const std::size_t i) const
        {
            return data_[i];
        }

    private:
        static constexpr const std::array<unsigned int, 4> data_{0, 0, 0, 0};
    };
}

#endif
