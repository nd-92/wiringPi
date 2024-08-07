#ifndef __WIRING_PI_NULL_DEVICE_H
#define __WIRING_PI_NULL_DEVICE_H

// Null device occupying 1 pin by default

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

        [[nodiscard]] inline consteval std::string_view deviceName() const
        {
            return "Null";
        }

    private:
        const pin_t pinMode_;
        const pin_t pinBase_;
        const pin_t numPins_ = 1;
        const pin_t pinMax_ = pinBase_ + numPins_ - 1;
    };
}

#endif