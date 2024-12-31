// ======================================================================== //
//                                                                          //
// nullDevice.H                                                             //
//                                                                          //
// ======================================================================== //
// Null device occupying 1 pin by default                                   //
// ======================================================================== //

#ifndef __WIRING_PI_nullDevice_H
#define __WIRING_PI_nullDevice_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"

namespace WiringPi
{
    class nullDevice : public wiringPiNode
    {
    public:
        [[nodiscard]] inline constexpr nullDevice(
            const pin_t pinBaseVal,
            const pin_t pinMode)
            : pinMode_(pinMode),
              pinBase_(pinBaseVal) {};

        ~nullDevice() {};

        [[nodiscard]] inline consteval name_t deviceName() const
        {
            return "Null";
        }

    private:
        const pin_t pinMode_;
        const pin_t pinBase_;
        const pin_t nPins_ = 1;
        const pin_t pinMax_ = pinBase_ + nPins_ - 1;
    };
}

#endif
