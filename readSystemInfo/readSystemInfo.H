// ************************************************************ //
// readSystemInfo.H:                                            //
// This program reads the info about the model of Raspberry Pi  //
// Used to generate the piInfo.H file for the wiringPi library  //
// A bit messy for now, clean up later                          //
// ************************************************************ //

#ifndef __WIRING_PI_readSystemInfo_H
#define __WIRING_PI_readSystemInfo_H

// Header includes
#include "../wiringPi/wiringPiIncludes.H"
#include "../wiringPi/wiringPiDefines.H"

struct piInfoSpec
{
    const WiringPi::Pi::layout::type layout;
    const WiringPi::Pi::model::type model;
    const WiringPi::Pi::version::type version;
    const WiringPi::Pi::memory::type mem;
    const WiringPi::Pi::manufacturer::type manufacturer;
    const WiringPi::Pi::warranty::type warranty;
};

[[nodiscard]] inline WiringPi::Pi::layout::type __piGpioLayout()
{
    // FILE *cpuFd;
    char line[120];
    char *c;

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
        return WiringPi::Pi::layout::PI1_REV1;
    }
    else
    {
        return WiringPi::Pi::layout::DEFAULT;
    }
}

[[nodiscard]] inline piInfoSpec readSysInfo()
{
    char line[120];
    char *c;

    const WiringPi::Pi::layout::type layout = __piGpioLayout();

    {
        FILE *cpuFd = fopen("/proc/cpuinfo", "r");
        assert((cpuFd != NULL));

        while (fgets(line, 120, cpuFd) != NULL)
        {
            if (strncmp(line, "Revision", 8) == 0)
            {
                break;
            }
        }

        fclose(cpuFd);
    }

    assert((strncmp(line, "Revision", 8) == 0));

    // Chomp trailing CR/NL

    for (c = &line[strlen(line) - 1]; (*c == '\n') || (*c == '\r'); --c)
    {
        *c = 0;
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

    // Chomp spaces

    ++c;
    while (isspace(*c))
    {
        ++c;
    }

    // Check for new way:
    const unsigned int revision = static_cast<unsigned int>(strtol(c, NULL, 16));

    if ((revision & (1 << 23)) != 0) // New way
    {
        const WiringPi::Pi::model::type model = static_cast<WiringPi::Pi::model::type>((revision & (0xFF << 4)) >> 4);
        const WiringPi::Pi::version::type rev = static_cast<WiringPi::Pi::version::type>((revision & (0x0F << 0)) >> 0);
        const WiringPi::Pi::memory::type mem = static_cast<WiringPi::Pi::memory::type>((revision & (0x07 << 20)) >> 20);
        const WiringPi::Pi::manufacturer::type manufacturer = static_cast<WiringPi::Pi::manufacturer::type>((revision & (0x0F << 16)) >> 16);
        const WiringPi::Pi::warranty::type warranty = static_cast<WiringPi::Pi::warranty::type>((revision & (0x03 << 24)) != 0);
        return piInfoSpec{layout, model, rev, mem, manufacturer, warranty};
    }
    else // Old way
    {
        // Check that the revision line is valid
        assert((isdigit(*c)));

        // Make sure the revision line is long enough
        assert((!(strlen(c) < 4)));

        // If longer than 4, we'll assume it's been overvolted
        const WiringPi::Pi::warranty::type warranty = static_cast<WiringPi::Pi::warranty::type>(strlen(c) > 4);

        // Extract last 4 characters:
        c = c + strlen(c) - 4;

        // Fill out the replies as appropriate
        if (strcmp(c, "0002") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiB, WiringPi::Pi::version::V1, WiringPi::Pi::memory::MB256, WiringPi::Pi::manufacturer::Egoman, warranty};
        }
        else if (strcmp(c, "0003") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiB, WiringPi::Pi::version::V1_1, WiringPi::Pi::memory::MB256, WiringPi::Pi::manufacturer::Egoman, warranty};
        }
        else if (strcmp(c, "0004") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiB, WiringPi::Pi::version::V1_2, WiringPi::Pi::memory::MB256, WiringPi::Pi::manufacturer::Sony_UK, warranty};
        }
        else if (strcmp(c, "0005") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiB, WiringPi::Pi::version::V1_2, WiringPi::Pi::memory::MB256, WiringPi::Pi::manufacturer::Egoman, warranty};
        }
        else if (strcmp(c, "0006") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiB, WiringPi::Pi::version::V1_2, WiringPi::Pi::memory::MB256, WiringPi::Pi::manufacturer::Egoman, warranty};
        }
        else if (strcmp(c, "0007") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiA, WiringPi::Pi::version::V1_2, WiringPi::Pi::memory::MB256, WiringPi::Pi::manufacturer::Egoman, warranty};
        }
        else if (strcmp(c, "0008") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiA, WiringPi::Pi::version::V1_2, WiringPi::Pi::memory::MB256, WiringPi::Pi::manufacturer::Sony_UK, warranty};
        }
        else if (strcmp(c, "0009") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiA, WiringPi::Pi::version::V1_2, WiringPi::Pi::memory::MB256, WiringPi::Pi::manufacturer::Egoman, warranty};
        }
        else if (strcmp(c, "000d") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiB, WiringPi::Pi::version::V1_2, WiringPi::Pi::memory::MB512, WiringPi::Pi::manufacturer::Egoman, warranty};
        }
        else if (strcmp(c, "000e") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiB, WiringPi::Pi::version::V1_2, WiringPi::Pi::memory::MB512, WiringPi::Pi::manufacturer::Sony_UK, warranty};
        }
        else if (strcmp(c, "000f") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiB, WiringPi::Pi::version::V1_2, WiringPi::Pi::memory::MB512, WiringPi::Pi::manufacturer::Egoman, warranty};
        }
        else if (strcmp(c, "0010") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiBP, WiringPi::Pi::version::V1_2, WiringPi::Pi::memory::MB512, WiringPi::Pi::manufacturer::Sony_UK, warranty};
        }
        else if (strcmp(c, "0013") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiBP, WiringPi::Pi::version::V1_2, WiringPi::Pi::memory::MB512, WiringPi::Pi::manufacturer::Embest, warranty};
        }
        else if (strcmp(c, "0016") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiBP, WiringPi::Pi::version::V1_2, WiringPi::Pi::memory::MB512, WiringPi::Pi::manufacturer::Sony_UK, warranty};
        }
        else if (strcmp(c, "0019") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiBP, WiringPi::Pi::version::V1_2, WiringPi::Pi::memory::MB512, WiringPi::Pi::manufacturer::Egoman, warranty};
        }
        else if (strcmp(c, "0011") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiCM, WiringPi::Pi::version::V1_1, WiringPi::Pi::memory::MB512, WiringPi::Pi::manufacturer::Sony_UK, warranty};
        }
        else if (strcmp(c, "0014") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiCM, WiringPi::Pi::version::V1_1, WiringPi::Pi::memory::MB512, WiringPi::Pi::manufacturer::Embest, warranty};
        }
        else if (strcmp(c, "0017") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiCM, WiringPi::Pi::version::V1_1, WiringPi::Pi::memory::MB512, WiringPi::Pi::manufacturer::Sony_UK, warranty};
        }
        else if (strcmp(c, "001a") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiCM, WiringPi::Pi::version::V1_1, WiringPi::Pi::memory::MB512, WiringPi::Pi::manufacturer::Egoman, warranty};
        }
        else if (strcmp(c, "0012") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiAP, WiringPi::Pi::version::V1_1, WiringPi::Pi::memory::MB256, WiringPi::Pi::manufacturer::Sony_UK, warranty};
        }
        else if (strcmp(c, "0015") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiAP, WiringPi::Pi::version::V1_1, WiringPi::Pi::memory::MB512, WiringPi::Pi::manufacturer::Embest, warranty};
        }
        else if (strcmp(c, "0018") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiAP, WiringPi::Pi::version::V1_1, WiringPi::Pi::memory::MB256, WiringPi::Pi::manufacturer::Sony_UK, warranty};
        }
        else if (strcmp(c, "001b") == 0)
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiAP, WiringPi::Pi::version::V1_1, WiringPi::Pi::memory::MB256, WiringPi::Pi::manufacturer::Egoman, warranty};
        }
        else
        {
            return piInfoSpec{layout, WiringPi::Pi::model::PiA, WiringPi::Pi::version::V1, WiringPi::Pi::memory::MB256, WiringPi::Pi::manufacturer::Sony_UK, warranty};
        }
    }
}

[[nodiscard]] inline constexpr std::string_view modelStrings(const std::size_t model)
{
    if (model < 21)
    {
        constexpr const std::array<std::string_view, 21> arr{
            "Pi::model::PiA",
            "Pi::model::PiB",
            "Pi::model::PiAP",
            "Pi::model::PiBP",
            "Pi::model::Pi2",
            "Pi::model::PiALPHA",
            "Pi::model::PiCM",
            "Pi::model::Pi07",
            "Pi::model::Pi3B",
            "Pi::model::PiZERO",
            "Pi::model::PiCM3",
            "",
            "Pi::model::PiZERO_W",
            "Pi::model::Pi3BP",
            "Pi::model::Pi3AP",
            "",
            "Pi::model::PiCM3P",
            "Pi::model::Pi4B",
            "Pi::model::PiZERO_2W",
            "Pi::model::Pi400",
            "Pi::model::PiCM4"};
        return arr[model];
    }
    else
    {
        return "";
    }
}

[[nodiscard]] inline constexpr std::string_view layoutStrings(const std::size_t layout)
{
    if (layout < 3)
    {
        constexpr const std::array<std::string_view, 3> arr{
            "",
            "Pi::layout::PI1_REV1",
            "Pi::layout::DEFAULT"};
        return arr[layout];
    }
    else
    {
        return "";
    }
}

[[nodiscard]] inline constexpr std::string_view versionStrings(const std::size_t version)
{
    if (version < 6)
    {
        constexpr const std::array<std::string_view, 6> arr{
            "Pi::version::V1",
            "Pi::version::V1_1",
            "Pi::version::V1_2",
            "Pi::version::V2",
            "Pi::version::V1_4",
            "Pi::version::V1_5"};
        return arr[version];
    }
    else
    {
        return "";
    }
}

[[nodiscard]] inline constexpr std::string_view memoryStrings(const std::size_t memory)
{
    if (memory < 6)
    {
        constexpr const std::array<std::string_view, 6> arr{
            "Pi::memory::MB256",
            "Pi::memory::MB512",
            "Pi::memory::GB1",
            "Pi::memory::GB2",
            "Pi::memory::GB4",
            "Pi::memory::GB8"};
        return arr[memory];
    }
    else
    {
        return "";
    }
}

[[nodiscard]] inline constexpr std::string_view manufacturerStrings(const std::size_t manufacturer)
{
    if (manufacturer < 6)
    {
        constexpr const std::array<std::string_view, 6> arr{
            "Pi::manufacturer::Sony_UK",
            "Pi::manufacturer::Egoman",
            "Pi::manufacturer::Embest",
            "Pi::manufacturer::Sony_Japan",
            "Pi::manufacturer::Embest_2",
            "Pi::manufacturer::Stadium"};
        return arr[manufacturer];
    }
    else
    {
        return "";
    }
}

[[nodiscard]] inline constexpr std::string_view warrantyStrings(const std::size_t warranty)
{
    if (warranty < 2)
    {
        constexpr const std::array<std::string_view, 2> arr{
            "Pi::warranty::valid",
            "Pi::warranty::voided"};
        return arr[warranty];
    }
    else
    {
        return "";
    }
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
void writeVariableTemplate(std::ofstream &fileName, const std::string_view variableType, const std::string_view variableName, const T variable)
{
    fileName << "    [[nodiscard]] inline consteval " << variableType << " " << variableName << "()" << std::endl;
    fileName << "    {" << std::endl;
    fileName << "        return " << variable << ";" << std::endl;
    fileName << "    }" << std::endl;
    // fileName << std::endl;
}

#endif
