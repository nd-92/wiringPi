// ======================================================================== //
//                                                                          //
// version.H                                                                //
//                                                                          //
// ======================================================================== //
// Software version info about the wiringPi library                         //
// ======================================================================== //

#ifndef __WIRING_PI_version_H
#define __WIRING_PI_version_H

namespace WiringPi
{
    [[nodiscard]] consteval const std::string_view VERSION()
    {
        return "2.70";
    }

    template <typename T>
    [[nodiscard]] consteval int VERSION_MAJOR()
    {
        return 2;
    }

    template <typename T>
    [[nodiscard]] consteval int VERSION_MINOR()
    {
        return 70;
    }
}

#endif
