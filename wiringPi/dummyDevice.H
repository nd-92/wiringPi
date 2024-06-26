#ifndef __WIRING_PI_DUMMY_DEVICE_H
#define __WIRING_PI_DUMMY_DEVICE_H

#include "wiringPiNode.H"

// Dummy device occupying 8 pins by default

namespace WiringPi
{
    class dummyDevice : public wiringPiNode
    {
    public:
        constexpr dummyDevice(
            const size_t pinBaseVal)
            : pinBase_(pinBaseVal){};

        inline gpio_t digitalRead() const
        {
            // std::cout << deviceName() << " digitalRead returning LOW" << std::endl;
            return LOW<gpio_t>();
        }

        inline std::string deviceName() const
        {
            return "dummyDevice";
        }

        inline constexpr size_t pinBase() const
        {
            return pinBase_;
        }

        inline constexpr size_t numPins() const
        {
            return numPins_;
        }

        inline constexpr size_t pinMax() const
        {
            return pinMax_;
        }

    private:
        const size_t pinBase_;
        const size_t numPins_ = 8;
        const size_t pinMax_ = pinBase_ + numPins_ - 1;
    };
}

#endif