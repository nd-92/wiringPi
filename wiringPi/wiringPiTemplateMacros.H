#ifndef __WIRING_PI_GLOBALS_H
#define __WIRING_PI_GLOBALS_H

#include "../piInfo.H"
#include "../version.H"

namespace WiringPi
{

    // Check WPI mode
    consteval int getMode(const int mode, const int model)
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

        if ((model == PI_MODEL_CM()) || (model == PI_MODEL_CM3()) || (model == PI_MODEL_CM3P()))
        {
            return WPI_MODE_GPIO();
        }
        else
        {
            return mode;
        }
    }

    // Initialise piGpioBase and piGpioPupOffset from board model - default vals 1 and 0
    template <typename T>
    consteval T piGpioPupOffset(const int model)
    {
        T i = 0;
        switch (model)
        {
        case PI_MODEL_A():
        case PI_MODEL_B():
        case PI_MODEL_AP():
        case PI_MODEL_BP():
        case PI_ALPHA():
        case PI_MODEL_CM():
        case PI_MODEL_ZERO():
        case PI_MODEL_ZERO_W():
            i = GPPUD<T>();
            break;
        case PI_MODEL_4B():
        case PI_MODEL_400():
        case PI_MODEL_CM4():
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

}

#endif