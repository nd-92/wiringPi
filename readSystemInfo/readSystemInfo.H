// ************************************************************ //
// readSystemInfo.H:                                            //
// This program reads the info about the model of Raspberry Pi  //
// Used to generate the piInfo.H file for the wiringPi library  //
// A bit messy for now, clean up later                          //
// ************************************************************ //

// #ifndef __WIRING_PI_readSystemInfo_H
// #define __WIRING_PI_readSystemInfo_H

// Header includes
#include <asm/ioctl.h>
#include <cassert>
#include <chrono>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <poll.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_view>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <vector>

#define WPI_MODE_PINS 0
#define WPI_MODE_GPIO 1
#define WPI_MODE_GPIO_SYS 2
#define WPI_MODE_PHYS 3
#define WPI_MODE_PIFACE 4
#define WPI_MODE_UNINITIALISED -1

#define PI_MODEL_A 0
#define PI_MODEL_B 1
#define PI_MODEL_AP 2
#define PI_MODEL_BP 3
#define PI_MODEL_2 4
#define PI_ALPHA 5
#define PI_MODEL_CM 6
#define PI_MODEL_07 7
#define PI_MODEL_3B 8
#define PI_MODEL_ZERO 9
#define PI_MODEL_CM3 10
#define PI_MODEL_ZERO_W 12
#define PI_MODEL_3BP 13
#define PI_MODEL_3AP 14
#define PI_MODEL_CM3P 16
#define PI_MODEL_4B 17
#define PI_MODEL_ZERO_2W 18
#define PI_MODEL_400 19
#define PI_MODEL_CM4 20

// Pi versions
template <typename T>
consteval T __PI_VERSION_1()
{
    return static_cast<T>(0);
}
template <typename T>
consteval T __PI_VERSION_1_1()
{
    return static_cast<T>(1);
}
template <typename T>
consteval T __PI_VERSION_1_2()
{
    return static_cast<T>(2);
}
template <typename T>
consteval T __PI_VERSION_2()
{
    return static_cast<T>(3);
}

// Pi manufacturers
template <typename T>
consteval T __PI_MAKER_SONY()
{
    return static_cast<T>(0);
}
template <typename T>
consteval T __PI_MAKER_EGOMAN()
{
    return static_cast<T>(1);
}
template <typename T>
consteval T __PI_MAKER_EMBEST()
{
    return static_cast<T>(2);
}
template <typename T>
consteval T __PI_MAKER_UNKNOWN()
{
    return static_cast<T>(3);
}

void __piGpioLayoutOops(const char *why)
{
    fprintf(stderr, "Oops: Unable to determine board revision from /proc/cpuinfo\n");
    fprintf(stderr, " -> %s\n", why);
    fprintf(stderr, " ->  You'd best google the error to find out why.\n");
    // fprintf (stderr, " ->  http://www.raspberrypi.org/phpBB3/viewtopic.php?p=184410#p184410\n") ;
    exit(EXIT_FAILURE);
}

int __piGpioLayout(void)
{
    FILE *cpuFd;
    // FILE *cpuFd = fopen("/proc/cpuinfo", "r");
    char line[120];
    char *c;
    static int gpioLayout = -1;

    if (gpioLayout != -1)
    {
        return gpioLayout;
    }

    if ((cpuFd = fopen("/proc/cpuinfo", "r")) == NULL)
    {
        __piGpioLayoutOops("Unable to open /proc/cpuinfo");
    }

    while (fgets(line, 120, cpuFd) != NULL)
    {
        if (strncmp(line, "Hardware", 8) == 0)
        {
            break;
        }
    }

    rewind(cpuFd);
    while (fgets(line, 120, cpuFd) != NULL)
    {
        if (strncmp(line, "Revision", 8) == 0)
        {
            break;
        }
    }

    fclose(cpuFd);

    if (strncmp(line, "Revision", 8) != 0)
    {
        __piGpioLayoutOops("No \"Revision\" line");
    }

    // Chomp trailing CR/NL

    for (c = &line[strlen(line) - 1]; (*c == '\n') || (*c == '\r'); --c)
    {
        *c = 0;
    }

    // if (wiringPiDebug)
    {
        // printf("piGpioLayout: Revision string: %s\n", line);
    }

    // Scan to the first character of the revision number

    for (c = line; *c; ++c)
    {
        if (*c == ':')
        {
            break;
        }
    }

    if (*c != ':')
    {
        __piGpioLayoutOops("Bogus \"Revision\" line (no colon)");
    }

    // Chomp spaces

    ++c;
    while (isspace(*c))
    {
        ++c;
    }

    if (!isxdigit(*c))
    {
        __piGpioLayoutOops("Bogus \"Revision\" line (no hex digit at start of revision)");
    }

    // Make sure its long enough

    if (strlen(c) < 4)
    {
        __piGpioLayoutOops("Bogus revision line (too small)");
    }

    // Isolate  last 4 characters: (in-case of overvolting or new encoding scheme)

    c = c + strlen(c) - 4;

    // if (wiringPiDebug)
    {
        // printf("piGpioLayout: last4Chars are: \"%s\"\n", c);
    }

    if ((strcmp(c, "0002") == 0) || (strcmp(c, "0003") == 0))
    {
        gpioLayout = 1;
    }
    else
    {
        gpioLayout = 2;
    } // Covers everything else from the B revision 2 to the B+, the Pi v2, v3, zero and CM's.

    // if (wiringPiDebug)
    {
        // printf("piGpioLayoutOops: Returning revision: %d\n", gpioLayout);
    }

    return gpioLayout;
}

std::string generateHeaderTimeString()
{
    const auto end = std::chrono::system_clock::now();
    const std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    const std::string generatedString = "// Header generated on " + std::string(strtok(std::ctime(&end_time), "\n"));
    const std::string lineString = "// ======================================================================== //";
    const size_t toPad = lineString.length() - generatedString.length();

    std::string padString = "//";
    for (size_t i = 0; i < toPad - 2; i++)
    {
        padString = " " + padString;
    }

    return generatedString;
}

template <typename T>
void writeVariableTemplate(std::ofstream &fileName, const std::string_view variableName, const T variable)
{
    fileName << "    template <typename T>" << std::endl;
    fileName << "    [[nodiscard]] inline consteval T " << variableName << "()" << std::endl;
    fileName << "    {" << std::endl;
    fileName << "        return " << variable << ";" << std::endl;
    fileName << "    }" << std::endl;
    fileName << std::endl;
}

// #endif
