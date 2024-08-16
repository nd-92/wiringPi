// ======================================================================== //
//                                                                          //
// wiringPiDefines.H                                                        //
//                                                                          //
// ======================================================================== //
// Useful typedefs, constants, etc used throughout the source code          //
// ======================================================================== //

#ifndef __WIRING_PI_wiringPiDefines_H
#define __WIRING_PI_wiringPiDefines_H

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
    typedef std::array<std::string_view, 64> nameTable;

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
    [[nodiscard]] inline consteval std::array<std::string_view, 8> pinModes()
    {
        return std::array<std::string_view, 8>{
            "IN",
            "OUT",
            "PWM_OUTPUT",
            "GPIO_CLOCK",
            "SOFT_PWM_OUTPUT",
            "SOFT_TONE_OUTPUT",
            "PWM_TONE_OUTPUT",
            "PWM_OFF"};
    }
    [[nodiscard]] inline consteval std::array<std::string_view, 8> modeStrings()
    {
        return std::array<std::string_view, 8>{
            "  IN  ",
            "  OUT ",
            "PWM_OUTPUT",
            "GPIO_CLOCK",
            "SOFT_PWM_OUTPUT",
            "SOFT_TONE_OUTPUT",
            "PWM_TONE_OUTPUT",
            "PWM_OFF"};
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
    [[nodiscard]] inline consteval std::array<std::string_view, 6> wiringPiModes()
    {
        return std::array<std::string_view, 6>{
            "PINS",
            "GPIO",
            "GPIO_SYS",
            "PHYS",
            "PIFACE",
            "UNINITIALISED"};
    }

    // Pi models
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_A()
    {
        return 0;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_B()
    {
        return 1;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_AP()
    {
        return 2;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_BP()
    {
        return 3;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_2()
    {
        return 4;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_ALPHA()
    {
        return 5;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_CM()
    {
        return 6;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_07()
    {
        return 7;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_3B()
    {
        return 8;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_ZERO()
    {
        return 9;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_CM3()
    {
        return 10;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_ZERO_W()
    {
        return 12;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_3BP()
    {
        return 13;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_3AP()
    {
        return 14;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_CM3P()
    {
        return 16;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_4B()
    {
        return 17;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_ZERO_2W()
    {
        return 18;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_400()
    {
        return 19;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_CM4()
    {
        return 20;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_CM4S()
    {
        return 21;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PI_MODEL_5()
    {
        return 23;
    }

    [[nodiscard]] inline consteval std::array<std::string_view, 21> piModelStrings()
    {
        return std::array<std::string_view, 21>{
            "A",
            "B",
            "AP",
            "BP",
            "2",
            "ALPHA",
            "CM",
            "07",
            "3B",
            "ZERO",
            "CM3",
            "",
            "ZERO_W",
            "3BP",
            "3AP",
            "",
            "CM3P",
            "4B",
            "ZERO_2W",
            "400",
            "CM4"};
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
    template <typename T>
    [[nodiscard]] inline consteval T GPIO_CLOCK_SOURCE()
    {
        return 1;
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

    // PWM
    template <typename T>
    [[nodiscard]] inline consteval T PWM_MODE_MS()
    {
        return 0;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM_MODE_BAL()
    {
        return 1;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM0_MS_MODE()
    {
        return 0x0080;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM0_USEFIFO()
    {
        return 0x0020;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM0_REVPOLAR()
    {
        return 0x0010;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM0_OFFSTATE()
    {
        return 0x0008;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM0_REPEATFF()
    {
        return 0x0004;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM0_SERIAL()
    {
        return 0x0002;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM0_ENABLE()
    {
        return 0x0001;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM1_MS_MODE()
    {
        return 0x8000;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM1_USEFIFO()
    {
        return 0x2000;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM1_REVPOLAR()
    {
        return 0x1000;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM1_OFFSTATE()
    {
        return 0x0800;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM1_REPEATFF()
    {
        return 0x0400;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM1_SERIAL()
    {
        return 0x0200;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWM1_ENABLE()
    {
        return 0x0100;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWMCLK_CNTL()
    {
        return 40;
    }
    template <typename T>
    [[nodiscard]] inline consteval T PWMCLK_DIV()
    {
        return 41;
    }
    template <typename T>
    [[nodiscard]] inline consteval T BCM_PASSWORD()
    {
        return 0x5A000000;
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

    // Static assertion check for valid program mode
    template <const int mode>
    inline void setupModeCheck()
    {
        static_assert(((mode == WPI_MODE_PINS()) || (mode == WPI_MODE_PHYS()) || (mode == WPI_MODE_GPIO())), "Invalid wiringPiMode for pwmSetMode: valid modes are PINS, PHYS, GPIO.");
    }

    // Static assertion check for non-GPIO mode on certain models
    template <const int model, const int mode>
    inline void computeModelCheck()
    {
        static_assert(!(model == PI_MODEL_CM<int>() && mode != WPI_MODE_GPIO()), "Only WPI_MODE_GPIO available on PI_MODEL_CM");
        static_assert(!(model == PI_MODEL_CM3<int>() && mode != WPI_MODE_GPIO()), "Only WPI_MODE_GPIO available on PI_MODEL_CM3");
        static_assert(!(model == PI_MODEL_CM3P<int>() && mode != WPI_MODE_GPIO()), "Only WPI_MODE_GPIO available on PI_MODEL_CM3P");
    }

    template <const pin_t nPins, const pinTable pinMap_, const pin_t pinNumber>
    inline void onboardPinCheck()
    {
        static_assert(pinMap_[pinNumber] < nPins, "Pin number is not onboard (onboard pins range from 0 to 31)");
    }

}

#endif
