#ifndef __WIRING_PI_DEFINES_H
#define __WIRING_PI_DEFINES_H

#include "wiringPiIncludes.H"

namespace WiringPi
{

    // Data type of pins
    typedef uint32_t pin_t;
    typedef uint32_t gpio_t;
    typedef uint32_t pwm_t;
    typedef uint32_t clk_t;
    typedef uint32_t pads_t;
    typedef uint32_t timer_t;
    typedef std::array<pin_t, 64> pinTable;
    typedef std::array<gpio_t, 64> gpioTable;

    // Mask for the bottom 64 pins which belong to the Raspberry Pi
    // The others are available for the other devices
    template <typename T>
    [[nodiscard]] inline consteval T PI_GPIO_MASK()
    {
        return static_cast<T>(0xFFFFFFC0);
    }

    // Environment strings
    [[nodiscard]] consteval const std::string_view ENV_DEBUG()
    {
        return "WIRINGPI_DEBUG";
    }
    [[nodiscard]] consteval const std::string_view ENV_CODES()
    {
        return "WIRINGPI_CODES";
    }
    [[nodiscard]] consteval const std::string_view ENV_GPIOMEM()
    {
        return "WIRINGPI_GPIOMEM";
    }

    // Pin modes
    template <typename T>
    [[nodiscard]] inline consteval T INPUT()
    {
        return 0;
    }
    template <typename T>
    [[nodiscard]] inline consteval T OUTPUT()
    {
        return 1;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM_OUTPUT()
    {
        return 2;
    }
    template <typename T>
    [[nodiscard]] inline consteval T GPIO_CLOCK()
    {
        return 3;
    }
    template <typename T>
    [[nodiscard]] inline consteval T SOFT_PWM_OUTPUT()
    {
        return 4;
    }
    template <typename T>
    [[nodiscard]] inline consteval T SOFT_TONE_OUTPUT()
    {
        return 5;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM_TONE_OUTPUT()
    {
        return 6;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM_OFF()
    {
        return 7;
    }

    // Return values
    template <typename T>
    [[nodiscard]] inline consteval T HIGH()
    {
        return 1;
    }
    template <typename T>
    [[nodiscard]] inline consteval T LOW()
    {
        return 0;
    }

    // wiringPi modes
    [[nodiscard]] inline consteval int WPI_MODE_PINS()
    {
        return 0;
    }
    [[nodiscard]] inline consteval int WPI_MODE_GPIO()
    {
        return 1;
    }
    [[nodiscard]] inline consteval int WPI_MODE_GPIO_SYS()
    {
        return 2;
    }
    [[nodiscard]] inline consteval int WPI_MODE_PHYS()
    {
        return 3;
    }
    [[nodiscard]] inline consteval int WPI_MODE_PIFACE()
    {
        return 4;
    }
    [[nodiscard]] inline consteval int WPI_MODE_UNINITIALISED()
    {
        return -1;
    }

    // Pi models
    [[nodiscard]] inline consteval int PI_MODEL_A()
    {
        return 0;
    }
    [[nodiscard]] inline consteval int PI_MODEL_B()
    {
        return 1;
    }
    [[nodiscard]] inline consteval int PI_MODEL_AP()
    {
        return 2;
    }
    [[nodiscard]] inline consteval int PI_MODEL_BP()
    {
        return 3;
    }
    [[nodiscard]] inline consteval int PI_MODEL_2()
    {
        return 4;
    }
    [[nodiscard]] inline consteval int PI_ALPHA()
    {
        return 5;
    }
    [[nodiscard]] inline consteval int PI_MODEL_CM()
    {
        return 6;
    }
    [[nodiscard]] inline consteval int PI_MODEL_07()
    {
        return 7;
    }
    [[nodiscard]] inline consteval int PI_MODEL_3B()
    {
        return 8;
    }
    [[nodiscard]] inline consteval int PI_MODEL_ZERO()
    {
        return 9;
    }
    [[nodiscard]] inline consteval int PI_MODEL_CM3()
    {
        return 10;
    }
    [[nodiscard]] inline consteval int PI_MODEL_ZERO_W()
    {
        return 12;
    }
    [[nodiscard]] inline consteval int PI_MODEL_3BP()
    {
        return 13;
    }
    [[nodiscard]] inline consteval int PI_MODEL_3AP()
    {
        return 14;
    }
    [[nodiscard]] inline consteval int PI_MODEL_CM3P()
    {
        return 16;
    }
    [[nodiscard]] inline consteval int PI_MODEL_4B()
    {
        return 17;
    }
    [[nodiscard]] inline consteval int PI_MODEL_ZERO_2W()
    {
        return 18;
    }
    [[nodiscard]] inline consteval int PI_MODEL_400()
    {
        return 19;
    }
    [[nodiscard]] inline consteval int PI_MODEL_CM4()
    {
        return 20;
    }

    // Pi versions
    template <typename T>
    [[nodiscard]] inline consteval T PI_VERSION_1()
    {
        return 0;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_VERSION_1_1()
    {
        return 1;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_VERSION_1_2()
    {
        return 2;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_VERSION_2()
    {
        return 3;
    }

    // Pi manufacturers
    template <typename T>
    [[nodiscard]] inline consteval T PI_MAKER_SONY()
    {
        return 0;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MAKER_EGOMAN()
    {
        return 1;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MAKER_EMBEST()
    {
        return 2;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MAKER_UNKNOWN()
    {
        return 3;
    }

    // Error codes
    template <typename T>
    [[nodiscard]] inline consteval T WPI_FATAL()
    {
        return 1 == 1;
    }
    template <typename T>
    [[nodiscard]] inline consteval T WPI_ALMOST()
    {
        return 1 == 2;
    }

    // Memory params
    template <typename T>
    [[nodiscard]] inline consteval T PAGE_SIZE()
    {
        return 4 * 1024;
    }
    template <typename T>
    [[nodiscard]] inline consteval T BLOCK_SIZE()
    {
        return 4 * 1024;
    }

    // The base address of the GPIO memory mapped hardware IO
    template <typename T>
    [[nodiscard]] inline consteval T GPIO_PERI_BASE_OLD()
    {
        return 0x20000000;
    }
    template <typename T>
    [[nodiscard]] inline consteval T GPIO_PERI_BASE_2835()
    {
        return 0x3F000000;
    }
    template <typename T>
    [[nodiscard]] inline consteval T GPIO_PERI_BASE_2711()
    {
        return static_cast<T>(0xFE000000);
    }

    // Pin pull up/down register
    // Pin pull-up/down for pins 15 : 0
    // Pin pull-up/down for pins 31 : 16
    // Pin pull-up/down for pins 47 : 32
    // Pin pull-up/down for pins 57 : 48
    template <typename T>
    [[nodiscard]] inline consteval T GPPUD()
    {
        return 37;
    }
    template <typename T>
    [[nodiscard]] inline consteval T GPPUPPDN0()
    {
        return 57;
    }
    template <typename T>
    [[nodiscard]] inline consteval T GPPUPPDN1()
    {
        return 58;
    }
    template <typename T>
    [[nodiscard]] inline consteval T GPPUPPDN2()
    {
        return 59;
    }
    template <typename T>
    [[nodiscard]] inline consteval T GPPUPPDN3()
    {
        return 60;
    }

    // Pull up/down/none
    template <typename T>
    [[nodiscard]] inline consteval T PUD_OFF()
    {
        return 0;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PUD_DOWN()
    {
        return 1;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PUD_UP()
    {
        return 2;
    }

    // PWM offsets into the PWM control region
    template <typename T>
    [[nodiscard]] inline consteval T PWM_CONTROL()
    {
        return 0;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM_STATUS()
    {
        return 1;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM0_RANGE()
    {
        return 4;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM0_DATA()
    {
        return 5;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM1_RANGE()
    {
        return 8;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM1_DATA()
    {
        return 9;
    }

    // Timer offsets
    template <typename T>
    [[nodiscard]] inline consteval T TIMER_LOAD()
    {
        return 0x400 >> 2;
    }
    template <typename T>
    [[nodiscard]] inline consteval T TIMER_VALUE()
    {
        return 0x404 >> 2;
    }
    template <typename T>
    [[nodiscard]] inline consteval T TIMER_CONTROL()
    {
        return 0x408 >> 2;
    }
    template <typename T>
    [[nodiscard]] inline consteval T IMER_IRQ_CLR()
    {
        return 0x40C >> 2;
    }
    template <typename T>
    [[nodiscard]] inline consteval T TIMER_IRQ_RAW()
    {
        return 0x410 >> 2;
    }
    template <typename T>
    [[nodiscard]] inline consteval T TIMER_IRQ_MASK()
    {
        return 0x414 >> 2;
    }
    template <typename T>
    [[nodiscard]] inline consteval T TIMER_RELOAD()
    {
        return 0x418 >> 2;
    }
    template <typename T>
    [[nodiscard]] inline consteval T TIMER_PRE_DIV()
    {
        return 0x41C >> 2;
    }
    template <typename T>
    [[nodiscard]] inline consteval T TIMER_COUNTER()
    {
        return 0x420 >> 2;
    }

}

#endif