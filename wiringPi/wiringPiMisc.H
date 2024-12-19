// ======================================================================== //
//                                                                          //
// wiringPiMisc.H                                                           //
//                                                                          //
// ======================================================================== //
// Miscellaneous useful functions for the wiringPi library                  //
// ======================================================================== //

#ifndef __WIRING_PI_wiringPiMisc_H
#define __WIRING_PI_wiringPiMisc_H

#include "../piInfo.H"
#include "../version.H"
#include "wiringPiDefines.H"

namespace WiringPi
{
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
