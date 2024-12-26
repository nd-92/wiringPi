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
        [[nodiscard]] inline constexpr onboardPins() : pinMode_(pinTables::pinMode()) {};
        // [[nodiscard]] inline constexpr onboardPins(const gpioTable &pinModes)
        //     : pinMode_(pinModes) {};

        // Read the value of a given pin, returns either HIGH or LOW
        template <const pin_t pin_>
        [[nodiscard]] inline gpio_t digitalRead(const volatile gpio_t *gpioPtr, const pin_constant<pin_> pin) const volatile
        {
            return static_cast<gpio_t>((*(gpioPtr + pinTables::gpioToGPLEV(pin())) & pinTables::digitalReadModulo(pin())) != 0);
        }

        // Set an output bit on a given pin
        template <const pin_t pin, const gpio_t value>
        inline void digitalWrite(volatile gpio_t *gpioPtr) const volatile
        {
            *(gpioPtr + pinTables::gpioCLRSET(pin + (64 * !value))) = pinTables::digitalReadModulo(pin);
        }

        // Sets the mode of a pin to be input, output or PWM output
        template <const pin_t pin_, const gpio_t mode_, const pin_t fSel_, const pin_t shift_>
        inline void pinMode(volatile gpio_t *gpioPtr, const pin_constant<pin_> pin, const gpio_constant<mode_> mode, const pin_constant<fSel_> fSel, const pin_constant<shift_> shift)
        {
            // Check that the pin is valid
            static_assert(nullPinCheck(pin()));

            // Continue
            if constexpr (mode() == input() || mode() == pwmOff())
            {
                *(gpioPtr + fSel()) = (*(gpioPtr + fSel()) & static_cast<gpio_t>(~(7 << shift())));
            }
            else if constexpr (mode() == output())
            {
                *(gpioPtr + fSel()) = (*(gpioPtr + fSel()) & static_cast<gpio_t>(~(7 << shift()))) | (1 << shift());
            }
            pinMode_[pin()] = mode();
        }

        // Control the internal pull-up/down resistors on a GPIO pin
        template <const pin_t pin_, const gpio_t pud_, const gpio_t offset_>
        inline void pullUpDnControl(volatile gpio_t *gpioPtr, const pin_constant<pin_> pin, const gpio_constant<pud_> pud, const gpio_constant<offset_> offset) const volatile
        {
            // Check that the pin is valid
            static_assert(nullPinCheck(pin()));

            // Continue
            if constexpr (offset() == GPPUPPDN0<gpio_t>())
            {
                // Pi 4B pull up/down method
                *(gpioPtr + pinTables::pullReg(pin())) = pullBits(gpioPtr, pin(), pud());
            }
            else
            {
                *(gpioPtr + GPPUD<gpio_t>()) = pud() & 3;
                std::this_thread::sleep_for(std::chrono::microseconds(5));
                *(gpioPtr + gpioToPUDCLK(pin())) = digitalReadModulo(pin());
                std::this_thread::sleep_for(std::chrono::microseconds(5));
                *(gpioPtr + GPPUD<gpio_t>()) = 0;
                std::this_thread::sleep_for(std::chrono::microseconds(5));
                *(gpioPtr + gpioToPUDCLK(pin())) = 0;
                std::this_thread::sleep_for(std::chrono::microseconds(5));
            }
        }

        // Set an output PWM value
        template <const pin_t pin_>
        inline void pwmWrite(volatile gpio_t *pwmPtr, const pin_constant<pin_> pin, const gpio_t value) const volatile
        {
            // Check that the pin is valud
            static_assert(nullPinCheck(pin()));

            // Continue
            *(pwmPtr + gpioToPwmPort_[pin()]) = value;
        }

        // Returns the map of pin modes
        [[nodiscard]] inline constexpr const pinTable &pinModeMap() const
        {
            return pinMode_;
        }

        // Returns the name of the device
        [[nodiscard]] inline consteval name_t deviceName() const
        {
            return "Onboard pins";
        }

    private:
        // This is potentially constexpr, but not yet
        gpioTable pinMode_;

        static constexpr const gpioTable gpioToPwmPort_ = pinTables::gpioToPwmPort();

        template <const pin_t pin_, const gpio_t pud_>
        [[nodiscard]] inline constexpr pin_t pullBits(const volatile gpio_t *gpioPtr, const pin_constant<pin_> pin, const gpio_constant<pud_> pud) const
        {
            pin_t pullBits_ = *(gpioPtr + pinTables::pullReg(pin()));
            pullBits_ &= static_cast<gpio_t>(~(3 << pinTables::pullShift(pin())));
            pullBits_ |= (pud() << pinTables::pullShift(pin()));
            return pullBits_;
        }
    };
}
#endif
