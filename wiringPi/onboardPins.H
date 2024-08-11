#ifndef __WIRING_PI_onboardPins_H
#define __WIRING_PI_onboardPins_H

#include "wiringPi.H"

// GPIO device occupying 1 pin
// Default constructor allows setting of mode
// Only acceptable modes are INPUT/OUTPUT for now

namespace WiringPi
{
    [[nodiscard]] inline consteval pin_t digitalReadModulo(const pin_t pinNumber)
    {
        return (1 << (pinNumber & 31));
    }

    [[nodiscard]] inline consteval gpio_t gpioToPUDCLK(const gpio_t i)
    {
        return 38 + (i / 32);
    }

    // Offset to the GPIO Input level registers for each GPIO pin
    // This is equivalent to the gpioToGPLEV table
    // Returns 13 on pins 0 -> 31, 14 on pins 32 -> 63
    [[nodiscard]] inline consteval gpio_t gpioToGPLEV(const pin_t i)
    {
        return 13 + (i / 32);
    }

    // Offset to the GPIO Clear and Set registers for each GPIO pin
    // This is equivalent to the gpioToGPCLR and gpioToGPSET tables
    // Value passed to this function should be deviceNo + (64 * !value)
    // Returns 10 -> 11 for digital value LOW, 7 -> 8 for digital value HIGH
    [[nodiscard]] inline consteval gpio_t gpioCLRSET(const pin_t i)
    {
        return 7 + (i / 64) + (i / 32);
    }

    class onboardPins
    {
    public:
        [[nodiscard]] inline constexpr onboardPins() {};

        // Read the value of a given pin, returns either HIGH or LOW
        template <const pin_t pinNumber>
        [[nodiscard]] inline gpio_t digitalRead(const volatile gpio_t *gpioPtr) const volatile
        {
            return static_cast<gpio_t>((*(gpioPtr + gpioToGPLEV(pinNumber)) & digitalReadModulo(pinNumber)) != 0);
        }

        // Set an output bit on a given pin
        template <const pin_t pinNumber>
        inline void digitalWrite(volatile gpio_t *gpioPtr, const gpio_t value) const volatile
        {
            *(gpioPtr + gpioCLRSET(pinNumber + (64 * !value))) = digitalReadModulo(pinNumber);
        }

        // Sets the mode of a pin to be input, output or PWM output
        inline void pinMode(volatile gpio_t *gpioPtr, const pin_t pinNumber, const gpio_t mode, const pin_t fSel, const pin_t shift)
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

        // Returns the name of the device
        [[nodiscard]] inline consteval std::string_view deviceName() const
        {
            return "Onboard pins";
        }

        // Control the internal pull-up/down resistors on a GPIO pin
        void pullUpDnControl(volatile gpio_t *gpioPtr, const pin_t pinNumber, const gpio_t pud) const volatile
        {
            // Pi 4B pull up/down method
            pin_t pullBits = *(gpioPtr + pullReg_[pinNumber]);
            pullBits &= static_cast<gpio_t>(~(3 << pullShift_[pinNumber]));
            pullBits |= (pud << pullShift_[pinNumber]);
            std::cout << "pullBits = " << pullBits << std::endl;
            *(gpioPtr + pullReg_[pinNumber]) = pullBits;
        }

    private:
        gpioTable pinMode_ = pinModeI();
        static constexpr const gpioTable pullReg_ = pullRegI();
        static constexpr const gpioTable pullShift_ = pullShiftI();
    };
}
#endif