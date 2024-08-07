#ifndef __WIRING_PI_DUMMY_DEVICE_H
#define __WIRING_PI_DUMMY_DEVICE_H

// #include "wiringPiNode.H"

// Dummy device occupying 8 pins by default

namespace WiringPi
{
    class dummyDevice : public wiringPiNode
    {
    public:
        [[nodiscard]] inline constexpr dummyDevice(
            const pin_t pinBaseVal,
            const pin_t pinMode)
            : pinBase_(pinBaseVal)
        {
            // Set the mode of the pin
            pinMode_ = pinMode;
        };

        [[nodiscard]] inline gpio_t digitalRead() const
        {
            // std::cout << deviceName() << " digitalRead returning LOW" << std::endl;
            return LOW<gpio_t>();
        }

        [[nodiscard]] inline consteval std::string_view deviceName() const
        {
            return "dummyDevice";
        }

        [[nodiscard]] inline constexpr pin_t pinBase() const
        {
            return pinBase_;
        }

        [[nodiscard]] inline constexpr pin_t numPins() const
        {
            return numPins_;
        }

        [[nodiscard]] inline constexpr pin_t pinMax() const
        {
            return pinMax_;
        }

    private:
        pin_t pinMode_;
        const pin_t pinBase_;
        const pin_t numPins_ = 8;
        const pin_t pinMax_ = pinBase_ + numPins_ - 1;
    };
}

#endif