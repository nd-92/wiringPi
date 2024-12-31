// ======================================================================== //
//                                                                          //
// mcp23008.H                                                               //
//                                                                          //
// ======================================================================== //
// Extend wiringPi with the MCP 23008 I2C GPIO expander chip                //
// ======================================================================== //

#ifndef __WIRING_PI_nullDevice_H
#define __WIRING_PI_nullDevice_H

#include "i2c.H"

namespace WiringPi
{
    template <typename T>
    [[nodiscard]] inline consteval T MCP23x08_IODIR()
    {
        return 0x00;
    }

    template <const pin_t pinBase, const int layout, const pin_t devID>
    class mcp23008 : public wiringPiNode
    {
    public:
        [[nodiscard]] mcp23008() {};

        ~mcp23008() {};

        [[nodiscard]] inline consteval name_t deviceName() const
        {
            return "mcp23008";
        }

        template <const pin_t pin_, const gpio_t mode_>
        inline void pinMode(const pin_constant<pin_> pin, const gpio_constant<mode_> mode) const
        {
            static_assert(pin_ < 8, "Device MCP23008 has only 8 pins");
            constexpr const pin_t mask = 1 << (pin - pinBase_);
            I2C_t old = i2c_.read8(MCP23x08_IODIR<uint8_t>());

            if constexpr (mode() == output())
            {
                old &= (~mask);
            }
            else
            {
                old |= mask;
            }

            i2c_.write8(MCP23x08_IODIR<uint8_t>(), old);
        }

        [[nodiscard]] inline int fd() const
        {
            return i2c_.fd();
        }

    private:
        const i2c<layout, devID> i2c_;
        static constexpr const pin_t pinBase_ = pinBase;
        static constexpr const pin_t nPins_ = 8;
    };
}

#endif