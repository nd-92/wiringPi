// ======================================================================== //
//                                                                          //
// drcNet.H                                                                 //
//                                                                          //
// ======================================================================== //
// Extend wiringPi with the DRC Network protocol (e.g. to another Pi)       //
// ======================================================================== //

#ifndef __WIRING_PI_drcNet_H
#define __WIRING_PI_drcNet_H

#include "wiringPi.H"

namespace WiringPi
{
    class drcNet : public wiringPiNode
    {
    public:
        template <const pin_t pinBase, const pin_t nPins_>
        [[nodiscard]] drcNet(
            const char *ipAddress,
            const char *port,
            const char *password){};

    private:
        static constexpr const pin_t pinBase_ = pinBase;
        static constexpr const pin_t nPins_ = nPins;
    };
}

#endif
