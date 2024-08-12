// ======================================================================== //
//                                                                          //
// wiringPiMisc.H                                                           //
//                                                                          //
// ======================================================================== //
// Miscellaneous useful functions for the wiringPi library                  //
// ======================================================================== //

#ifndef __WIRING_PI_wiringPiTemplateMacros_H
#define __WIRING_PI_wiringPiTemplateMacros_H

#include "../piInfo.H"
#include "../version.H"

namespace WiringPi
{

    // Check WPI mode
    [[nodiscard]] inline consteval int getMode(const int mode, const int model)
    {
        if ((model == PI_MODEL_CM<int>()) || (model == PI_MODEL_CM3<int>()) || (model == PI_MODEL_CM3P<int>()))
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
    [[nodiscard]] inline consteval T piGpioPupOffset(const int model)
    {
        switch (model)
        {
        case PI_MODEL_A<int>():
        {
            return 0;
        }
        case PI_MODEL_B<int>():
        {
            return 0;
        }
        case PI_MODEL_AP<int>():
        {
            return 0;
        }
        case PI_MODEL_BP<int>():
        {
            return 0;
        }
        case PI_MODEL_ALPHA<int>():
        {
            return 0;
        }
        case PI_MODEL_CM<int>():
        {
            return 0;
        }
        case PI_MODEL_ZERO<int>():
        {
            return 0;
        }
        case PI_MODEL_ZERO_W<int>():
        {
            return GPPUD<T>();
        }
        case PI_MODEL_4B<int>():
        {
            return 0;
        }
        case PI_MODEL_400<int>():
        {
            return 0;
        }
        case PI_MODEL_CM4<int>():
        {
            return GPPUPPDN0<T>();
        }
        default:
        {
            return GPPUD<T>();
        }
        }
    }

    // Initialise the base address of the GPIO memory mapped hardware IO
    template <typename T>
    [[nodiscard]] inline constexpr T piGpioBase(const int model, const bool usingGpioMem)
    {
        T i = 0;
        // Initialise piGpioBase and piGpioPupOffset from board model - default vals 1 and 0
        switch (model)
        {
        case PI_MODEL_A<int>():
        {
            break;
        }
        case PI_MODEL_B<int>():
        {
            break;
        }
        case PI_MODEL_AP<int>():
        {
            break;
        }
        case PI_MODEL_BP<int>():
        {
            break;
        }
        case PI_MODEL_ALPHA<int>():
        {
            break;
        }
        case PI_MODEL_CM<int>():
        {
            break;
        }
        case PI_MODEL_ZERO<int>():
        {
            break;
        }
        case PI_MODEL_ZERO_W<int>():
        {
            i = GPIO_PERI_BASE_OLD<T>();
            break;
        }
        case PI_MODEL_4B<int>():
        {
            break;
        }
        case PI_MODEL_400<int>():
        {
            break;
        }
        case PI_MODEL_CM4<int>():
        {
            i = GPIO_PERI_BASE_2711<T>();
            break;
        }
        default:
        {
            i = GPIO_PERI_BASE_2835<T>();
            break;
        }
        }

        if (usingGpioMem)
        {
            i = 0;
        }

        return i;
    }

    [[nodiscard]] int failure(const int fatal, const char *message, ...)
    {
        va_list argp;
        char buffer[1024];

        const bool wiringPiReturnCodes = true;

        if (!fatal && wiringPiReturnCodes)
        {
            return -1;
        }

        va_start(argp, message);
        vsnprintf(buffer, 1023, message, argp);
        va_end(argp);

        fprintf(stderr, "%s", buffer);
        exit(EXIT_FAILURE);

        return 0;
    }

}

#endif
