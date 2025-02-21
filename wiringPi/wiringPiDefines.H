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
    typedef uint64_t frequency_t;
    typedef std::string_view name_t;
    typedef double scalar_t;

    template <const pin_t pin_>
    using pin_constant = const std::integral_constant<pin_t, pin_>;
    template <const gpio_t gpio_>
    using gpio_constant = const std::integral_constant<gpio_t, gpio_>;
    template <const pwm_t pwm_>
    using pwm_constant = const std::integral_constant<pwm_t, pwm_>;
    template <const clk_t clk_>
    using clk_constant = const std::integral_constant<clk_t, clk_>;
    template <const pads_t pads_>
    using pads_constant = const std::integral_constant<pads_t, pads_>;
    template <const timer_t timer_>
    using timer_constant = const std::integral_constant<timer_t, timer_>;

    // Maximum number of pins
    template <typename T>
    [[nodiscard]] inline consteval pin_t MAX_PINS()
    {
        return 64;
    }

    // Type safe initialisation of a default value of -1
    template <typename T>
    [[nodiscard]] inline consteval T nullPin()
    {
        return static_cast<T>(-1);
    }

    // Type safe initialisation of a frequency of 5 kHz (used for softTone)
    [[nodiscard]] inline consteval frequency_t maxFrequency()
    {
        return 5000;
    }

    // Pin table types
    typedef std::array<pin_t, MAX_PINS<std::size_t>()> pinTable;
    typedef std::array<frequency_t, MAX_PINS<std::size_t>()> frequencyTable;
    typedef std::array<bool, MAX_PINS<std::size_t>()> switchTable;
    typedef std::array<gpio_t, MAX_PINS<std::size_t>()> gpioTable;
    typedef std::array<name_t, MAX_PINS<std::size_t>()> nameTable;
    typedef std::array<int, MAX_PINS<std::size_t>()> fileDescriptorTable;
    typedef std::array<std::thread, MAX_PINS<std::size_t>()> threadTable;

    // Pin modes
    namespace pinModes
    {
        // This is an enumerated type because the pin mode can only be one of a few select values
        typedef enum Enum : gpio_t
        {
            input = 0,
            output = 1,
            pwmOutput = 2,
            gpioClock = 3,
            softPwmOutput = 4,
            softToneOutput = 5,
            pwmToneOutput = 6,
            pwmOff = 7
        } type;

        template <const type pinMode_>
        using constant = const std::integral_constant<type, pinMode_>;

        using constant_input = constant<input>;
        using constant_output = constant<output>;

        [[nodiscard]] inline consteval std::array<name_t, 8> modeStrings()
        {
            return std::array<name_t, 8>{
                "  IN  ",
                "  OUT ",
                "PWM_OUTPUT",
                "GPIO_CLOCK",
                "SOFT_PWM_OUTPUT",
                "SOFT_TONE_OUTPUT",
                "PWM_TONE_OUTPUT",
                "PWM_OFF"};
        }
    }

    // Return values
    [[nodiscard]] inline consteval gpio_t high()
    {
        return 1;
    }
    [[nodiscard]] inline consteval gpio_t low()
    {
        return 0;
    }

    // wiringPi modes
    namespace wiringPiModes
    {
        // This is an enumerated type because the mode can only be one of a few select values
        typedef enum Enum : int
        {
            pins = 0,
            gpio = 1,
            gpioSys = 2,
            phys = 3,
            piFace = 4,
            uninitialised = -1,
            all = 100
        } type;

        template <const type mode_>
        using constant = const std::integral_constant<type, mode_>;

        [[nodiscard]] inline consteval name_t names(const pin_t mode)
        {
            constexpr const std::array<name_t, 6> arr{
                "PINS",
                "GPIO",
                "GPIO_SYS",
                "PHYS",
                "PIFACE",
                "UNINITIALISED"};
            return arr[mode];
        }
    }

    namespace Pi
    {
        // Pi models
        namespace model
        {
            // This is an enumerated type because the model can only be one of a few select values
            typedef enum Enum : std::size_t
            {
                PiA = 0,
                PiB = 1,
                PiAP = 2,
                PiBP = 3,
                Pi2 = 4,
                PiAlpha = 5,
                PiCM = 6,
                Pi07 = 7,
                Pi3B = 8,
                PiZero = 9,
                PiCM3 = 10,
                PiZero_W = 12,
                Pi3BP = 13,
                Pi3AP = 14,
                PiCM3P = 16,
                Pi4B = 17,
                PiZero_2W = 18,
                Pi400 = 19,
                PiCM4 = 20,
                PiCM4S = 21,
                Pi5 = 23
            } type;

            template <const type model_>
            using constant = const std::integral_constant<type, model_>;

            [[nodiscard]] inline consteval name_t names(const std::size_t model)
            {
                constexpr const std::array<name_t, 21> arr{
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
                return arr[model];
            }
        }

        // Pi versions
        namespace version
        {
            // This is an enumerated type because the version can only be one of a few select values
            typedef enum Enum : std::size_t
            {
                V1 = 0,   // Revision 1.0
                V1_1 = 1, // Revision 1.1
                V1_2 = 2, // Revision 1.2
                V2 = 3,   // Revision 2.0
                V1_4 = 4, // Revision 1.4
                V1_5 = 5  // Revision 1.5
            } type;
        }

        // Pi manufacturers
        namespace manufacturer
        {
            // This is an enumerated type because the manufacturer can only be one of a few select values
            typedef enum Enum : std::size_t
            {
                Sony_UK = 0,
                Egoman = 1,
                Embest = 2,
                Sony_Japan = 3,
                Embest_2 = 4,
                Stadium = 5
            } type;
        }

        // Memory capacity
        namespace memory
        {
            // This is an enumerated type because the memory can only be one of a few select values
            typedef enum Enum : std::size_t
            {
                MB256 = 0,
                MB512 = 1,
                GB1 = 2,
                GB2 = 3,
                GB4 = 4,
                GB8 = 5
            } type;
        }

        // GPIO board layout
        namespace layout
        {
            // This is an enumerated type because the layout can only be one of a few select values
            typedef enum Enum : std::size_t
            {
                PI1_REV1 = 1,
                DEFAULT = 2
            } type;

            template <const type layout_>
            using constant = const std::integral_constant<type, layout_>;

            template <typename T, const Pi::layout::type layout>
            [[nodiscard]] constexpr T deviceName()
            {
                if constexpr (layout == Pi::layout::PI1_REV1)
                {
                    return "/dev/i2c-0";
                }
                else
                {
                    return "/dev/i2c-1";
                }
            }
        }

        // Warranty
        namespace warranty
        {
            // This is an enumerated type because the warranty can only be valid or voided
            typedef enum Enum : std::size_t
            {
                valid = 0,
                voided = 1
            } type;
        }
    }

    // Memory params
    namespace memory
    {
        template <typename T>
        [[nodiscard]] inline consteval T pageSize()
        {
            return 4 * 1024;
        }
        template <typename T>
        [[nodiscard]] inline consteval T blockSize()
        {
            return 4 * 1024;
        }
    }

    // The base address of the GPIO memory mapped hardware IO
    namespace GPIO
    {
        namespace PERI
        {
            template <typename T>
            [[nodiscard]] inline consteval T BASE_OLD()
            {
                return 0x20000000;
            }
            template <typename T>
            [[nodiscard]] inline consteval T BASE_2835()
            {
                return 0x3F000000;
            }
            template <typename T>
            [[nodiscard]] inline consteval T BASE_2711()
            {
                return static_cast<T>(0xFE000000);
            }
        }
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

    namespace PWM
    {
        // PWM offsets into the PWM control region
        template <typename T>
        [[nodiscard]] inline consteval T CONTROL()
        {
            return 0;
        }
        template <typename T>
        [[nodiscard]] inline consteval T STATUS()
        {
            return 1;
        }
        namespace CLK
        {
            template <typename T>
            [[nodiscard]] inline consteval T CNTL()
            {
                return 40;
            }
            template <typename T>
            [[nodiscard]] inline consteval T DIV()
            {
                return 41;
            }
            template <typename T>
            [[nodiscard]] inline consteval T BCM_PASSWORD()
            {
                return 0x5A000000;
            }
        }
        namespace mode
        {
            // This is an enumerated type because the mode can only be one of a few select values
            typedef enum Enum : std::size_t
            {
                MS = 0,
                BAL = 1
            } type;

            template <const type mode_>
            using constant = const std::integral_constant<type, mode_>;
        }
    }

    // PWM0 defines
    namespace PWM0
    {
        template <typename T>
        [[nodiscard]] inline consteval T MS_MODE()
        {
            return 0x0080;
        }
        template <typename T>
        [[nodiscard]] inline consteval T USEFIFO()
        {
            return 0x0020;
        }
        template <typename T>
        [[nodiscard]] inline consteval T REVPOLAR()
        {
            return 0x0010;
        }
        template <typename T>
        [[nodiscard]] inline consteval T OFFSTATE()
        {
            return 0x0008;
        }
        template <typename T>
        [[nodiscard]] inline consteval T REPEATFF()
        {
            return 0x0004;
        }
        template <typename T>
        [[nodiscard]] inline consteval T SERIAL()
        {
            return 0x0002;
        }
        template <typename T>
        [[nodiscard]] inline consteval T ENABLE()
        {
            return 0x0001;
        }
        template <typename T>
        [[nodiscard]] inline consteval T RANGE()
        {
            return 4;
        }
        template <typename T>
        [[nodiscard]] inline consteval T DATA()
        {
            return 5;
        }
    }

    // PWM1 defines
    namespace PWM1
    {
        template <typename T>
        [[nodiscard]] inline consteval T MS_MODE()
        {
            return 0x8000;
        }
        template <typename T>
        [[nodiscard]] inline consteval T USEFIFO()
        {
            return 0x2000;
        }
        template <typename T>
        [[nodiscard]] inline consteval T REVPOLAR()
        {
            return 0x1000;
        }
        template <typename T>
        [[nodiscard]] inline consteval T OFFSTATE()
        {
            return 0x0800;
        }
        template <typename T>
        [[nodiscard]] inline consteval T REPEATFF()
        {
            return 0x0400;
        }
        template <typename T>
        [[nodiscard]] inline consteval T SERIAL()
        {
            return 0x0200;
        }
        template <typename T>
        [[nodiscard]] inline consteval T ENABLE()
        {
            return 0x0100;
        }
        template <typename T>
        [[nodiscard]] inline consteval T RANGE()
        {
            return 8;
        }
        template <typename T>
        [[nodiscard]] inline consteval T DATA()
        {
            return 9;
        }
    }

    // Timer offsets
    namespace timer
    {
        [[nodiscard]] inline consteval timer_t load()
        {
            return (0x400 >> 2);
        }
        [[nodiscard]] inline consteval timer_t value()
        {
            return (0x404 >> 2);
        }
        [[nodiscard]] inline consteval timer_t control()
        {
            return (0x408 >> 2);
        }
        [[nodiscard]] inline consteval timer_t irqClr()
        {
            return (0x40C >> 2);
        }
        [[nodiscard]] inline consteval timer_t irqRaw()
        {
            return (0x410 >> 2);
        }
        [[nodiscard]] inline consteval timer_t irqMask()
        {
            return (0x414 >> 2);
        }
        [[nodiscard]] inline consteval timer_t reload()
        {
            return (0x418 >> 2);
        }
        [[nodiscard]] inline consteval timer_t preDiv()
        {
            return (0x41C >> 2);
        }
        [[nodiscard]] inline consteval timer_t counter()
        {
            return (0x420 >> 2);
        }
    }

    namespace shift
    {
        [[nodiscard]] inline consteval int LSBFIRST()
        {
            return 0;
        }
        [[nodiscard]] inline consteval int MSBFIRST()
        {
            return 1;
        }
    }
}

#endif
