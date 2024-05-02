#ifndef __WIRING_PI_DEFINES_H
#define __WIRING_PI_DEFINES_H

// Suppress unused parameter warnings - not used yet
#define UNU __attribute__((unused))

// Mask for the bottom 64 pins which belong to the Raspberry Pi
//	The others are available for the other devices

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
    return static_cast<T>(1);
}
template <typename T>
consteval T LOW()
{
    return static_cast<T>(0);
}

// wiringPi modes
template <typename T>
consteval T WPI_MODE_PINS()
{
    return static_cast<T>(0);
}
template <typename T>
consteval T WPI_MODE_GPIO()
{
    return static_cast<T>(1);
}
template <typename T>
consteval T WPI_MODE_GPIO_SYS()
{
    return static_cast<T>(2);
}
template <typename T>
consteval T WPI_MODE_PHYS()
{
    return static_cast<T>(3);
}
template <typename T>
consteval T WPI_MODE_PIFACE()
{
    return static_cast<T>(4);
}
template <typename T>
consteval T WPI_MODE_UNINITIALISED()
{
    return static_cast<T>(-1);
}

// Pi models
template <typename T>
consteval T PI_MODEL_A()
{
    return static_cast<T>(0);
}
template <typename T>
consteval T PI_MODEL_B()
{
    return static_cast<T>(1);
}
template <typename T>
consteval T PI_MODEL_AP()
{
    return static_cast<T>(2);
}
template <typename T>
consteval T PI_MODEL_BP()
{
    return static_cast<T>(3);
}
template <typename T>
consteval T PI_MODEL_2()
{
    return static_cast<T>(4);
}
template <typename T>
consteval T PI_ALPHA()
{
    return static_cast<T>(5);
}
template <typename T>
consteval T PI_MODEL_CM()
{
    return static_cast<T>(6);
}
template <typename T>
consteval T PI_MODEL_07()
{
    return static_cast<T>(7);
}
template <typename T>
consteval T PI_MODEL_3B()
{
    return static_cast<T>(8);
}
template <typename T>
consteval T PI_MODEL_ZERO()
{
    return static_cast<T>(9);
}
template <typename T>
consteval T PI_MODEL_CM3()
{
    return static_cast<T>(10);
}
template <typename T>
consteval T PI_MODEL_ZERO_W()
{
    return static_cast<T>(12);
}
template <typename T>
consteval T PI_MODEL_3BP()
{
    return static_cast<T>(13);
}
template <typename T>
consteval T PI_MODEL_3AP()
{
    return static_cast<T>(14);
}
template <typename T>
consteval T PI_MODEL_CM3P()
{
    return static_cast<T>(16);
}
template <typename T>
consteval T PI_MODEL_4B()
{
    return static_cast<T>(17);
}
template <typename T>
consteval T PI_MODEL_ZERO_2W()
{
    return static_cast<T>(18);
}
template <typename T>
consteval T PI_MODEL_400()
{
    return static_cast<T>(19);
}
template <typename T>
consteval T PI_MODEL_CM4()
{
    return static_cast<T>(20);
}

// Pi versions
template <typename T>
consteval T PI_VERSION_1()
{
    return static_cast<T>(0);
}
template <typename T>
consteval T PI_VERSION_1_1()
{
    return static_cast<T>(1);
}
template <typename T>
consteval T PI_VERSION_1_2()
{
    return static_cast<T>(2);
}
template <typename T>
consteval T PI_VERSION_2()
{
    return static_cast<T>(3);
}

// Pi manufacturers
template <typename T>
consteval T PI_MAKER_SONY()
{
    return static_cast<T>(0);
}
template <typename T>
consteval T PI_MAKER_EGOMAN()
{
    return static_cast<T>(1);
}
template <typename T>
consteval T PI_MAKER_EMBEST()
{
    return static_cast<T>(2);
}
template <typename T>
consteval T PI_MAKER_UNKNOWN()
{
    return static_cast<T>(3);
}

// Error codes
template <typename T>
consteval T WPI_FATAL()
{
    return static_cast<T>(1 == 1);
}
template <typename T>
consteval T WPI_ALMOST()
{
    return static_cast<T>(1 == 2);
}

// Memory params
template <typename T>
consteval T PAGE_SIZE()
{
    return static_cast<T>(4 * 1024);
}
template <typename T>
consteval T BLOCK_SIZE()
{
    return static_cast<T>(4 * 1024);
}

// piGpioBase:
// The base address of the GPIO memory mapped hardware IO
template <typename T>
consteval T GPIO_PERI_BASE_OLD()
{
    return static_cast<T>(0x20000000);
}
template <typename T>
consteval T GPIO_PERI_BASE_2835()
{
    return static_cast<T>(0x3F000000);
}
template <typename T>
consteval T GPIO_PERI_BASE_2711()
{
    return static_cast<T>(0xFE000000);
}

// GPPUD:
// GPIO Pin pull up/down register
// Pin pull-up/down for pins 15 : 0
// Pin pull-up/down for pins 31 : 16
// Pin pull-up/down for pins 47 : 32
// Pin pull-up/down for pins 57 : 48
template <typename T>
consteval T GPPUD()
{
    return static_cast<T>(37);
}
template <typename T>
consteval T GPPUPPDN0()
{
    return static_cast<T>(57);
}
template <typename T>
consteval T GPPUPPDN1()
{
    return static_cast<T>(58);
}
template <typename T>
consteval T GPPUPPDN2()
{
    return static_cast<T>(59);
}
template <typename T>
consteval T GPPUPPDN3()
{
    return static_cast<T>(60);
}

// Timer
// Word offsets
template <typename T>
consteval T TIMER_LOAD()
{
    return static_cast<T>(0x400 >> 2);
}
template <typename T>
consteval T TIMER_VALUE()
{
    return static_cast<T>(0x404 >> 2);
}
template <typename T>
consteval T TIMER_CONTROL()
{
    return static_cast<T>(0x408 >> 2);
}
template <typename T>
consteval T IMER_IRQ_CLR()
{
    return static_cast<T>(0x40C >> 2);
}
template <typename T>
consteval T TIMER_IRQ_RAW()
{
    return static_cast<T>(0x410 >> 2);
}
template <typename T>
consteval T TIMER_IRQ_MASK()
{
    return static_cast<T>(0x414 >> 2);
}
template <typename T>
consteval T TIMER_RELOAD()
{
    return static_cast<T>(0x418 >> 2);
}
template <typename T>
consteval T TIMER_PRE_DIV()
{
    return static_cast<T>(0x41C >> 2);
}
template <typename T>
consteval T TIMER_COUNTER()
{
    return static_cast<T>(0x420 >> 2);
}

#endif