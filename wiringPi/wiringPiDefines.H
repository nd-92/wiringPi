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
    typedef ssize_t I2C_t;
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
    template <const I2C_t I2C_>
    using I2C_constant = const std::integral_constant<I2C_t, I2C_>;
    template <const int layout_>
    using layout_constant = const std::integral_constant<int, layout_>;
    template <const int pwm_mode_>
    using pwm_mode_constant = const std::integral_constant<int, pwm_mode_>;

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

    // Mask for the bottom 64 pins which belong to the Raspberry Pi
    // The others are available for the other devices
    template <typename T>
    [[nodiscard]] inline consteval T PI_GPIO_MASK()
    {
        return static_cast<T>(0xFFFFFFC0);
    }

    // Pin table types
    typedef std::array<pin_t, MAX_PINS<size_t>()> pinTable;
    typedef std::array<frequency_t, MAX_PINS<size_t>()> frequencyTable;
    typedef std::array<bool, MAX_PINS<size_t>()> switchTable;
    typedef std::array<gpio_t, MAX_PINS<size_t>()> gpioTable;
    typedef std::array<name_t, MAX_PINS<size_t>()> nameTable;
    typedef std::array<int, MAX_PINS<size_t>()> fileDescriptorTable;
    typedef std::array<std::thread, MAX_PINS<size_t>()> threadTable;

    // Environment strings - maybe unused
    [[nodiscard]] consteval const name_t ENV_DEBUG()
    {
        return "WIRINGPI_DEBUG";
    }
    [[nodiscard]] consteval const name_t ENV_CODES()
    {
        return "WIRINGPI_CODES";
    }
    [[nodiscard]] consteval const name_t ENV_GPIOMEM()
    {
        return "WIRINGPI_GPIOMEM";
    }

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

        [[nodiscard]] inline consteval std::array<name_t, 8> pinModes()
        {
            return std::array<name_t, 8>{
                "IN",
                "OUT",
                "PWM_OUTPUT",
                "GPIO_CLOCK",
                "SOFT_PWM_OUTPUT",
                "SOFT_TONE_OUTPUT",
                "PWM_TONE_OUTPUT",
                "PWM_OFF"};
        }
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
        namespace models
        {
            typedef enum Enum : int
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

            template <const type pi_model_>
            using constant = const std::integral_constant<type, pi_model_>;

            [[nodiscard]] inline consteval name_t names(const size_t model)
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
        namespace versions
        {
            template <typename T>
            [[nodiscard]] inline consteval T V1()
            {
                return 0;
            }
            template <typename T>
            [[nodiscard]] inline consteval T V1_1()
            {
                return 1;
            }
            template <typename T>
            [[nodiscard]] inline consteval T V1_2()
            {
                return 2;
            }
            template <typename T>
            [[nodiscard]] inline consteval T V_2()
            {
                return 3;
            }
        }

        // Pi manufacturers
        namespace manufacturers
        {
            [[nodiscard]] inline consteval int Sony()
            {
                return 0;
            }
            [[nodiscard]] inline consteval int Egoman()
            {
                return 1;
            }
            [[nodiscard]] inline consteval int Embest()
            {
                return 2;
            }
            [[nodiscard]] inline consteval int unknown()
            {
                return 3;
            }
        }

        namespace layout
        {
            typedef enum Enum : int
            {
                PI1_REV1 = 1,
                DEFAULT = 2
            } type;
        }
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
            [[nodiscard]] inline consteval int MS()
            {
                return 0;
            }
            [[nodiscard]] inline consteval int BAL()
            {
                return 1;
            }
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
            return 0x400 >> 2;
        }
        [[nodiscard]] inline consteval timer_t value()
        {
            return 0x404 >> 2;
        }
        [[nodiscard]] inline consteval timer_t control()
        {
            return 0x408 >> 2;
        }
        [[nodiscard]] inline consteval timer_t irqClr()
        {
            return 0x40C >> 2;
        }
        [[nodiscard]] inline consteval timer_t irqRaw()
        {
            return 0x410 >> 2;
        }
        [[nodiscard]] inline consteval timer_t irqMask()
        {
            return 0x414 >> 2;
        }
        [[nodiscard]] inline consteval timer_t reload()
        {
            return 0x418 >> 2;
        }
        [[nodiscard]] inline consteval timer_t preDiv()
        {
            return 0x41C >> 2;
        }
        [[nodiscard]] inline consteval timer_t counter()
        {
            return 0x420 >> 2;
        }
    }

    // I2C defines
    namespace I2C
    {
        template <typename T>
        [[nodiscard]] inline consteval T slave()
        {
            return 0x0703;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbus()
        {
            return 0x0720;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusRead()
        {
            return 1;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusWrite()
        {
            return 0;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusQuick()
        {
            return 0;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusByte()
        {
            return 1;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusByteData()
        {
            return 2;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusWordData()
        {
            return 3;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusProcCall()
        {
            return 4;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusBlockData()
        {
            return 5;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusI2CBlockBroken()
        {
            return 6;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusBlockProcCall()
        {
            return 7;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusI2CBlockData()
        {
            return 8;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusBlockMax()
        {
            return 32;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusI2CBlockMax()
        {
            return 32;
        }
        template <const int layout>
        [[nodiscard]] consteval const name_t deviceName()
        {
            if constexpr (layout == 1)
            {
                return "/dev/i2c-0";
            }
            else
            {
                return "/dev/i2c-1";
            }
        }
    }

    union i2c_smbus_data
    {
        uint8_t byte;
        uint16_t word;
        uint8_t block[I2C::smbusBlockMax<size_t>() + 2];
    };

    struct i2c_smbus_ioctl_data
    {
        const uint8_t read_write;
        const uint8_t command;
        const uint32_t size;
        union i2c_smbus_data *data;
    };

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
