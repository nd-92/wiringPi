// ************************************************************ //
//  readCpuInfo.C:                                              //
//	This program reads the info about the model of Raspberry Pi //
// ************************************************************ //

// Header includes
#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>

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

void piGpioLayoutOops(const char *why)
{
    fprintf(stderr, "Oops: Unable to determine board revision from /proc/cpuinfo\n");
    fprintf(stderr, " -> %s\n", why);
    fprintf(stderr, " ->  You'd best google the error to find out why.\n");
    // fprintf (stderr, " ->  http://www.raspberrypi.org/phpBB3/viewtopic.php?p=184410#p184410\n") ;
    exit(EXIT_FAILURE);
}

int piGpioLayout(void)
{
    // FILE *cpuFd;
    FILE *cpuFd = fopen("/proc/cpuinfo", "r");
    char line[120];
    char *c;
    static int gpioLayout = -1;
    // constexpr int gpioLayout = -1;

    if (gpioLayout != -1) // No point checking twice
    {
        return gpioLayout;
    }

    if ((cpuFd = fopen("/proc/cpuinfo", "r")) == NULL)
    {
        piGpioLayoutOops("Unable to open /proc/cpuinfo");
    }

    // Start by looking for the Architecture to make sure we're really running
    //	on a Pi. I'm getting fed-up with people whinging at me because
    //	they can't get it to work on weirdFruitPi boards...

    while (fgets(line, 120, cpuFd) != NULL)
    {
        if (strncmp(line, "Hardware", 8) == 0)
        {
            break;
        }
    }

    // if (strncmp(line, "Hardware", 8) != 0)
    //{
    //     piGpioLayoutOops("No \"Hardware\" line");
    // }

    // if (wiringPiDebug)
    {
        // printf("piGpioLayout: Hardware: %s\n", line);
    }

    // See if it's BCM2708 or BCM2709 or the new BCM2835.

    // OK. As of Kernel 4.8,  we have BCM2835 only, regardless of model.
    //	However I still want to check because it will trap the cheapskates and rip-
    //	off merchants who want to use wiringPi on non-Raspberry Pi platforms - which
    //	I do not support so don't email me your bleating whinges about anything
    //	other than a genuine Raspberry Pi.

    // Actually... That has caused me more than 10,000 emails so-far. Mosty by
    //	people who think they know better by creating a statically linked
    //	version that will not run with a new 4.9 kernel. I utterly hate and
    //	despise those people.
    //
    //	I also get bleats from people running other than Raspbian with another
    //	distros compiled kernel rather than a foundation compiled kernel, so
    //	this might actually help them. It might not - I only have the capacity
    //	to support Raspbian.
    //
    //	However, I've decided to leave this check out and rely purely on the
    //	Revision: line for now. It will not work on a non-pi hardware or weird
    //	kernels that don't give you a suitable revision line.

    // So - we're Probably on a Raspberry Pi. Check the revision field for the real
    //	hardware type
    //	In-future, I ought to use the device tree as there are now Pi entries in
    //	/proc/device-tree/ ...
    //	but I'll leave that for the next revision. Or the next.

    // Isolate the Revision line

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
        piGpioLayoutOops("No \"Revision\" line");
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
        piGpioLayoutOops("Bogus \"Revision\" line (no colon)");
    }

    // Chomp spaces

    ++c;
    while (isspace(*c))
    {
        ++c;
    }

    if (!isxdigit(*c))
    {
        piGpioLayoutOops("Bogus \"Revision\" line (no hex digit at start of revision)");
    }

    // Make sure its long enough

    if (strlen(c) < 4)
    {
        piGpioLayoutOops("Bogus revision line (too small)");
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

int main()
{
    FILE *cpuFd;
    char line[120];
    char *c;
    unsigned int revision;
    // int bRev, bType, bProc, bMfg, bMem, bWarranty;
    int bRev, bType, bMfg, bMem, bWarranty;

    const int layout = piGpioLayout();

    //	Will deal with the properly later on - for now, lets just get it going...
    //  unsigned int modelNum ;

    // (void)piGpioLayout(); // Call this first to make sure all's OK. Don't care about the result.

    if ((cpuFd = fopen("/proc/cpuinfo", "r")) == NULL)
    {
        // piGpioLayoutOops("Unable to open /proc/cpuinfo");
    }

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
        // piGpioLayoutOops("No \"Revision\" line");
    }

    // Chomp trailing CR/NL

    for (c = &line[strlen(line) - 1]; (*c == '\n') || (*c == '\r'); --c)
    {
        *c = 0;
    }

    // if (wiringPiDebug)
    {
        // printf("piBoardId: Revision string: %s\n", line);
    }

    // Need to work out if it's using the new or old encoding scheme:

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
        // piGpioLayoutOops("Bogus \"Revision\" line (no colon)");
    }

    // Chomp spaces

    ++c;
    while (isspace(*c))
    {
        ++c;
    }

    if (!isxdigit(*c))
    {
        // piGpioLayoutOops("Bogus \"Revision\" line (no hex digit at start of revision)");
    }

    revision = (unsigned int)strtol(c, NULL, 16); // Hex number with no leading 0x

    // Check for new way:

    int model, rev, mem, maker, warranty;

    if ((revision & (1 << 23)) != 0) // New way
    {
        // if (wiringPiDebug)
        {
            // printf("piBoardId: New Way: revision is: %08X\n", revision);
        }

        bRev = (revision & (0x0F << 0)) >> 0;
        bType = (revision & (0xFF << 4)) >> 4;
        // bProc = (revision & (0x0F << 12)) >> 12; // Not used for now.
        bMfg = (revision & (0x0F << 16)) >> 16;
        bMem = (revision & (0x07 << 20)) >> 20;
        bWarranty = (revision & (0x03 << 24)) != 0;

        model = bType;
        rev = bRev;
        mem = bMem;
        maker = bMfg;
        warranty = bWarranty;

        // if (wiringPiDebug)
        {
            // printf(
            //     "piBoardId: rev: %d, type: %d, proc: %d, mfg: %d, mem: %d, warranty: %d\n",
            //     bRev, bType, bProc, bMfg, bMem, bWarranty);
        }
    }
    else // Old way
    {
        // if (wiringPiDebug)
        {
            printf("piBoardId: Old Way: revision is: %s\n", c);
        }

        if (!isdigit(*c))
        {
            // piGpioLayoutOops("Bogus \"Revision\" line (no digit at start of revision)");
        }

        // Make sure its long enough

        if (strlen(c) < 4)
        {
            // piGpioLayoutOops("Bogus \"Revision\" line (not long enough)");
        }

        // If longer than 4, we'll assume it's been overvolted

        warranty = strlen(c) > 4;

        // Extract last 4 characters:

        c = c + strlen(c) - 4;

        // Fill out the replys as appropriate

        if (strcmp(c, "0002") == 0)
        {
            model = PI_MODEL_B;
            rev = PI_VERSION_1<int>();
            mem = 0;
            maker = PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0003") == 0)
        {
            model = PI_MODEL_B;
            rev = PI_VERSION_1_1<int>();
            mem = 0;
            maker = PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0004") == 0)
        {
            model = PI_MODEL_B;
            rev = PI_VERSION_1_2<int>();
            mem = 0;
            maker = PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "0005") == 0)
        {
            model = PI_MODEL_B;
            rev = PI_VERSION_1_2<int>();
            mem = 0;
            maker = PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0006") == 0)
        {
            model = PI_MODEL_B;
            rev = PI_VERSION_1_2<int>();
            mem = 0;
            maker = PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0007") == 0)
        {
            model = PI_MODEL_A;
            rev = PI_VERSION_1_2<int>();
            mem = 0;
            maker = PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0008") == 0)
        {
            model = PI_MODEL_A;
            rev = PI_VERSION_1_2<int>();
            mem = 0;
            maker = PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "0009") == 0)
        {
            model = PI_MODEL_A;
            rev = PI_VERSION_1_2<int>();
            mem = 0;
            maker = PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "000d") == 0)
        {
            model = PI_MODEL_B;
            rev = PI_VERSION_1_2<int>();
            mem = 1;
            maker = PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "000e") == 0)
        {
            model = PI_MODEL_B;
            rev = PI_VERSION_1_2<int>();
            mem = 1;
            maker = PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "000f") == 0)
        {
            model = PI_MODEL_B;
            rev = PI_VERSION_1_2<int>();
            mem = 1;
            maker = PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0010") == 0)
        {
            model = PI_MODEL_BP;
            rev = PI_VERSION_1_2<int>();
            mem = 1;
            maker = PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "0013") == 0)
        {
            model = PI_MODEL_BP;
            rev = PI_VERSION_1_2<int>();
            mem = 1;
            maker = PI_MAKER_EMBEST<int>();
        }
        else if (strcmp(c, "0016") == 0)
        {
            model = PI_MODEL_BP;
            rev = PI_VERSION_1_2<int>();
            mem = 1;
            maker = PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "0019") == 0)
        {
            model = PI_MODEL_BP;
            rev = PI_VERSION_1_2<int>();
            mem = 1;
            maker = PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0011") == 0)
        {
            model = PI_MODEL_CM;
            rev = PI_VERSION_1_1<int>();
            mem = 1;
            maker = PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "0014") == 0)
        {
            model = PI_MODEL_CM;
            rev = PI_VERSION_1_1<int>();
            mem = 1;
            maker = PI_MAKER_EMBEST<int>();
        }
        else if (strcmp(c, "0017") == 0)
        {
            model = PI_MODEL_CM;
            rev = PI_VERSION_1_1<int>();
            mem = 1;
            maker = PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "001a") == 0)
        {
            model = PI_MODEL_CM;
            rev = PI_VERSION_1_1<int>();
            mem = 1;
            maker = PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0012") == 0)
        {
            model = PI_MODEL_AP;
            rev = PI_VERSION_1_1<int>();
            mem = 0;
            maker = PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "0015") == 0)
        {
            model = PI_MODEL_AP;
            rev = PI_VERSION_1_1<int>();
            mem = 1;
            maker = PI_MAKER_EMBEST<int>();
        }
        else if (strcmp(c, "0018") == 0)
        {
            model = PI_MODEL_AP;
            rev = PI_VERSION_1_1<int>();
            mem = 0;
            maker = PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "001b") == 0)
        {
            model = PI_MODEL_AP;
            rev = PI_VERSION_1_1<int>();
            mem = 0;
            maker = PI_MAKER_EGOMAN<int>();
        }
        else
        {
            model = 0;
            rev = 0;
            mem = 0;
            maker = 0;
        }
    }

    bool usingGpioMem = false;
    constexpr const char *gpiomem_global = "/dev/mem";
    constexpr const char *gpiomem_BCM = "/dev/gpiomem";
    constexpr const char *gpiomemGlobal = gpiomem_global;
    constexpr const char *gpiomemModule = gpiomem_BCM;
    int fd;

    if ((fd = open(gpiomemGlobal, O_RDWR | O_SYNC | O_CLOEXEC)) < 0)
    {

        if ((fd = open(gpiomemModule, O_RDWR | O_SYNC | O_CLOEXEC)) >= 0) // We're using gpiomem
        {
            usingGpioMem = true;
        }
    }

    std::ofstream MyFile("piInfo.H");

    MyFile << "#ifndef __PI_INFO_H" << std::endl;
    MyFile << "#define __PI_INFO_H" << std::endl;
    MyFile << std::endl;

    MyFile << "namespace WiringPi" << std::endl;
    MyFile << "{" << std::endl;

    // Write to the file
    MyFile << "    template <typename T>" << std::endl;
    MyFile << "    consteval T GPIO_LAYOUT()" << std::endl;
    MyFile << "    {" << std::endl;
    MyFile << "        return " << layout << ";" << std::endl;
    MyFile << "    }" << std::endl;
    MyFile << std::endl;

    MyFile << "    template <typename T>" << std::endl;
    MyFile << "    consteval T PI_MODEL()" << std::endl;
    MyFile << "    {" << std::endl;
    MyFile << "        return " << model << ";" << std::endl;
    MyFile << "    }" << std::endl;
    MyFile << std::endl;

    MyFile << "    template <typename T>" << std::endl;
    MyFile << "    consteval T PI_REV()" << std::endl;
    MyFile << "    {" << std::endl;
    MyFile << "        return " << rev << ";" << std::endl;
    MyFile << "    }" << std::endl;
    MyFile << std::endl;

    MyFile << "    template <typename T>" << std::endl;
    MyFile << "    consteval T PI_MEM()" << std::endl;
    MyFile << "    {" << std::endl;
    MyFile << "        return " << mem << ";" << std::endl;
    MyFile << "    }" << std::endl;
    MyFile << std::endl;

    MyFile << "    template <typename T>" << std::endl;
    MyFile << "    consteval T PI_MAKER()" << std::endl;
    MyFile << "    {" << std::endl;
    MyFile << "        return " << maker << ";" << std::endl;
    MyFile << "    }" << std::endl;
    MyFile << std::endl;

    MyFile << "    template <typename T>" << std::endl;
    MyFile << "    consteval T PI_WARRANTY()" << std::endl;
    MyFile << "    {" << std::endl;
    MyFile << "        return " << warranty << ";" << std::endl;
    MyFile << "    }" << std::endl;
    MyFile << std::endl;

    MyFile << "    template <typename T>" << std::endl;
    MyFile << "    consteval T USING_GPIOMEM()" << std::endl;
    MyFile << "    {" << std::endl;
    MyFile << "        return " << usingGpioMem << ";" << std::endl;
    MyFile << "    }" << std::endl;
    MyFile << std::endl;

    MyFile << "}" << std::endl;

    MyFile << "#endif" << std::endl;

    // Close the file
    MyFile.close();
}