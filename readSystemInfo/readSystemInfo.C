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
    const piInfoSpec piInfo = readSysInfo();

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
    writeVariableTemplate(outFile, "Pi::layout::type", "myLayout", layoutStrings(piInfo.layout));
    outFile << std::endl;
    writeVariableTemplate(outFile, "Pi::model::type", "myModel", modelStrings(piInfo.model));
    outFile << std::endl;
    writeVariableTemplate(outFile, "Pi::version::type", "myVersion", versionStrings(piInfo.version));
    outFile << std::endl;
    writeVariableTemplate(outFile, "Pi::memory::type", "myMemory", memoryStrings(piInfo.mem));
    outFile << std::endl;
    writeVariableTemplate(outFile, "Pi::manufacturer::type", "myManufacturer", manufacturerStrings(piInfo.manufacturer));
    outFile << std::endl;
    writeVariableTemplate(outFile, "Pi::warranty::type", "myWarranty", warrantyStrings(piInfo.warranty));

    outFile << "}" << std::endl;
    outFile << std::endl;
    outFile << "#endif" << std::endl;

    // Close the file
    outFile.close();

    return 0;
}