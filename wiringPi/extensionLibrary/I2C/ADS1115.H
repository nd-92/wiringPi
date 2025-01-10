// ======================================================================== //
//                                                                          //
// ADS1115.H                                                                //
//                                                                          //
// ======================================================================== //
// Extend wiringPi with the ADS1115 I2C 16-bit ADC                          //
// ======================================================================== //

#ifndef __WIRING_PI_ADS1115_H
#define __WIRING_PI_ADS1115_H

namespace WiringPi
{

    namespace config
    {
        typedef uint16_t config_t;

        // Operational Status Register
        // Write - Starts a single-conversion
        // Read    1 = Conversion complete
        namespace os
        {
            [[nodiscard]] inline consteval config_t MASK() { return 0x8000; }
            [[nodiscard]] inline consteval config_t SINGLE() { return 0x8000; }
        }

        // Multiplexor
        namespace mux
        {
            [[nodiscard]] inline consteval config_t MASK() { return 0x7000; }

            // Differential modes
            namespace diff
            {
                [[nodiscard]] inline consteval config_t N0N1() { return 0x0000; } // Pos = AIN0, Neg = AIN1 (default)
                [[nodiscard]] inline consteval config_t N0N3() { return 0x1000; } // Pos = AIN0, Neg = AIN3
                [[nodiscard]] inline consteval config_t N1N3() { return 0x2000; } // Pos = AIN1, Neg = AIN3
                [[nodiscard]] inline consteval config_t N2N3() { return 0x3000; } // Pos = AIN2, Neg = AIN3 (2nd differential channel)
            }

            // Single-ended modes
            namespace single
            {
                [[nodiscard]] inline consteval config_t N0() { return 0x4000; } // AIN0
                [[nodiscard]] inline consteval config_t N1() { return 0x5000; } // AIN1
                [[nodiscard]] inline consteval config_t N2() { return 0x6000; } // AIN2
                [[nodiscard]] inline consteval config_t N3() { return 0x7000; } // AIN3
            }
        }

        // Programmable gain amplifier
        namespace pga
        {
            [[nodiscard]] inline consteval config_t MASK() { return 0x0E00; }
            [[nodiscard]] inline consteval config_t _6_144V() { return 0x0000; } // +/-6.144V range = Gain 2/3
            [[nodiscard]] inline consteval config_t _4_096V() { return 0x0200; } // +/-4.096V range = Gain 1
            [[nodiscard]] inline consteval config_t _2_048V() { return 0x0400; } // +/-2.048V range = Gain 2 (default)
            [[nodiscard]] inline consteval config_t _1_024V() { return 0x0600; } // +/-1.024V range = Gain 4
            [[nodiscard]] inline consteval config_t _0_512V() { return 0x0800; } // +/-0.512V range = Gain 8
            [[nodiscard]] inline consteval config_t _0_256V() { return 0x0A00; } // +/-0.256V range = Gain 16
            [[nodiscard]] inline consteval config_t MODE() { return 0x0100; }    // 0 is continuous, 1 is single-shot (default)
        }

        // Data rate
        namespace dr
        {
            [[nodiscard]] inline consteval config_t MASK() { return 0x00E0; }

            // Samples per second
            namespace sps
            {
                [[nodiscard]] inline consteval config_t _8SPS() { return 0x0000; }   //   8 samples per second
                [[nodiscard]] inline consteval config_t _16SPS() { return 0x0020; }  //  16 samples per second
                [[nodiscard]] inline consteval config_t _32SPS() { return 0x0040; }  //  32 samples per second
                [[nodiscard]] inline consteval config_t _64SPS() { return 0x0060; }  //  64 samples per second
                [[nodiscard]] inline consteval config_t _128SPS() { return 0x0080; } // 128 samples per second (default)
                [[nodiscard]] inline consteval config_t _475SPS() { return 0x00A0; } // 475 samples per second
                [[nodiscard]] inline consteval config_t _860SPS() { return 0x00C0; } // 860 samples per second
            }
        }

        // Comparator mode
        [[nodiscard]] inline consteval config_t CMODE_MASK() { return 0x0010; }
        [[nodiscard]] inline consteval config_t CMODE_TRAD() { return 0x0000; }   // Traditional comparator with hysteresis (default)
        [[nodiscard]] inline consteval config_t CMODE_WINDOW() { return 0x0010; } // Window comparator

        // Comparator polarity - the polarity of the output alert/rdy pin
        [[nodiscard]] inline consteval config_t CPOL_MASK() { return 0x0008; }
        [[nodiscard]] inline consteval config_t CPOL_ACTVLOW() { return 0x0000; } // Active low (default)
        [[nodiscard]] inline consteval config_t CPOL_ACTVHI() { return 0x0008; }  // Active high

        // Latching comparator - does the alert/rdy pin latch
        [[nodiscard]] inline consteval config_t CLAT_MASK() { return 0x0004; }
        [[nodiscard]] inline consteval config_t CLAT_NONLAT() { return 0x0000; } // Non-latching comparator (default)
        [[nodiscard]] inline consteval config_t CLAT_LATCH() { return 0x0004; }  // Latching comparator

        // Comparitor queue
        [[nodiscard]] inline consteval config_t CQUE_MASK() { return 0x0003; }
        [[nodiscard]] inline consteval config_t CQUE_1CONV() { return 0x0000; } // Assert after one conversions
        [[nodiscard]] inline consteval config_t CQUE_2CONV() { return 0x0001; } // Assert after two conversions
        [[nodiscard]] inline consteval config_t CQUE_4CONV() { return 0x0002; } // Assert after four conversions
        [[nodiscard]] inline consteval config_t CQUE_NONE() { return 0x0003; }  // Disable the comparator (default)
        [[nodiscard]] inline consteval config_t DEFAULT() { return 0x8583; }    // From the datasheet
    }

    template <const pin_t pinBase, const Pi::model::type Model, const Pi::layout::type Layout, const wiringPiModes::type wiringPiMode>
    class ADS1115 : public wiringPiNode
    {
    public:
        // Default constructor: take a pin_constant and a reference to the wiringPi object
        [[nodiscard]] ADS1115(const pin_constant<pinBase> devicePin, wiringPi<Model, Layout, wiringPiMode> &RaspberryPi) : i2c_(I2C<devicePin(), Model, Layout, wiringPiMode>(RaspberryPi)) {};

        ~ADS1115() {};

        [[nodiscard]] inline int fd() const
        {
            return i2c_.fd();
        }

        [[nodiscard]] inline consteval name_t deviceName() const
        {
            return deviceName_;
        }

        template <const pin_t pin_>
        inline void digitalWrite(const pin_constant<pin_> pin, const uint16_t value)
        {
            if constexpr (pin - pinBase_ == 0) // Gain Control
            {
                // static_assert(i < 6, "Data out of range!");
                data_[0] = gains_[(value < 6) ? value : 2];
            }
            else // Data rate control
            {
                // static_assert(i < 7, "Data out of range!");
                data_[1] = dataRates_[(value < 7) ? value : 4];
            }
        }

        template <const pin_t pin_>
        inline void analogWrite(const pin_constant<pin_> pin, const uint16_t value) const
        {
            // This condition must be met
            static_assert(pin - pinBase_ < 3);

            // Much cleaner write16
            i2c_.write16(pin - pinBase_ + 2, value);
        }

        template <const pin_t pin_>
        [[nodiscard]] inline uint16_t analogConfig(const pin_constant<pin_> pin) const
        {
            // Make sure the channel is from 0 to 7
            static_assert(pin - pinBase_ < 8);

            config::config_t config = config::DEFAULT();
            config &= static_cast<config::config_t>(~config::pga::MASK());
            config |= data_[0];

            //	Set sample speed
            config &= static_cast<config::config_t>(~config::dr::MASK());
            config |= data_[1];
            //	Set single-ended channel or differential mode
            config &= static_cast<config::config_t>(~config::mux::MASK());

            if constexpr (pin - pinBase_ == 0)
            {
                config |= config::mux::single::N0();
            }
            else if constexpr (pin - pinBase_ == 1)
            {
                config |= config::mux::single::N1();
            }
            else if constexpr (pin - pinBase_ == 2)
            {
                config |= config::mux::single::N2();
            }
            else if constexpr (pin - pinBase_ == 3)
            {
                config |= config::mux::single::N3();
            }
            else if constexpr (pin - pinBase_ == 4)
            {
                config |= config::mux::diff::N0N1();
            }
            else if constexpr (pin - pinBase_ == 5)
            {
                config |= config::mux::diff::N2N3();
            }
            else if constexpr (pin - pinBase_ == 6)
            {
                config |= config::mux::diff::N0N3();
            }
            else if constexpr (pin - pinBase_ == 7)
            {
                config |= config::mux::diff::N1N3();
            }

            config |= config::os::SINGLE();

            return __bswap_16(config);
        }

        template <const pin_t pin_>
        [[nodiscard]] inline uint16_t analogRead(const pin_constant<pin_> pin) const
        {
            i2c_.write16(1, analogConfig(pin));

            // Wait for the conversion to complete
            for (;;) // Potentially dangerous - should be killable
            {
                const uint16_t result_1 = __bswap_16(i2c_.read16(1));
                if ((result_1 & config::os::MASK()) != 0)
                {
                    break;
                }
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }

            return __bswap_16(i2c_.read16(0));
        }

        template <const pin_t i_>
        [[nodiscard]] inline constexpr config::config_t data(const pin_constant<i_> i) const
        {
            static_assert(i < 2);
            return data_[i];
        }

        template <const pin_t i_>
        [[nodiscard]] inline consteval config::config_t dataRate(const pin_constant<i_> i) const
        {
            static_assert(i < 7);
            return dataRates_[i];
        }

        template <const pin_t i_>
        [[nodiscard]] inline consteval config::config_t gain(const pin_constant<i_> i) const
        {
            static_assert(i < 6);
            return gains_[i];
        }

    private:
        const I2C<pinBase, Model, Layout, wiringPiMode> i2c_;
        static constexpr const pin_t pinBase_ = pinBase;
        static constexpr const pin_t nPins_ = 8;
        static constexpr const name_t deviceName_ = "ADS1115";

        // Array holding the data
        std::array<config::config_t, 2> data_ = {config::pga::_4_096V(), config::dr::sps::_128SPS()};

        static constexpr const std::array<config::config_t, 7> dataRates_{
            config::dr::sps::_8SPS(),
            config::dr::sps::_16SPS(),
            config::dr::sps::_32SPS(),
            config::dr::sps::_64SPS(),
            config::dr::sps::_128SPS(),
            config::dr::sps::_475SPS(),
            config::dr::sps::_860SPS()};

        static constexpr const std::array<config::config_t, 6> gains_ = {
            config::pga::_6_144V(),
            config::pga::_4_096V(),
            config::pga::_2_048V(),
            config::pga::_1_024V(),
            config::pga::_0_512V(),
            config::pga::_0_256V()};
    };
}

#endif