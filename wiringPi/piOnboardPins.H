// ======================================================================== //
//                                                                          //
// piOnboardPins.H                                                          //
//                                                                          //
// ======================================================================== //
// Oboard pin class for the Raspberry Pi                                    //
// Handles reading/writing to/from onboard pins                             //
// Accesses hardware through read/write methods                             //
// ======================================================================== //

#ifndef __WIRING_PI_piOnboardPins_H
#define __WIRING_PI_piOnboardPins_H

#include "wiringPi.H"

namespace WiringPi
{
    [[nodiscard]] inline consteval pin_t digitalReadModulo(const pin_t pinNumber)
    {
        return (1 << (pinNumber & 31));
    }

    // Offset to the Pull Up Down Clock regsiter
    // This is equivalent to the gpioToPUDCLK table
    // Returns 38 on pins 0 -> 31, 39 on pins 32 -> 63
    [[nodiscard]] inline consteval gpio_t gpioToPUDCLK(const gpio_t i)
    {
        return 38 + (i / 32);
    }

    // Offset to the GPIO Input level registers for each GPIO pin
    // This is equivalent to the gpioToGPLEV table
    // Returns 13 on pins 0 -> 31, 14 on pins 32 -> 63
    [[nodiscard]] inline consteval gpio_t gpioToGPLEV(const pin_t i)
    {
        return (13 + (i / 32));
    }

    // Offset to the GPIO Clear and Set registers for each GPIO pin
    // This is equivalent to the gpioToGPCLR and gpioToGPSET tables
    // Value passed to this function should be deviceNo + (64 * !value)
    // Returns 10 -> 11 for digital value LOW, 7 -> 8 for digital value HIGH
    [[nodiscard]] inline consteval gpio_t gpioCLRSET(const pin_t i)
    {
        return (7 + (i / 64) + (i / 32));
    }

    [[nodiscard]] inline consteval gpio_t pullReg(const pin_t i)
    {
        return (GPPUPPDN0<gpio_t>() + (i >> 4));
    }

    [[nodiscard]] inline consteval gpio_t pullShift(const pin_t i)
    {
        return ((i & 0xf) << 1);
    }

    class onboardPins : public wiringPiNode
    {
    public:
        [[nodiscard]] inline constexpr onboardPins() {};

        // Read the value of a given pin, returns either HIGH or LOW
        template <const pin_t pinNumber>
        [[nodiscard]] inline gpio_t digitalRead(const volatile gpio_t *gpioPtr) const volatile
        {
            static_assert(pinNumber != nullPin<pin_t>());
            return static_cast<gpio_t>((*(gpioPtr + gpioToGPLEV(pinNumber)) & digitalReadModulo(pinNumber)) != 0);
        }

        // Set an output bit on a given pin
        template <const pin_t pinNumber>
        inline void digitalWrite(volatile gpio_t *gpioPtr, const gpio_t value) const volatile
        {
            static_assert(pinNumber != nullPin<pin_t>());
            *(gpioPtr + gpioCLRSET(pinNumber + (64 * !value))) = digitalReadModulo(pinNumber);
        }

        // Sets the mode of a pin to be input, output or PWM output
        template <const pin_t pinNumber>
        inline void pinMode(volatile gpio_t *gpioPtr, const gpio_t mode, const pin_t fSel, const pin_t shift)
        {
            static_assert(pinNumber != nullPin<pin_t>());
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

        // Control the internal pull-up/down resistors on a GPIO pin
        template <const pin_t pinNumber, const gpio_t offset>
        inline void pullUpDnControl(volatile gpio_t *gpioPtr, const gpio_t pud) const volatile
        {
            static_assert(pinNumber != nullPin<pin_t>());
            if constexpr (offset == GPPUPPDN0<gpio_t>())
            {
                // Pi 4B pull up/down method
                pin_t pullBits = *(gpioPtr + pullReg(pinNumber));
                pullBits &= static_cast<gpio_t>(~(3 << pullShift(pinNumber)));
                pullBits |= (pud << pullShift(pinNumber));
                *(gpioPtr + pullReg(pinNumber)) = pullBits;
            }
            else
            {
                *(gpioPtr + GPPUD<gpio_t>()) = pud & 3;
                delayMicroseconds(5);
                *(gpioPtr + gpioToPUDCLK(pinNumber)) = digitalReadModulo(pinNumber);
                delayMicroseconds(5);
                *(gpioPtr + GPPUD<gpio_t>()) = 0;
                delayMicroseconds(5);
                *(gpioPtr + gpioToPUDCLK(pinNumber)) = 0;
                delayMicroseconds(5);
            }
        }

        // Set an output PWM value
        template <const pin_t pinNumber>
        inline void pwmWrite(volatile gpio_t *pwmPtr, const gpio_t value) const volatile
        {
            static_assert(pinNumber != nullPin<pin_t>());
            *(pwmPtr + gpioToPwmPort_[pinNumber]) = value;
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

    private:
        gpioTable pinMode_ = pinModeI();
        static constexpr const gpioTable gpioToPwmPort_ = gpioToPwmPortI();
    };
}
#endif
