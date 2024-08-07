#ifndef __WIRING_PI_PIN_TABLES_H
#define __WIRING_PI_PIN_TABLES_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"

namespace WiringPi
{
    // Type safe initialisation of a default value of -1
    template <typename T>
    [[nodiscard]] inline consteval T gpioMinusOne()
    {
        return static_cast<T>(-1);
    }

    [[nodiscard]] inline consteval const pinTable gpioToPwmPortI()
    {
        return std::array<pin_t, 64>{
            0, 0, 0, 0, 0, 0, 0, 0,                                                    //
            0, 0, 0, 0, PWM0_DATA<pin_t>(), PWM1_DATA<pin_t>(), 0, 0,                  //
            0, 0, PWM0_DATA<pin_t>(), PWM1_DATA<pin_t>(), 0, 0, 0, 0,                  //
            0, 0, 0, 0, 0, 0, 0, 0,                                                    //
            0, 0, 0, 0, 0, 0, 0, 0,                                                    //
            PWM0_DATA<pin_t>(), PWM1_DATA<pin_t>(), 0, 0, 0, PWM1_DATA<pin_t>(), 0, 0, //
            0, 0, 0, 0, 0, 0, 0, 0,                                                    //
            0, 0, 0, 0, 0, 0, 0, 0};
    }

    [[nodiscard]] inline consteval const pinTable pinToGpioI(const int layout)
    {
        if (layout == 1) // A, B, Rev 1, 1.1
        {
            return pinTable{
                17, 18, 21, 22, 23, 24, 25, 4,                                                                                                                                                          //
                0, 1, 8, 7, 10, 9, 11, 14,                                                                                                                                                              //
                15, gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(),                    //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>()};
        }
        else // A2, B2, A+, B+, CM, Pi2, Pi3, Zero, Zero W, Zero 2 W
        {
            return pinTable{
                17, 18, 27, 22, 23, 24, 25, 4,                                                                                                                                                          //
                2, 3, 8, 7, 10, 9, 11, 14,                                                                                                                                                              //
                15, 28, 29, 30, 31, 5, 6, 13,                                                                                                                                                           //
                19, 26, 12, 16, 20, 21, 0, 1,                                                                                                                                                           //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>()};
        }
    }

    [[nodiscard]] inline consteval const pinTable physToGpioI(const int layout)
    {
        if (layout == 1) // A, B, Rev 1, 1.1
        {
            return pinTable{
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), 0, gpioMinusOne<pin_t>(), 1, gpioMinusOne<pin_t>(), 4,                                                             //
                14, gpioMinusOne<pin_t>(), 15, 17, 18, 21, gpioMinusOne<pin_t>(), 22,                                                                                                                   //
                23, gpioMinusOne<pin_t>(), 24, 10, gpioMinusOne<pin_t>(), 9, 25, 11,                                                                                                                    //
                8, gpioMinusOne<pin_t>(), 7, gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(),                                         //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>()};
        }
        else // A2, B2, A+, B+, CM, Pi2, Pi3, Zero, Zero W, Zero 2 W
        {
            return pinTable{
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), 2, gpioMinusOne<pin_t>(), 3, gpioMinusOne<pin_t>(), 4,                                          //
                14, gpioMinusOne<pin_t>(), 15, 17, 18, 27, gpioMinusOne<pin_t>(), 22,                                                                                                //
                23, gpioMinusOne<pin_t>(), 24, 10, gpioMinusOne<pin_t>(), 9, 25, 11,                                                                                                 //
                8, gpioMinusOne<pin_t>(), 7, 0, 1, 5, gpioMinusOne<pin_t>(), 6,                                                                                                      //
                12, 13, gpioMinusOne<pin_t>(), 19, 16, 26, 20, gpioMinusOne<pin_t>(),                                                                                                //
                21, gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), 28, 29, 30, 31, gpioMinusOne<pin_t>(),                                                          //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>()};
        }
    }

    [[nodiscard]] inline consteval const pinTable gpioToGpio(const int layout)
    {
        if (layout == 1) // A, B, Rev 1, 1.1
        {
            return pinTable{
                0, 1, 2, 3, 4, 5, 6, 7,                                                                                                                                                                 //
                8, 9, 10, 11, 12, 13, 14, 15,                                                                                                                                                           //
                16, gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(),                    //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>()};
        }
        else // A2, B2, A+, B+, CM, Pi2, Pi3, Zero, Zero W, Zero 2 W
        {
            return pinTable{
                0, 1, 2, 3, 4, 5, 6, 7,                                                                                                                                                                 //
                8, 9, 10, 11, 12, 13, 14, 15,                                                                                                                                                           //
                16, 17, 18, 19, 20, 21, 22, 23,                                                                                                                                                         //
                24, 25, 26, 27, 28, 29, 30, 31,                                                                                                                                                         //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>()};
        }
    }

    [[nodiscard]] inline consteval const pinTable pinMapI(const int mode, const int layout)
    {
        if (mode == WPI_MODE_PINS())
        {
            return pinToGpioI(layout);
        }

        if (mode == WPI_MODE_GPIO())
        {
            return gpioToGpio(layout);
        }

        if (mode == WPI_MODE_PHYS())
        {
            return physToGpioI(layout);
        }

        return pinTable{
            gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
            gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
            gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
            gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
            gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
            gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
            gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
            gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>()};
    }

    [[nodiscard]] inline consteval const pinTable gpioToGPLEVI()
    {
        return pinTable{
            13, 13, 13, 13, 13, 13, 13, 13, //
            13, 13, 13, 13, 13, 13, 13, 13, //
            13, 13, 13, 13, 13, 13, 13, 13, //
            13, 13, 13, 13, 13, 13, 13, 13, //
            14, 14, 14, 14, 14, 14, 14, 14, //
            14, 14, 14, 14, 14, 14, 14, 14, //
            14, 14, 14, 14, 14, 14, 14, 14, //
            14, 14, 14, 14, 14, 14, 14, 14};
    }

    [[nodiscard]] inline constexpr const pinTable sysFdsI(const int mode)
    {
        if (mode == WPI_MODE_GPIO_SYS())
        {
            char fName[128];
            pinTable arr = pinTable{
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>()};
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
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), //
                gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>(), gpioMinusOne<pin_t>()};
        }
    }

}

#endif