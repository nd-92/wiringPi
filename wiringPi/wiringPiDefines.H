#ifndef __WIRING_PI_DEFINES_H
#define __WIRING_PI_DEFINES_H

namespace WiringPi
{

    // Data type of pins
    typedef unsigned int pin_t;
    typedef unsigned int gpio_t;
    typedef unsigned int pwm_t;
    typedef unsigned int clk_t;
    typedef unsigned int pads_t;
    typedef unsigned int timer_t;

    // Suppress unused parameter warnings - not used yet
    // #define UNU __attribute__((unused))

    // Mask for the bottom 64 pins which belong to the Raspberry Pi
    // The others are available for the other devices

    template <typename T>
    consteval T PI_GPIO_MASK()
    {
        // return (0xFFFFFFC0);
        return static_cast<T>(0xFFFFFFC0);
    }

    // Environment strings
    consteval const char *ENV_DEBUG()
    {
        return "WIRINGPI_DEBUG";
    }
    consteval const char *ENV_CODES()
    {
        return "WIRINGPI_CODES";
    }
    consteval const char *ENV_GPIOMEM()
    {
        return "WIRINGPI_GPIOMEM";
    }

    // Return values
    template <typename T>
    consteval T HIGH()
    {
        return 1;
    }
    template <typename T>
    consteval T LOW()
    {
        return 0;
    }

    // wiringPi modes
    consteval int WPI_MODE_PINS()
    {
        return 0;
    }
    consteval int WPI_MODE_GPIO()
    {
        return 1;
    }
    consteval int WPI_MODE_GPIO_SYS()
    {
        return 2;
    }
    consteval int WPI_MODE_PHYS()
    {
        return 3;
    }
    consteval int WPI_MODE_PIFACE()
    {
        return 4;
    }
    consteval int WPI_MODE_UNINITIALISED()
    {
        return -1;
    }

    // Pi models
    consteval int PI_MODEL_A()
    {
        return 0;
    }
    consteval int PI_MODEL_B()
    {
        return 1;
    }
    consteval int PI_MODEL_AP()
    {
        return 2;
    }
    consteval int PI_MODEL_BP()
    {
        return 3;
    }
    consteval int PI_MODEL_2()
    {
        return 4;
    }
    consteval int PI_ALPHA()
    {
        return 5;
    }
    consteval int PI_MODEL_CM()
    {
        return 6;
    }
    consteval int PI_MODEL_07()
    {
        return 7;
    }
    consteval int PI_MODEL_3B()
    {
        return 8;
    }
    consteval int PI_MODEL_ZERO()
    {
        return 9;
    }
    consteval int PI_MODEL_CM3()
    {
        return 10;
    }
    consteval int PI_MODEL_ZERO_W()
    {
        return 12;
    }
    consteval int PI_MODEL_3BP()
    {
        return 13;
    }
    consteval int PI_MODEL_3AP()
    {
        return 14;
    }
    consteval int PI_MODEL_CM3P()
    {
        return 16;
    }
    consteval int PI_MODEL_4B()
    {
        return 17;
    }
    consteval int PI_MODEL_ZERO_2W()
    {
        return 18;
    }
    consteval int PI_MODEL_400()
    {
        return 19;
    }
    consteval int PI_MODEL_CM4()
    {
        return 20;
    }

    // Pi versions
    template <typename T>
    consteval T PI_VERSION_1()
    {
        return 0;
    }
    template <typename T>
    consteval T PI_VERSION_1_1()
    {
        return 1;
    }
    template <typename T>
    consteval T PI_VERSION_1_2()
    {
        return 2;
    }
    template <typename T>
    consteval T PI_VERSION_2()
    {
        return 3;
    }

    // Pi manufacturers
    template <typename T>
    consteval T PI_MAKER_SONY()
    {
        return 0;
    }
    template <typename T>
    consteval T PI_MAKER_EGOMAN()
    {
        return 1;
    }
    template <typename T>
    consteval T PI_MAKER_EMBEST()
    {
        return 2;
    }
    template <typename T>
    consteval T PI_MAKER_UNKNOWN()
    {
        return 3;
    }

    // Error codes
    template <typename T>
    consteval T WPI_FATAL()
    {
        return 1 == 1;
    }
    template <typename T>
    consteval T WPI_ALMOST()
    {
        return 1 == 2;
    }

    // Memory params
    template <typename T>
    consteval T PAGE_SIZE()
    {
        return 4 * 1024;
    }
    template <typename T>
    consteval T BLOCK_SIZE()
    {
        return 4 * 1024;
    }

    // piGpioBase:
    // The base address of the GPIO memory mapped hardware IO
    template <typename T>
    consteval T GPIO_PERI_BASE_OLD()
    {
        return 0x20000000;
    }
    template <typename T>
    consteval T GPIO_PERI_BASE_2835()
    {
        return 0x3F000000;
    }
    template <typename T>
    consteval T GPIO_PERI_BASE_2711()
    {
        return static_cast<T>(0xFE000000);
    }

    // GPPUD: pin pull up/down register
    // Pin pull-up/down for pins 15 : 0
    // Pin pull-up/down for pins 31 : 16
    // Pin pull-up/down for pins 47 : 32
    // Pin pull-up/down for pins 57 : 48
    template <typename T>
    consteval T GPPUD()
    {
        return 37;
    }
    template <typename T>
    consteval T GPPUPPDN0()
    {
        return 57;
    }
    template <typename T>
    consteval T GPPUPPDN1()
    {
        return 58;
    }
    template <typename T>
    consteval T GPPUPPDN2()
    {
        return 59;
    }
    template <typename T>
    consteval T GPPUPPDN3()
    {
        return 60;
    }

    // PWM offsets into the PWM control region
    template <typename T>
    consteval T PWM_CONTROL()
    {
        return 0;
    }
    template <typename T>
    consteval T PWM_STATUS()
    {
        return 1;
    }
    template <typename T>
    consteval T PWM0_RANGE()
    {
        return 4;
    }
    template <typename T>
    consteval T PWM0_DATA()
    {
        return 5;
    }
    template <typename T>
    consteval T PWM1_RANGE()
    {
        return 8;
    }
    template <typename T>
    consteval T PWM1_DATA()
    {
        return 9;
    }

    // Timer offsets
    template <typename T>
    consteval T TIMER_LOAD()
    {
        return 0x400 >> 2;
    }
    template <typename T>
    consteval T TIMER_VALUE()
    {
        return 0x404 >> 2;
    }
    template <typename T>
    consteval T TIMER_CONTROL()
    {
        return 0x408 >> 2;
    }
    template <typename T>
    consteval T IMER_IRQ_CLR()
    {
        return 0x40C >> 2;
    }
    template <typename T>
    consteval T TIMER_IRQ_RAW()
    {
        return 0x410 >> 2;
    }
    template <typename T>
    consteval T TIMER_IRQ_MASK()
    {
        return 0x414 >> 2;
    }
    template <typename T>
    consteval T TIMER_RELOAD()
    {
        return 0x418 >> 2;
    }
    template <typename T>
    consteval T TIMER_PRE_DIV()
    {
        return 0x41C >> 2;
    }
    template <typename T>
    consteval T TIMER_COUNTER()
    {
        return 0x420 >> 2;
    }

}

#endif