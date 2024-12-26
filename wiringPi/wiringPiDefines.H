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
// #include "wiringPiStrings.H"

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
    template <const int mode_>
    using mode_constant = const std::integral_constant<int, mode_>;
    template <const int pi_model_>
    using pi_model_constant = const std::integral_constant<int, pi_model_>;
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

    // Mask for the bottom 64 pins which belong to the Raspberry Pi
    // The others are available for the other devices
    template <typename T>
    [[nodiscard]] inline consteval T PI_GPIO_MASK()
    {
        return static_cast<T>(0xFFFFFFC0);
    }

    // Pin table types
    typedef std::array<pin_t, MAX_PINS<size_t>()> pinTable;
    typedef std::array<bool, MAX_PINS<size_t>()> switchTable;
    typedef std::array<gpio_t, MAX_PINS<size_t>()> gpioTable;
    typedef std::array<std::string_view, MAX_PINS<size_t>()> nameTable;
    typedef std::array<int, MAX_PINS<size_t>()> fileDescriptorTable;
    typedef std::array<std::thread, MAX_PINS<size_t>()> threadTable;

    // Default initialisation of std::thread
    // [[nodiscard]] inline consteval std::thread nullThread()
    // {
    //     return std::thread;
    // }

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
    namespace pinModes
    {
        [[nodiscard]] inline consteval gpio_t input()
        {
            return 0;
        }
        [[nodiscard]] inline consteval gpio_t output()
        {
            return 1;
        }
        [[nodiscard]] inline consteval gpio_t pwmOutput()
        {
            return 2;
        }
        [[nodiscard]] inline consteval gpio_t gpioClock()
        {
            return 3;
        }
        [[nodiscard]] inline consteval gpio_t softPwmOutput()
        {
            return 4;
        }
        [[nodiscard]] inline consteval gpio_t softToneOutput()
        {
            return 5;
        }
        [[nodiscard]] inline consteval gpio_t pwmToneOutput()
        {
            return 6;
        }
        [[nodiscard]] inline consteval gpio_t pwmOff()
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
    }
    using input = const gpio_constant<pinModes::input()>;
    using output = const gpio_constant<pinModes::output()>;
    using pwmOutput = const gpio_constant<pinModes::pwmOutput()>;
    using gpioClock = const gpio_constant<pinModes::gpioClock()>;
    using softPwmOutput = const gpio_constant<pinModes::softPwmOutput()>;
    using softToneOutput = const gpio_constant<pinModes::softToneOutput()>;
    using pwmToneOutput = const gpio_constant<pinModes::pwmToneOutput()>;
    using pwmOff = const gpio_constant<pinModes::pwmOff()>;

    // Return values
    namespace returnValues
    {
        template <typename T>
        [[nodiscard]] inline consteval T high()
        {
            return 1;
        }
        template <typename T>
        [[nodiscard]] inline consteval T low()
        {
            return 0;
        }
    }
    using high = const gpio_constant<returnValues::high<gpio_t>()>;
    using low = const gpio_constant<returnValues::low<gpio_t>()>;

    // wiringPi modes
    namespace wiringPiModes
    {
        [[nodiscard]] inline consteval int pins()
        {
            return 0;
        }
        [[nodiscard]] inline consteval int gpio()
        {
            return 1;
        }
        [[nodiscard]] inline consteval int gpioSys()
        {
            return 2;
        }
        [[nodiscard]] inline consteval int phys()
        {
            return 3;
        }
        [[nodiscard]] inline consteval int piFace()
        {
            return 4;
        }
        [[nodiscard]] inline consteval int uninitialised()
        {
            return -1;
        }
        [[nodiscard]] inline consteval std::string_view names(const pin_t mode)
        {
            constexpr const std::array<std::string_view, 6> arr{
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
            [[nodiscard]] inline consteval int PiA()
            {
                return 0;
            }
            [[nodiscard]] inline consteval int PiB()
            {
                return 1;
            }
            [[nodiscard]] inline consteval int PiAP()
            {
                return 2;
            }
            [[nodiscard]] inline consteval int PiBP()
            {
                return 3;
            }
            [[nodiscard]] inline consteval int Pi2()
            {
                return 4;
            }
            [[nodiscard]] inline consteval int PiAlpha()
            {
                return 5;
            }
            [[nodiscard]] inline consteval int PiCM()
            {
                return 6;
            }
            [[nodiscard]] inline consteval int Pi07()
            {
                return 7;
            }
            [[nodiscard]] inline consteval int Pi3B()
            {
                return 8;
            }
            [[nodiscard]] inline consteval int PiZero()
            {
                return 9;
            }
            [[nodiscard]] inline consteval int PiCM3()
            {
                return 10;
            }
            [[nodiscard]] inline consteval int PiZero_W()
            {
                return 12;
            }
            [[nodiscard]] inline consteval int Pi3BP()
            {
                return 13;
            }
            [[nodiscard]] inline consteval int Pi3AP()
            {
                return 14;
            }
            [[nodiscard]] inline consteval int PiCM3P()
            {
                return 16;
            }
            [[nodiscard]] inline consteval int Pi4B()
            {
                return 17;
            }
            [[nodiscard]] inline consteval int PiZero_2W()
            {
                return 18;
            }
            [[nodiscard]] inline consteval int Pi400()
            {
                return 19;
            }
            [[nodiscard]] inline consteval int PiCM4()
            {
                return 20;
            }
            [[nodiscard]] inline consteval int PiCM4S()
            {
                return 21;
            }
            [[nodiscard]] inline consteval int Pi5()
            {
                return 23;
            }
            [[nodiscard]] inline consteval std::string_view names(const size_t model)
            {
                constexpr const std::array<std::string_view, 21> arr{
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
        [[nodiscard]] consteval const std::string_view deviceName()
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

}

#endif
