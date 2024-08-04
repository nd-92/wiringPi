#ifndef __WIRING_PI_NULL_DEVICE_H
#define __WIRING_PI_NULL_DEVICE_H

#include "wiringPiNode.H"

// Null device occupying 1 pin by default

namespace WiringPi
{
    class nullDevice : public wiringPiNode
    {
    public:
        [[nodiscard]] inline constexpr nullDevice(
            const size_t pinBaseVal,
            const size_t pinMode)
            : pinMode_(pinMode),
              pinBase_(pinBaseVal) {};

        ~nullDevice() {};

        [[nodiscard]] inline consteval std::string_view deviceName() const
        {
            return "Null";
        }

    private:
        const size_t pinMode_;
        const size_t pinBase_;
        const size_t numPins_ = 1;
        const size_t pinMax_ = pinBase_ + numPins_ - 1;
    };
}

#endif