// ************************************************************ //
// readSystemInfo.C:                                            //
// This program reads the info about the model of Raspberry Pi  //
// Used to generate the piInfo.H file for the wiringPi library  //
// A bit messy for now, clean up later                          //
// ************************************************************ //

// Include header
#include "readSystemInfo.H"

int main()
{
    // FILE *cpuFd;
    char line[120];
    char *c;
    unsigned int revision;
    // int bRev, bType, bProc, bMfg, bMem, bWarranty;
    int bRev, bType, bMfg, bMem, bWarranty;

    const int layout = __piGpioLayout();

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

    if (strncmp(line, "Revision", 8) != 0)
    {
        // piGpioLayoutOops("No \"Revision\" line");
    }

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

    revision = (unsigned int)strtol(c, NULL, 16); // Hex number with no leading 0x

    // Check for new way:

    int model, rev, mem, maker, warranty;

    if ((revision & (1 << 23)) != 0) // New way
    {
        bRev = (revision & (0x0F << 0)) >> 0;
        bType = (revision & (0xFF << 4)) >> 4;
        bMfg = (revision & (0x0F << 16)) >> 16;
        bMem = (revision & (0x07 << 20)) >> 20;
        bWarranty = (revision & (0x03 << 24)) != 0;

        model = bType;
        rev = bRev;
        mem = bMem;
        maker = bMfg;
        warranty = bWarranty;
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
            rev = __PI_VERSION_1<int>();
            mem = 0;
            maker = __PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0003") == 0)
        {
            model = PI_MODEL_B;
            rev = __PI_VERSION_1_1<int>();
            mem = 0;
            maker = __PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0004") == 0)
        {
            model = PI_MODEL_B;
            rev = __PI_VERSION_1_2<int>();
            mem = 0;
            maker = __PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "0005") == 0)
        {
            model = PI_MODEL_B;
            rev = __PI_VERSION_1_2<int>();
            mem = 0;
            maker = __PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0006") == 0)
        {
            model = PI_MODEL_B;
            rev = __PI_VERSION_1_2<int>();
            mem = 0;
            maker = __PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0007") == 0)
        {
            model = PI_MODEL_A;
            rev = __PI_VERSION_1_2<int>();
            mem = 0;
            maker = __PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0008") == 0)
        {
            model = PI_MODEL_A;
            rev = __PI_VERSION_1_2<int>();
            mem = 0;
            maker = __PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "0009") == 0)
        {
            model = PI_MODEL_A;
            rev = __PI_VERSION_1_2<int>();
            mem = 0;
            maker = __PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "000d") == 0)
        {
            model = PI_MODEL_B;
            rev = __PI_VERSION_1_2<int>();
            mem = 1;
            maker = __PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "000e") == 0)
        {
            model = PI_MODEL_B;
            rev = __PI_VERSION_1_2<int>();
            mem = 1;
            maker = __PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "000f") == 0)
        {
            model = PI_MODEL_B;
            rev = __PI_VERSION_1_2<int>();
            mem = 1;
            maker = __PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0010") == 0)
        {
            model = PI_MODEL_BP;
            rev = __PI_VERSION_1_2<int>();
            mem = 1;
            maker = __PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "0013") == 0)
        {
            model = PI_MODEL_BP;
            rev = __PI_VERSION_1_2<int>();
            mem = 1;
            maker = __PI_MAKER_EMBEST<int>();
        }
        else if (strcmp(c, "0016") == 0)
        {
            model = PI_MODEL_BP;
            rev = __PI_VERSION_1_2<int>();
            mem = 1;
            maker = __PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "0019") == 0)
        {
            model = PI_MODEL_BP;
            rev = __PI_VERSION_1_2<int>();
            mem = 1;
            maker = __PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0011") == 0)
        {
            model = PI_MODEL_CM;
            rev = __PI_VERSION_1_1<int>();
            mem = 1;
            maker = __PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "0014") == 0)
        {
            model = PI_MODEL_CM;
            rev = __PI_VERSION_1_1<int>();
            mem = 1;
            maker = __PI_MAKER_EMBEST<int>();
        }
        else if (strcmp(c, "0017") == 0)
        {
            model = PI_MODEL_CM;
            rev = __PI_VERSION_1_1<int>();
            mem = 1;
            maker = __PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "001a") == 0)
        {
            model = PI_MODEL_CM;
            rev = __PI_VERSION_1_1<int>();
            mem = 1;
            maker = __PI_MAKER_EGOMAN<int>();
        }
        else if (strcmp(c, "0012") == 0)
        {
            model = PI_MODEL_AP;
            rev = __PI_VERSION_1_1<int>();
            mem = 0;
            maker = __PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "0015") == 0)
        {
            model = PI_MODEL_AP;
            rev = __PI_VERSION_1_1<int>();
            mem = 1;
            maker = __PI_MAKER_EMBEST<int>();
        }
        else if (strcmp(c, "0018") == 0)
        {
            model = PI_MODEL_AP;
            rev = __PI_VERSION_1_1<int>();
            mem = 0;
            maker = __PI_MAKER_SONY<int>();
        }
        else if (strcmp(c, "001b") == 0)
        {
            model = PI_MODEL_AP;
            rev = __PI_VERSION_1_1<int>();
            mem = 0;
            maker = __PI_MAKER_EGOMAN<int>();
        }
        else
        {
            model = 0;
            rev = 0;
            mem = 0;
            maker = 0;
        }
    }

    std::ofstream outFile("piInfo.H");

    outFile << "// ======================================================================== //" << std::endl;
    outFile << "//                                                                          //" << std::endl;
    outFile << "// piInfo.H                                                                 //" << std::endl;
    outFile << "//                                                                          //" << std::endl;
    outFile << "// ======================================================================== //" << std::endl;
    outFile << "// Hardware info about your Raspberry Pi necessary for the wiringPi library //" << std::endl;
    outFile << generateHeaderTimeString() << std::endl;
    outFile << "// ======================================================================== //" << std::endl;
    outFile << std::endl;
    outFile << "#ifndef __WIRING_PI_piInfo_H" << std::endl;
    outFile << "#define __WIRING_PI_piInfo_H" << std::endl;
    outFile << std::endl;

    outFile << "namespace WiringPi" << std::endl;
    outFile << "{" << std::endl;

    // Write to the file
    writeVariableTemplate(outFile, "GPIO_LAYOUT", layout);
    writeVariableTemplate(outFile, "PI_MODEL", model);
    writeVariableTemplate(outFile, "PI_REV", rev);
    writeVariableTemplate(outFile, "PI_MEM", mem);
    writeVariableTemplate(outFile, "PI_MAKER", maker);
    writeVariableTemplate(outFile, "PI_WARRANTY", warranty);

    outFile << "}" << std::endl;
    outFile << std::endl;
    outFile << "#endif" << std::endl;

    // Close the file
    outFile.close();
}