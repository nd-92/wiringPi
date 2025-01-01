// ======================================================================== //
//                                                                          //
// wiringPiPinTables.H                                                      //
//                                                                          //
// ======================================================================== //
// Contains pin conversion tables for various running modes                 //
// Used to initialise the wiringPi class with the correct pin table         //
// All are consteval to prevent incorrect code from being compiled          //
// ======================================================================== //

#ifndef __WIRING_PI_wiringPiPinTables_H
#define __WIRING_PI_wiringPiPinTables_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"

namespace WiringPi
{
    namespace pinTables
    {
        // Table holding default values for PWM mark
        [[nodiscard]] inline constexpr frequencyTable softPwmMarks()
        {
            return frequencyTable{
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0};
        }

        // Table holding default values for PWM range
        [[nodiscard]] inline constexpr frequencyTable softPwmRange()
        {
            return frequencyTable{
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0};
        }

        // Table holding default values for tone frequencies
        [[nodiscard]] inline constexpr frequencyTable softToneFreq()
        {
            return frequencyTable{
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0};
        }

        // Table holding default values for thread switches
        [[nodiscard]] inline constexpr switchTable threadSwitches()
        {
            return switchTable{
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0};
        }

        // Table holding default-initialised null functions
        void nullFunc()
        {
            return;
        }
        [[nodiscard]] threadTable threadRegistry()
        {
            threadTable threads;
            for (std::size_t i = 0; i < MAX_PINS<std::size_t>(); i++)
            {
                threads[i] = std::thread(nullFunc);
            }
            for (std::size_t i = 0; i < MAX_PINS<std::size_t>(); i++)
            {
                threads[i].join();
            }
            return threads;
        }

        // Returns 2 ^ (pin & 31)
        [[nodiscard]] inline consteval gpio_t digitalReadModulo(const pin_t pin)
        {
            return static_cast<gpio_t>(1 << (pin & 31));
        }

        // Offset to the Pull Up Down Clock regsiter
        // This is equivalent to the gpioToPUDCLK table
        // Returns 38 on pins 0 -> 31, 39 on pins 32 -> 63
        [[nodiscard]] inline consteval gpio_t gpioToPUDCLK(const pin_t pin)
        {
            return 38 + (pin / 32);
        }

        // Offset to the GPIO Input level registers for each GPIO pin
        // This is equivalent to the gpioToGPLEV table
        [[nodiscard]] inline consteval gpio_t gpioToGPLEV(const pin_t pin)
        {
            return (13 + (pin / 32));
        }

        // Offset to the GPIO Clear and Set registers for each GPIO pin
        // This is equivalent to the gpioToGPCLR and gpioToGPSET tables
        // Value passed to this function should be deviceNo + (64 * !value)
        // Returns 10 -> 11 for digital value LOW, 7 -> 8 for digital value HIGH
        [[nodiscard]] inline consteval gpio_t gpioCLRSET(const pin_t pin)
        {
            return (7 + (pin / 64) + (pin / 32));
        }

        [[nodiscard]] inline consteval gpio_t pullReg(const pin_t pin)
        {
            return (GPPUPPDN0<gpio_t>() + (pin >> 4));
        }

        [[nodiscard]] inline consteval gpio_t pullShift(const pin_t pin)
        {
            return ((pin & 0xf) << 1);
        }

        // Get the number of physical pins for the board layout
        // Fails compilation if layout is not 1 or 2
        template <const Pi::layout::type layout>
        [[nodiscard]] inline consteval pin_t numPhysicalPins()
        {
            static_assert(layoutCheck(layout));
            if constexpr (layout == 1)
            {
                return 17;
            }
            else
            {
                return 32;
            }
        }

        [[nodiscard]] inline consteval const pinTable gpioToClkCon()
        {
            return pinTable{
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), 28, 30, 32, nullPin<pin_t>(),
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), 28, 30, nullPin<pin_t>(), nullPin<pin_t>(),
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                28, nullPin<pin_t>(), 28, nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                nullPin<pin_t>(), nullPin<pin_t>(), 28, 30, 28, nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>()};
        }

        [[nodiscard]] inline consteval const pinTable gpioToClkDiv()
        {
            return pinTable{
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), 29, 31, 33, nullPin<pin_t>(),
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), 29, 31, nullPin<pin_t>(), nullPin<pin_t>(),
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                29, nullPin<pin_t>(), 29, nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                nullPin<pin_t>(), nullPin<pin_t>(), 29, 31, 29, nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>()};
        }

        [[nodiscard]] inline consteval const pinTable gpioToPwmPort()
        {
            return pinTable{
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, PWM0::DATA<pin_t>(), PWM1::DATA<pin_t>(), 0, 0,
                0, 0, PWM0::DATA<pin_t>(), PWM1::DATA<pin_t>(), 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                PWM0::DATA<pin_t>(), PWM1::DATA<pin_t>(), 0, 0, 0, PWM1::DATA<pin_t>(), 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0};
        }

        // Fails compilation if layout is not 1 or 2
        template <const Pi::layout::type layout>
        [[nodiscard]] inline consteval const pinTable pinToGpio()
        {
            static_assert(layoutCheck(layout));
            if constexpr (layout == 1) // A, B, Rev 1, 1.1
            {
                return pinTable{
                    17, 18, 21, 22, 23, 24, 25, 4,
                    0, 1, 8, 7, 10, 9, 11, 14,
                    15, nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>()};
            }
            else // A2, B2, A+, B+, CM, Pi2, Pi3, Zero, Zero W, Zero 2 W
            {
                return pinTable{
                    17, 18, 27, 22, 23, 24, 25, 4,
                    2, 3, 8, 7, 10, 9, 11, 14,
                    15, 28, 29, 30, 31, 5, 6, 13,
                    19, 26, 12, 16, 20, 21, 0, 1,
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>()};
            }
        }

        // Fails compilation if layout is not 1 or 2
        template <const Pi::layout::type layout>
        [[nodiscard]] inline consteval const pinTable physToGpio()
        {
            static_assert(layoutCheck(layout));
            if (layout == 1) // A, B, Rev 1, 1.1
            {
                return pinTable{
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), 0, nullPin<pin_t>(), 1, nullPin<pin_t>(), 4,
                    14, nullPin<pin_t>(), 15, 17, 18, 21, nullPin<pin_t>(), 22,
                    23, nullPin<pin_t>(), 24, 10, nullPin<pin_t>(), 9, 25, 11,
                    8, nullPin<pin_t>(), 7, nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>()};
            }
            else // A2, B2, A+, B+, CM, Pi2, Pi3, Zero, Zero W, Zero 2 W
            {
                return pinTable{
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), 2, nullPin<pin_t>(), 3, nullPin<pin_t>(), 4,
                    14, nullPin<pin_t>(), 15, 17, 18, 27, nullPin<pin_t>(), 22,
                    23, nullPin<pin_t>(), 24, 10, nullPin<pin_t>(), 9, 25, 11,
                    8, nullPin<pin_t>(), 7, 0, 1, 5, nullPin<pin_t>(), 6,
                    12, 13, nullPin<pin_t>(), 19, 16, 26, 20, nullPin<pin_t>(),
                    21, nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), 28, 29, 30, 31, nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>()};
            }
        }

        // Fails compilation if layout is not 1 or 2
        template <const Pi::layout::type layout>
        [[nodiscard]] inline consteval const pinTable gpioToGpio()
        {
            static_assert(layoutCheck(layout));
            if (layout == 1) // A, B, Rev 1, 1.1
            {
                return pinTable{
                    0, 1, 2, 3, 4, 5, 6, 7,
                    8, 9, 10, 11, 12, 13, 14, 15,
                    16, nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>()};
            }
            else // A2, B2, A+, B+, CM, Pi2, Pi3, Zero, Zero W, Zero 2 W
            {
                return pinTable{
                    0, 1, 2, 3, 4, 5, 6, 7,
                    8, 9, 10, 11, 12, 13, 14, 15,
                    16, 17, 18, 19, 20, 21, 22, 23,
                    24, 25, 26, 27, 28, 29, 30, 31,
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>()};
            }
        }

        [[nodiscard]] inline consteval const pinTable physToWpi()
        {
            return pinTable{
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), 8, nullPin<pin_t>(), 9, nullPin<pin_t>(), 7,
                15, nullPin<pin_t>(), 16, 0, 1, 2, nullPin<pin_t>(), 3,
                4, nullPin<pin_t>(), 5, 12, nullPin<pin_t>(), 13, 6, 14,
                10, nullPin<pin_t>(), 11, 30, 31, 21, nullPin<pin_t>(), 22,
                26, 23, nullPin<pin_t>(), 24, 27, 25, 28, nullPin<pin_t>(),
                29, nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), 17, 18, 19, 20, nullPin<pin_t>(),
                nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>()};
        }

        // Create the pin map for a given wiringPi mode
        // For now, fails compilation on modes other than PINS, GPIO and PHYS
        // Fails compilation if layout is not 1 or 2
        template <const Pi::layout::type layout, const wiringPiModes::type mode>
        [[nodiscard]] inline consteval const pinTable pinMap()
        {
            static_assert(layoutCheck(layout));
            static_assert(setupModeCheck(mode));

            switch (mode)
            {
            case wiringPiModes::pins:
            {
                return pinToGpio<layout>();
            }
            case wiringPiModes::gpio:
            {
                return gpioToGpio<layout>();
            }
            case wiringPiModes::phys:
            {
                return physToGpio<layout>();
            }
                // default:
                // {
                //     return pinTable{
                //         nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                //         nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                //         nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                //         nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                //         nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                //         nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                //         nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                //         nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>()};
                // }
            }
        }

        [[nodiscard]] inline consteval const pinTable gpioToGPLEV()
        {
            return pinTable{
                13, 13, 13, 13, 13, 13, 13, 13,
                13, 13, 13, 13, 13, 13, 13, 13,
                13, 13, 13, 13, 13, 13, 13, 13,
                13, 13, 13, 13, 13, 13, 13, 13,
                14, 14, 14, 14, 14, 14, 14, 14,
                14, 14, 14, 14, 14, 14, 14, 14,
                14, 14, 14, 14, 14, 14, 14, 14,
                14, 14, 14, 14, 14, 14, 14, 14};
        }

        [[nodiscard]] inline constexpr const pinTable sysFds(const wiringPiModes::type mode)
        {
            if (mode == wiringPiModes::gpioSys)
            {
                char fName[128];
                pinTable arr = pinTable{
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>()};
                for (pin_t pin = 0; pin < MAX_PINS<pin_t>(); ++pin)
                {
                    sprintf(fName, "/sys/class/gpio/gpio%u/value", pin);
                    arr[pin] = static_cast<pin_t>(open(fName, O_RDWR));
                }
                return arr;
            }
            else
            {
                return pinTable{
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(),
                    nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>()};
            }
        }

        [[nodiscard]] inline constexpr gpioTable pinMode()
        {
            return gpioTable{
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0};
        }

        [[nodiscard]] inline consteval const pinTable gpioToGPFSEL()
        {
            return pinTable{
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 1, 2, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 3, 3,
                3, 3, 3, 3, 3, 3, 3, 3,
                4, 4, 4, 4, 4, 4, 4, 4,
                4, 4, 5, 5, 5, 5, 5, 5,
                5, 5, 5, 5, nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>()};
        }

        [[nodiscard]] inline consteval const pinTable gpioToShift()
        {
            return pinTable{
                0, 3, 6, 9, 12, 15, 18, 21,
                24, 27, 0, 3, 6, 9, 12, 15,
                18, 21, 24, 27, 0, 3, 6, 9,
                12, 15, 18, 21, 24, 27, 0, 3,
                6, 9, 12, 15, 18, 21, 24, 27,
                0, 3, 6, 9, 12, 15, 18, 21,
                24, 27, 0, 3, 6, 9, 12, 15,
                18, 21, 24, 27, nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>(), nullPin<pin_t>()};
        }

        // Physical names of onboard pins
        [[nodiscard]] inline consteval const nameTable physNames()
        {
            return nameTable{
                "         ", " 3.3v    ",
                " 5v      ", " SDA.1   ",
                " 5v      ", " SCL.1   ",
                " 0v      ", " GPIO.07 ",
                " TxD     ", " 0v      ",
                " RxD     ", " GPIO.00 ",
                " GPIO.01 ", " GPIO.02 ",
                " 0v      ", " GPIO.03 ",
                " GPIO.04 ", " 3.3v    ",
                " GPIO.05 ", " MOSI    ",
                " 0v      ", " MISO    ",
                " GPIO.06 ", " SCLK    ",
                " CE0     ", " 0v      ",
                " CE1     ", " SDA.0   ",
                " SCL.0   ", " GPIO.21 ",
                " 0v      ", " GPIO.22 ",
                " GPIO.26 ", " GPIO.23 ",
                " 0v      ", " GPIO.24 ",
                " GPIO.27 ", " GPIO.25 ",
                " GPIO.28 ", " 0v      ",
                " GPIO.29 ", "         ",
                "         ", "         ",
                "         ", "         ",
                "         ", "         ",
                "         ", "         ",
                "         ", " GPIO.17 ",
                " GPIO.18 ", " GPIO.19 ",
                " GPIO.20 ", "         ",
                "         ", "         ",
                "         ", "         ",
                "         ", "         ",
                "         ", "         "};
        }

        [[nodiscard]] inline consteval const name_t physicalNames(const pin_t i)
        {
            if ((i == nullPin<pin_t>()) | (i > MAX_PINS<pin_t>() - 1))
            {
                return "         ";
            }
            else
            {
                return physNames()[i];
            }
        }

        // Array of file descriptors
        // Used to initialise isrFds in piThread
        [[nodiscard]] inline consteval fileDescriptorTable isrFds()
        {
            return fileDescriptorTable{
                -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, -1};
        }
    }
}

#endif
