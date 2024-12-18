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
    class onboardPins : public wiringPiNode
    {
    public:
        [[nodiscard]] inline constexpr onboardPins() {};

        // Read the value of a given pin, returns either HIGH or LOW
        template <const pin_t __pinNumber>
        [[nodiscard]] inline gpio_t digitalRead(const volatile gpio_t *gpioPtr, const pin_constant<__pinNumber> pinNumber) const volatile
        {
            return static_cast<gpio_t>((*(gpioPtr + gpioToGPLEV(pinNumber())) & digitalReadModulo(pinNumber())) != 0);
        }

        // Set an output bit on a given pin
        template <const pin_t __pinNumber, const gpio_t __value>
        inline void digitalWrite(volatile gpio_t *gpioPtr, const pin_constant<__pinNumber> pinNumber, const pin_constant<__value> value) const volatile
        {
            *(gpioPtr + gpioCLRSET(pinNumber() + (64 * !value()))) = digitalReadModulo(pinNumber());
        }

        // Sets the mode of a pin to be input, output or PWM output
        template <const pin_t __pinNumber, const gpio_t __mode, const pin_t __fSel, const pin_t __shift>
        inline void pinMode(volatile gpio_t *gpioPtr, const pin_constant<__pinNumber> pinNumber, const gpio_constant<__mode> mode, const pin_constant<__fSel> fSel, const pin_constant<__shift> shift)
        {
            static_assert(pinNumber() != nullPin<pin_t>());
            if constexpr (mode() == INPUT<gpio_t>() || mode() == PWM_OFF<gpio_t>())
            {
                *(gpioPtr + fSel()) = (*(gpioPtr + fSel()) & static_cast<gpio_t>(~(7 << shift())));
            }
            else if constexpr (mode() == OUTPUT<gpio_t>())
            {
                *(gpioPtr + fSel()) = (*(gpioPtr + fSel()) & static_cast<gpio_t>(~(7 << shift()))) | (1 << shift());
            }
            pinMode_[pinNumber()] = mode();
        }

        // Control the internal pull-up/down resistors on a GPIO pin
        template <const pin_t __pinNumber, const gpio_t __pud, const gpio_t __offset>
        inline void pullUpDnControl(volatile gpio_t *gpioPtr, const pin_constant<__pinNumber> pinNumber, const gpio_constant<__pud> pud, const gpio_constant<__offset> offset) const volatile
        {
            static_assert(pinNumber() != nullPin<pin_t>());
            if constexpr (offset() == GPPUPPDN0<gpio_t>())
            {
                // Pi 4B pull up/down method
                pin_t pullBits = *(gpioPtr + pullReg<pinNumber>());
                pullBits &= static_cast<gpio_t>(~(3 << pullShift<pinNumber()>()));
                pullBits |= (pud() << pullShift<pinNumber()>());
                *(gpioPtr + pullReg<pinNumber>()) = pullBits;
            }
            else
            {
                *(gpioPtr + GPPUD<gpio_t>()) = pud() & 3;
                std::this_thread::sleep_for(std::chrono::microseconds(5));
                *(gpioPtr + gpioToPUDCLK(pinNumber())) = digitalReadModulo(pinNumber());
                std::this_thread::sleep_for(std::chrono::microseconds(5));
                *(gpioPtr + GPPUD<gpio_t>()) = 0;
                std::this_thread::sleep_for(std::chrono::microseconds(5));
                *(gpioPtr + gpioToPUDCLK(pinNumber())) = 0;
                std::this_thread::sleep_for(std::chrono::microseconds(5));
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
