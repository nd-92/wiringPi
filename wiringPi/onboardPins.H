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

        inline void digitalWrite(volatile gpio_t *gpioPtr, const pin_t pinNumber, const gpio_t value) const volatile
        {
            *(gpioPtr + gpioCLRSET(pinNumber + (64 * !value))) = 1 << (pinNumber & 31);
        }

        [[nodiscard]] inline consteval std::string_view deviceName() const
        {
            return "Onboard pins";
        }

    private:
        static constexpr const std::array<pin_t, 40> pinMode_ = pinModeI();
        static constexpr const std::array<gpio_t, 40> pullReg_ = pullRegI();
        static constexpr const std::array<gpio_t, 40> pullShift_ = pullShiftI();
    };
}
#endif