#ifndef __WIRING_PI_GLOBALS_H
#define __WIRING_PI_GLOBALS_H

#include "../piInfo.H"
#include "../version.H"

// Check WPI mode
template <typename T>
consteval T getMode(const T mode, const T model)
{
    // T wpiMode = WPI_MODE_UNINITIALISED<T>();
    // if ((model == PI_MODEL_CM<T>()) || (model == PI_MODEL_CM3<T>()) || (model == PI_MODEL_CM3P<T>()))
    // {
    //     wpiMode = WPI_MODE_GPIO<T>();
    // }
    // else
    // {
    //     wpiMode = WPI_MODE_PINS<T>();
    // }

    // if (mode == WPI_MODE_GPIO<T>())
    // {
    //     wpiMode = WPI_MODE_GPIO<T>();
    // }

    // if (mode == WPI_MODE_PHYS<T>())
    // {
    //     wpiMode = WPI_MODE_PHYS<T>();
    // }

    // return wpiMode;

    if ((model == PI_MODEL_CM<T>()) || (model == PI_MODEL_CM3<T>()) || (model == PI_MODEL_CM3P<T>()))
    {
        return WPI_MODE_GPIO<T>();
    }
    else
    {
        return mode;
    }
}

// Initialise piGpioBase and piGpioPupOffset from board model - default vals 1 and 0
template <typename T>
consteval T piGpioPupOffset(const T model)
{
    T i = 0;
    switch (model)
    {
    case PI_MODEL_A<T>():
    case PI_MODEL_B<T>():
    case PI_MODEL_AP<T>():
    case PI_MODEL_BP<T>():
    case PI_ALPHA<T>():
    case PI_MODEL_CM<T>():
    case PI_MODEL_ZERO<T>():
    case PI_MODEL_ZERO_W<T>():
        i = GPPUD<T>();
        break;
    case PI_MODEL_4B<T>():
    case PI_MODEL_400<T>():
    case PI_MODEL_CM4<T>():
        i = GPPUPPDN0<T>();
        break;
    default:
        i = GPPUD<T>();
        break;
    }

    return i;
}

void failure(const char *message, ...)
{
    va_list argp;
    char buffer[1024];

    va_start(argp, message);
    vsnprintf(buffer, 1023, message, argp);
    va_end(argp);

    fprintf(stderr, "%s", buffer);
    exit(EXIT_FAILURE);
}

#endif