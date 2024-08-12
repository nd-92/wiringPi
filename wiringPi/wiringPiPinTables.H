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
    // Type safe initialisation of a default value of -1
    template <typename T>
    [[nodiscard]] inline consteval T nullPin()
    {
        return static_cast<T>(-1);
    }

    // Get the number of physical pins for the board layout
    // Fails compilation if layout is not 1 or 2
    template <const int layout>
    [[nodiscard]] inline consteval pin_t numPhysicalPins()
    {
        static_assert((layout == 1 || layout == 2));
        if (layout == 1)
        {
            return 17;
        }
        else
        {
            return 32;
        }
    }

    [[nodiscard]] inline consteval const pinTable gpioToPwmPortI()
    {
        return std::array<pin_t, 64>{
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, PWM0_DATA<pin_t>(), PWM1_DATA<pin_t>(), 0, 0,
            0, 0, PWM0_DATA<pin_t>(), PWM1_DATA<pin_t>(), 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            PWM0_DATA<pin_t>(), PWM1_DATA<pin_t>(), 0, 0, 0, PWM1_DATA<pin_t>(), 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0};
    }

    // Fails compilation if layout is not 1 or 2
    template <const int layout>
    [[nodiscard]] inline consteval const pinTable pinToGpioI()
    {
        static_assert((layout == 1 || layout == 2));
        if (layout == 1) // A, B, Rev 1, 1.1
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
    template <const int layout>
    [[nodiscard]] inline consteval const pinTable physToGpioI()
    {
        static_assert((layout == 1 || layout == 2));
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
    template <const int layout>
    [[nodiscard]] inline consteval const pinTable gpioToGpio()
    {
        static_assert((layout == 1 || layout == 2));
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

    [[nodiscard]] inline consteval const pinTable physToWpiI()
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
    template <const int layout, const int mode>
    [[nodiscard]] inline consteval const pinTable pinMapI()
    {
        static_assert((layout == 1 || layout == 2));
        static_assert((mode == WPI_MODE_PINS() || mode == WPI_MODE_GPIO() || mode == WPI_MODE_PHYS()));
        switch (mode)
        {
        case WPI_MODE_PINS():
        {
            return pinToGpioI<layout>();
        }
        case WPI_MODE_GPIO():
        {
            return gpioToGpio<layout>();
        }
        case WPI_MODE_PHYS():
        {
            return physToGpioI<layout>();
        }
        default:
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
    }

    [[nodiscard]] inline consteval const pinTable gpioToGPLEVI()
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

    [[nodiscard]] inline constexpr const pinTable sysFdsI(const int mode)
    {
        if (mode == WPI_MODE_GPIO_SYS())
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
            for (pin_t pin = 0; pin < 64; ++pin)
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

    [[nodiscard]] inline constexpr gpioTable pinModeI()
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
    [[nodiscard]] inline consteval const nameTable physNamesI()
    {
        return nameTable{
            "       ", "   3.3v", "5v     ", "  SDA.1", "5v     ", "  SCL.1", "0v     ", "GPIO. 7",
            "TxD    ", "     0v", "RxD    ", "GPIO. 0", "GPIO. 1", "GPIO. 2", "0v     ", "GPIO. 3",
            "GPIO. 4", "   3.3v", "GPIO. 5", "   MOSI", "0v     ", "   MISO", "GPIO. 6", "   SCLK",
            "CE0    ", "     0v", "CE1    ", "  SDA.0", "SCL.0  ", "GPIO.21", "0v     ", "GPIO.22",
            "GPIO.26", "GPIO.23", "0v     ", "GPIO.24", "GPIO.27", "GPIO.25", "GPIO.28", "     0v",
            "GPIO.29", "       ", "       ", "       ", "       ", "       ", "       ", "       ",
            "       ", "       ", "       ", "GPIO.17", "GPIO.18", "GPIO.19", "GPIO.20", "       ",
            "       ", "       ", "       ", "       ", "       ", "       ", "       ", "       "};
    }
}

#endif
