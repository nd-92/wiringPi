#ifndef __WIRING_PI_PIN_TABLES_H
#define __WIRING_PI_PIN_TABLES_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"

namespace WiringPi
{
    using pinTable = std::array<pin_t, 64>;

    // Type safe initialisation of a default value of -1
    consteval gpio_t gpioMinusOne()
    {
        return static_cast<gpio_t>(-1);
    }

    consteval const pinTable gpioToPwmPortI()
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

    consteval const pinTable pinToGpioI(const int layout)
    {
        if (layout == 1) // A, B, Rev 1, 1.1
        {
            return pinTable{
                17, 18, 21, 22, 23, 24, 25, 4,                                                                                                  //
                0, 1, 8, 7, 10, 9, 11, 14,                                                                                                      //
                15, gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(),             //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne()};
        }
        else // A2, B2, A+, B+, CM, Pi2, Pi3, Zero, Zero W, Zero 2 W
        {
            return pinTable{
                17, 18, 27, 22, 23, 24, 25, 4,                                                                                                  //
                2, 3, 8, 7, 10, 9, 11, 14,                                                                                                      //
                15, 28, 29, 30, 31, 5, 6, 13,                                                                                                   //
                19, 26, 12, 16, 20, 21, 0, 1,                                                                                                   //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne()};
        }
    }

    consteval const pinTable physToGpioI(const int layout)
    {
        if (layout == 1) // A, B, Rev 1, 1.1
        {
            return pinTable{
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), 0, gpioMinusOne(), 1, gpioMinusOne(), 4,                                        //
                14, gpioMinusOne(), 15, 17, 18, 21, gpioMinusOne(), 22,                                                                         //
                23, gpioMinusOne(), 24, 10, gpioMinusOne(), 9, 25, 11,                                                                          //
                8, gpioMinusOne(), 7, gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(),                           //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne()};
        }
        else // A2, B2, A+, B+, CM, Pi2, Pi3, Zero, Zero W, Zero 2 W
        {
            return pinTable{
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), 2, gpioMinusOne(), 3, gpioMinusOne(), 4,                            //
                14, gpioMinusOne(), 15, 17, 18, 27, gpioMinusOne(), 22,                                                             //
                23, gpioMinusOne(), 24, 10, gpioMinusOne(), 9, 25, 11,                                                              //
                8, gpioMinusOne(), 7, 0, 1, 5, gpioMinusOne(), 6,                                                                   //
                12, 13, gpioMinusOne(), 19, 16, 26, 20, gpioMinusOne(),                                                             //
                21, gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), 28, 29, 30, 31, gpioMinusOne(),                                     //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne()};
        }
    }

    consteval const pinTable gpioToGPLEVI()
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

    constexpr const pinTable sysFdsI(const int mode)
    {
        if (mode == WPI_MODE_GPIO_SYS())
        {
            char fName[128];
            pinTable arr = pinTable{
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne()};
            for (size_t pin = 0; pin < 64; ++pin)
            {
                sprintf(fName, "/sys/class/gpio/gpio%d/value", pin);
                arr[pin] = static_cast<pin_t>(open(fName, O_RDWR));
            }
            return arr;
        }
        else
        {
            return pinTable{
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), //
                gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne(), gpioMinusOne()};
        }
    }

    // consteval const std::array<pin_t, 128> gpioCLRSETI()
    // {
    //     return std::array<pin_t, 128>{
    //         10, 10, 10, 10, 10, 10, 10, 10, //
    //         10, 10, 10, 10, 10, 10, 10, 10, //
    //         10, 10, 10, 10, 10, 10, 10, 10, //
    //         10, 10, 10, 10, 10, 10, 10, 10, //
    //         11, 11, 11, 11, 11, 11, 11, 11, //
    //         11, 11, 11, 11, 11, 11, 11, 11, //
    //         11, 11, 11, 11, 11, 11, 11, 11, //
    //         11, 11, 11, 11, 11, 11, 11, 11, //
    //         7, 7, 7, 7, 7, 7, 7, 7,         //
    //         7, 7, 7, 7, 7, 7, 7, 7,         //
    //         7, 7, 7, 7, 7, 7, 7, 7,         //
    //         7, 7, 7, 7, 7, 7, 7, 7,         //
    //         8, 8, 8, 8, 8, 8, 8, 8,         //
    //         8, 8, 8, 8, 8, 8, 8, 8,         //
    //         8, 8, 8, 8, 8, 8, 8, 8,         //
    //         8, 8, 8, 8, 8, 8, 8, 8};
    // }

    // consteval const std::array<pin_t, 128> gpioSETCLRI()
    // {
    //     return std::array<pin_t, 128>{
    //         7, 7, 7, 7, 7, 7, 7, 7,         //
    //         7, 7, 7, 7, 7, 7, 7, 7,         //
    //         7, 7, 7, 7, 7, 7, 7, 7,         //
    //         7, 7, 7, 7, 7, 7, 7, 7,         //
    //         8, 8, 8, 8, 8, 8, 8, 8,         //
    //         8, 8, 8, 8, 8, 8, 8, 8,         //
    //         8, 8, 8, 8, 8, 8, 8, 8,         //
    //         8, 8, 8, 8, 8, 8, 8, 8,         //
    //         10, 10, 10, 10, 10, 10, 10, 10, //
    //         10, 10, 10, 10, 10, 10, 10, 10, //
    //         10, 10, 10, 10, 10, 10, 10, 10, //
    //         10, 10, 10, 10, 10, 10, 10, 10, //
    //         11, 11, 11, 11, 11, 11, 11, 11, //
    //         11, 11, 11, 11, 11, 11, 11, 11, //
    //         11, 11, 11, 11, 11, 11, 11, 11, //
    //         11, 11, 11, 11, 11, 11, 11, 11};
    // }
}

#endif