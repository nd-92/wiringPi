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

template <typename T>
consteval T __WPI_MODE_PINS()
{
    return 0;
}
template <typename T>
consteval T __WPI_MODE_GPIO()
{
    return 1;
}
template <typename T>
consteval T __WPI_MODE_GPIO_SYS()
{
    return 2;
}
template <typename T>
consteval T __WPI_MODE_PHYS()
{
    return 3;
}
template <typename T>
consteval T __WPI_MODE_PIFACE()
{
    return 4;
}
template <typename T>
consteval T __WPI_MODE_UNINITIALISED()
{
    return -1;
}

template <typename T>
consteval T __PI_MODEL_A()
{
    return 0;
}
template <typename T>
consteval T __PI_MODEL_B()
{
    return 1;
}
template <typename T>
consteval T __PI_MODEL_AP()
{
    return 2;
}
template <typename T>
consteval T __PI_MODEL_BP()
{
    return 3;
}
template <typename T>
consteval T __PI_MODEL_2()
{
    return 4;
}
template <typename T>
consteval T __PI_ALPHA()
{
    return 5;
}
template <typename T>
consteval T __PI_MODEL_CM()
{
    return 6;
}
template <typename T>
consteval T __PI_MODEL_07()
{
    return 7;
}
template <typename T>
consteval T __PI_MODEL_3B()
{
    return 8;
}
template <typename T>
consteval T __PI_MODEL_ZERO()
{
    return 9;
}
template <typename T>
consteval T __PI_MODEL_CM3()
{
    return 10;
}
template <typename T>
consteval T __PI_MODEL_ZERO_W()
{
    return 12;
}
template <typename T>
consteval T __PI_MODEL_3BP()
{
    return 13;
}
template <typename T>
consteval T __PI_MODEL_3AP()
{
    return 14;
}
template <typename T>
consteval T __PI_MODEL_CM3P()
{
    return 16;
}
template <typename T>
consteval T __PI_MODEL_4B()
{
    return 17;
}
template <typename T>
consteval T __PI_MODEL_ZERO_2W()
{
    return 18;
}
template <typename T>
consteval T __PI_MODEL_400()
{
    return 19;
}
template <typename T>
consteval T __PI_MODEL_CM4()
{
    return 20;
}

// Pi versions
template <typename T>
consteval T __PI_VERSION_1()
{
    return 0;
}
template <typename T>
consteval T __PI_VERSION_1_1()
{
    return 1;
}
template <typename T>
consteval T __PI_VERSION_1_2()
{
    return 2;
}
template <typename T>
consteval T __PI_VERSION_2()
{
    return 3;
}

// Pi manufacturers
template <typename T>
consteval T __PI_MAKER_SONY()
{
    return 0;
}
template <typename T>
consteval T __PI_MAKER_EGOMAN()
{
    return 1;
}
template <typename T>
consteval T __PI_MAKER_EMBEST()
{
    return 2;
}
template <typename T>
consteval T __PI_MAKER_UNKNOWN()
{
    return 3;
}

int __piGpioLayout(void)
{
    // FILE *cpuFd;
    char line[120];
    char *c;
    int gpioLayout = -1;

    // Try to open /proc/cpuinfo
    FILE *cpuFd = fopen("/proc/cpuinfo", "r");

    assert((cpuFd != NULL));

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

    assert((strncmp(line, "Revision", 8) == 0));

    // Chomp trailing CR/NL
    for (c = &line[strlen(line) - 1]; (*c == '\n') || (*c == '\r'); --c)
    {
        *c = 0;
    }

    // Scan to the first character of the revision number
    for (c = line; *c; ++c)
    {
        if (*c == ':')
        {
            break;
        }
    }

    assert((!(*c != ':')));

    // Chomp spaces
    ++c;
    while (isspace(*c))
    {
        ++c;
    }

    assert((isxdigit(*c)));

    assert((!(strlen(c) < 4)));

    c = c + strlen(c) - 4;

    if ((strcmp(c, "0002") == 0) || (strcmp(c, "0003") == 0))
    {
        gpioLayout = 1;
    }
    else
    {
        gpioLayout = 2;
    }

    // Check that gpioLayout is not -1
    assert((gpioLayout != -1));

    // Check that it's either 1 or 2
    assert((gpioLayout == 1 || gpioLayout == 2));

    return gpioLayout;
}

[[nodiscard]] const std::string generateHeaderTimeString()
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

    return generatedString + padString;
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
