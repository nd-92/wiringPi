#ifndef __WIRING_PI_onboardPins_H
#define __WIRING_PI_onboardPins_H

#include "wiringPi.H"

// GPIO device occupying 1 pin
// Default constructor allows setting of mode
// Only acceptable modes are INPUT/OUTPUT for now

namespace WiringPi
{
    class onboardPins
    {
    public:
        [[nodiscard]] inline constexpr onboardPins() {};

        // Offset to the GPIO Input level registers for each GPIO pin
        // This is equivalent to the gpioToGPLEV table
        // Returns 13 on pins 0 -> 31, 14 on pins 32 -> 63
        [[nodiscard]] inline constexpr gpio_t gpioToGPLEV(const pin_t i) const volatile
        {
            return 13 + (i / 32);
        }

        // Offset to the GPIO Clear and Set registers for each GPIO pin
        // This is equivalent to the gpioToGPCLR and gpioToGPSET tables
        // Value passed to this function should be deviceNo + (64 * !value)
        // Returns 10 -> 11 for digital value LOW, 7 -> 8 for digital value HIGH
        [[nodiscard]] inline constexpr gpio_t gpioCLRSET(const pin_t i) const volatile
        {
            return 7 + (i / 64) + (i / 32);
        }

        // Read the value of a given pin, returns either HIGH or LOW
        [[nodiscard]] inline gpio_t digitalRead(const volatile gpio_t *gpioPtr, const pin_t pinNumber) const volatile
        {
            if ((*(gpioPtr + gpioToGPLEV(pinNumber)) & (1 << (pinNumber & 31))) != 0)
            {
                return HIGH<gpio_t>();
            }
            else
            {
                return LOW<gpio_t>();
            }
        }

        // Set an output bit on a given pin
        inline void digitalWrite(volatile gpio_t *gpioPtr, const pin_t pinNumber, const gpio_t value) const volatile
        {
            *(gpioPtr + gpioCLRSET(pinNumber + (64 * !value))) = 1 << (pinNumber & 31);
        }

        // Returns the name of the device
        [[nodiscard]] inline consteval std::string_view deviceName() const
        {
            return "Onboard pins";
        }

        // Sets the mode of a pin to be input, output or PWM output
        inline void pinMode(
            volatile gpio_t *gpioPtr,
            const pin_t pinNumber,
            const gpio_t mode,
            const pin_t fSel,
            const pin_t shift)
        {
            if (mode == INPUT<gpio_t>() || mode == PWM_OFF<gpio_t>())
            {
                *(gpioPtr + fSel) = (*(gpioPtr + fSel) & static_cast<gpio_t>(~(7 << shift)));
            }
            else if (mode == OUTPUT<gpio_t>())
            {
                *(gpioPtr + fSel) = (*(gpioPtr + fSel) & static_cast<gpio_t>(~(7 << shift))) | (1 << shift);
            }
            pinMode_[pinNumber] = mode;
        }

        // Returns the map of pin modes
        [[nodiscard]] inline constexpr const pinTable &pinModeMap() const
        {
            return pinMode_;
        }

    private:
        gpioTable pinMode_ = pinModeI();
        static constexpr const gpioTable pullReg_ = pullRegI();
        static constexpr const gpioTable pullShift_ = pullShiftI();
    };
}
#endif