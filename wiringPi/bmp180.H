// ======================================================================== //
//                                                                          //
// mcp23008.H                                                               //
//                                                                          //
// ======================================================================== //
// Extend wiringPi with the BMP180 I2C Pressure and Temperature	sensor      //
// ======================================================================== //

#ifndef __WIRING_PI_nullDevice_H
#define __WIRING_PI_nullDevice_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"
#include "i2c.H"

namespace WiringPi
{
    template <const pin_t pinBase, const Pi::models::type Model, const Pi::layout::type Layout, const wiringPiModes::type wiringPiMode>
    class bmp180 : public wiringPiNode
    {
    public:
        // Default constructor: take a reference to the wiringPi object
        [[nodiscard]] bmp180(wiringPi<Model, Layout, wiringPiMode> &RaspberryPi) : i2c_(i2c<0x77, Model, Layout, wiringPiMode>(RaspberryPi)) {};

        ~bmp180() {};

        [[nodiscard]] inline consteval name_t deviceName() const
        {
            return "bmp180";
        }

        // Write to a fake register to represent the height above sea level
        template <const pin_t pin_>
        inline void analogWrite(const pin_constant<pin_> pin, const int value)
        {
            if constexpr (pin() - pinBase_ == 0)
            {
                altitude_ = value;
            }
        }

        template <const pin_t pin_>
        [[nodiscard]] inline gpio_t analogRead(const pin_constant<pin_> pin)
        {
            // Get the channel
            constexpr const pin_t chan = pin() - pinBase_;

            readTempPress();

            if constexpr (chan == 0) // Read Temperature
            {
                return cTemp_;
            }
            else if constexpr (chan == 1) // Pressure
            {
                return cPress_;
            }
            else if constexpr (chan == 2) // Pressure in mB
            {
                return cPress_ / pow(1 - (static_cast<scalar_t>(altitude_) / 44330.0), 5.255);
            }
            else
            {
                return 0;
            }
        }

    private:
        static constexpr const pin_t BMP180_OSS = 0;
        i2c<0x77, Model, Layout, wiringPiMode> i2c_;
        static constexpr const pin_t pinBase_ = pinBase;
        static constexpr const pin_t nPins_ = 4;

        void readTempPress()
        {
            // Start a temperature sensor reading
            i2c_.write8(0xF4, 0x2E);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            // Read the raw data and calculate
            const scalar_t tu = (static_cast<scalar_t>(static_cast<uint8_t>(-1)) * 256.0) + static_cast<scalar_t>(static_cast<uint8_t>(-1));
            const scalar_t a = c5_ * (tu - c6_);
            const scalar_t fTemp = a + (mc_ / (a + md_));
            cTemp_ = static_cast<gpio_t>(rint(((100.0 * fTemp) + 0.5) / 10.0));

            // Start a pressure sensor reading
            i2c_.write8(0xF4, 0x34 | (BMP180_OSS << 6));
            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            // Read the raw data nd calculate
            const scalar_t pu = (static_cast<scalar_t>(static_cast<uint8_t>(-1)) * 256.0) + static_cast<scalar_t>(static_cast<uint8_t>(-1)) + (static_cast<scalar_t>(static_cast<uint8_t>(-1)) / 256.0);
            const scalar_t s = fTemp - 25.0;
            const scalar_t x = (x2_ * pow(s, 2.0)) + (x1_ * s) + x0_;
            const scalar_t y = (yy2_ * pow(s, 2.0)) + (yy1_ * s) + yy0_;
            const scalar_t z = (pu - x) / y;
            const scalar_t fPress = (p2_ * pow(z, 2.0)) + (p1_ * z) + p0_;
            cPress_ = static_cast<gpio_t>(rint(((100.0 * fPress) + 0.5) / 10.0));
        }

        template <typename T>
        [[nodiscard]] T read16(const uint8_t reg) const
        {
            return static_cast<T>((i2c_.read8(reg) << 8) | i2c_.read8(reg + 1));
            // return static_cast<T>(-1);
        }

        // Methods to read the calibration parameters
        [[nodiscard]] int16_t AC1() const
        {
            return read16<int16_t>(0xAA);
        }
        [[nodiscard]] int16_t AC2() const
        {
            return read16<int16_t>(0xAC);
        }
        [[nodiscard]] int16_t AC3() const
        {
            return read16<int16_t>(0xAE);
        }
        [[nodiscard]] uint16_t AC4() const
        {
            return read16<uint16_t>(0xB0);
        }
        [[nodiscard]] uint16_t AC5() const
        {
            return read16<uint16_t>(0xB2);
        }
        [[nodiscard]] uint16_t AC6() const
        {
            return read16<uint16_t>(0xB4);
        }
        [[nodiscard]] int16_t VB1() const
        {
            return read16<int16_t>(0xB6);
        }
        [[nodiscard]] int16_t VB2() const
        {
            return read16<int16_t>(0xB8);
        }
        [[nodiscard]] int16_t MB() const
        {
            return read16<int16_t>(0xBA);
        }
        [[nodiscard]] int16_t MC() const
        {
            return read16<int16_t>(0xBC);
        }
        [[nodiscard]] int16_t MD() const
        {
            return read16<int16_t>(0xBE);
        }
        [[nodiscard]] scalar_t c3() const
        {
            return 160.0 * std::pow(2.0, -15.0) * static_cast<scalar_t>(AC3());
        }
        [[nodiscard]] scalar_t c4() const
        {
            return std::pow(10.0, -3.0) * std::pow(2.0, -15.0) * static_cast<scalar_t>(AC4());
        }
        [[nodiscard]] scalar_t b1() const
        {
            return std::pow(160.0, 2.0) * std::pow(2.0, -30.0) * static_cast<scalar_t>(VB1());
        }

        // Numerical constants
        const scalar_t c5_ = (std::pow(2.0, -15.0) / 160.0) * AC5();
        const scalar_t c6_ = static_cast<scalar_t>(AC6());
        const scalar_t mc_ = (std::pow(2.0, 11.0) / std::pow(160.0, 2.0)) * static_cast<scalar_t>(MC());
        const scalar_t md_ = static_cast<scalar_t>(MD()) / 160.0;
        const scalar_t x0_ = static_cast<scalar_t>(AC1());
        const scalar_t x1_ = 160.0 * std::pow(2.0, -13.0) * static_cast<scalar_t>(AC2());
        const scalar_t x2_ = std::pow(160.0, 2.0) * std::pow(2.0, -25.0) * static_cast<scalar_t>(VB2());
        const scalar_t yy0_ = static_cast<scalar_t>(c4()) * std::pow(2.0, 15.0);
        const scalar_t yy1_ = static_cast<scalar_t>(c4()) * static_cast<scalar_t>(c3());
        const scalar_t yy2_ = static_cast<scalar_t>(c4()) * static_cast<scalar_t>(b1());
        // static constexpr const scalar_t p0_ = (3791.0 - 8.0) / 1600.0;
        // static constexpr const scalar_t p1_ = 1.0 - 7357.0 * std::pow(2.0, -20.0);
        // static constexpr const scalar_t p2_ = 3038.0 * 100.0 * std::pow(2.0, -36.0);
        static constexpr const scalar_t p0_ = 3783.0 / 1600.0;
        static constexpr const scalar_t p1_ = 1041219.0 / 1048576.0;
        static constexpr const scalar_t p2_ = 37975.0 / 8589934592.0;

        int altitude_ = 0;
        gpio_t cPress_ = 0;
        gpio_t cTemp_ = 0;
    };
}

#endif