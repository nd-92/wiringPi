// ======================================================================== //
//                                                                          //
// nullDevice.H                                                             //
//                                                                          //
// ======================================================================== //
// Dummy device occupying 8 pins by default                                 //
// ======================================================================== //

#ifndef __WIRING_PI_dummyDevice_H
#define __WIRING_PI_dummyDevice_H

namespace WiringPi
{
    class dummyDevice : public wiringPiNode
    {
    public:
        [[nodiscard]] inline constexpr dummyDevice(
            const pin_t pinMode,
            const pin_t pinBase)
            : pinMode_(pinMode),
              pinBase_(pinBase) {};

        [[nodiscard]] inline consteval gpio_t digitalRead() const
        {
            return 0;
        }

        [[nodiscard]] inline consteval std::string_view deviceName() const
        {
            return "dummyDevice";
        }

        [[nodiscard]] inline consteval pin_t pinBase() const
        {
            return pinBase_;
        }

        [[nodiscard]] inline consteval pin_t numPins() const
        {
            return numPins_;
        }

        [[nodiscard]] inline consteval pin_t pinMax() const
        {
            return pinMax_;
        }

    private:
        const pin_t pinMode_;
        const pin_t pinBase_;
        const pin_t numPins_ = 8;
        const pin_t pinMax_ = pinBase_ + numPins_ - 1;
    };
}

#endif
