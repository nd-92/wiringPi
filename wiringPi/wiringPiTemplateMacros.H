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
#include "wiringPiDefines.H"

namespace WiringPi
{
    // Initialise piGpioBase and piGpioPupOffset from board model - default vals 1 and 0
    template <typename T, const int model>
    [[nodiscard]] inline consteval T piGpioPupOffset()
    {
        static_assert(piModelCheck(model), "Invalid Raspberry Pi model");

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
    template <typename T, const int model>
    [[nodiscard]] inline constexpr T piGpioBase(const bool usingGpioMem)
    {
        // Check for a valid model
        static_assert(piModelCheck(model), "Invalid Raspberry Pi model");

        if (usingGpioMem)
        {
            return 0;
        }
        // Initialise piGpioBase and piGpioPupOffset from board model - default vals 1 and 0
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
            return GPIO_PERI_BASE_OLD<T>();
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
            return GPIO_PERI_BASE_2711<T>();
        }
        default:
        {
            return GPIO_PERI_BASE_2835<T>();
        }
        }
    }

    // Maybe unused
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
