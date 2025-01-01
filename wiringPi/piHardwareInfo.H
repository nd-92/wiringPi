// ======================================================================== //
//                                                                          //
// piHardwareInfo.H                                                         //
//                                                                          //
// ======================================================================== //
// Hardware information class for the Raspberry Pi                          //
// All functions consteval                                                  //
// Requires piInfo.H to be generated in order to function - use readCpuInfo //
// ======================================================================== //

#ifndef __WIRING_PI_piHardwareInfo_H
#define __WIRING_PI_piHardwareInfo_H

#include "wiringPi.H"

namespace WiringPi
{
    template <const Pi::model::type Model, const Pi::layout::type Layout>
    class piHardware
    {
    public:
        [[nodiscard]] inline constexpr piHardware() {};

        // Returns the model code
        [[nodiscard]] static inline consteval Pi::model::type model()
        {
            return piModel_;
        }

        // Returns the GPIO pin layout code
        [[nodiscard]] static inline consteval Pi::layout::type gpioLayout()
        {
            return gpioLayout_;
        }

        // // Returns the revision code
        // [[nodiscard]] inline consteval int revision() const
        // {
        //     return piRev_;
        // }

        // // Returns the memory code
        // [[nodiscard]] inline consteval int memory() const
        // {
        //     return piMem_;
        // }

        // // Returns the maker code
        // [[nodiscard]] inline consteval int maker() const
        // {
        //     return piMaker_;
        // }

        // // Returns the warranty code
        // [[nodiscard]] inline consteval int warranty() const
        // {
        //     return piWarranty_;
        // }

        // Returns a string_view of the model
        [[nodiscard]] inline consteval name_t modelString() const
        {
            return modelString_;
        }

        // Check if pin is onboard
        // Returns true only if less than 32
        [[nodiscard]] inline consteval bool isOnboard(const pin_t pin) const
        {
            return pin < numPhysicalPins_;
        }

        // Returns the number of physical pins
        [[nodiscard]] inline consteval pin_t nPins() const
        {
            return numPhysicalPins_;
        }

        // Returns the offset to the GPIO pull up/down register
        [[nodiscard]] inline consteval pin_t gpioPupOffset() const
        {
            return piGpioPupOffset_;
        }

    private:
        // Pi hardware info
        static constexpr const Pi::model::type piModel_ = Model;
        static constexpr const Pi::layout::type gpioLayout_ = Layout;

        [[nodiscard]] static inline consteval pin_t piGpioPupOffset()
        {
            static_assert(piModelCheck(piModel_), "Invalid Raspberry Pi model");

            switch (piModel_)
            {
            case Pi::model::PiA:
            {
                return 0;
            }
            case Pi::model::PiB:
            {
                return 0;
            }
            case Pi::model::PiAP:
            {
                return 0;
            }
            case Pi::model::PiBP:
            {
                return 0;
            }
            case Pi::model::PiAlpha:
            {
                return 0;
            }
            case Pi::model::PiCM:
            {
                return 0;
            }
            case Pi::model::PiZero:
            {
                return 0;
            }
            case Pi::model::PiZero_W:
            {
                return GPPUD<pin_t>();
            }
            case Pi::model::Pi4B:
            {
                return 0;
            }
            case Pi::model::Pi400:
            {
                return 0;
            }
            case Pi::model::PiCM4:
            {
                return GPPUPPDN0<pin_t>();
            }
            default:
            {
                return GPPUD<pin_t>();
            }
            }
        }
        // static constexpr const int piRev_ = PI_REV<int>();
        // static constexpr const int piMem_ = PI_MEM<int>();
        // static constexpr const int piMaker_ = PI_MAKER<int>();
        // static constexpr const int piWarranty_ = PI_WARRANTY<int>();
        static constexpr const pin_t piGpioPupOffset_ = piGpioPupOffset();
        static constexpr const name_t modelString_ = Pi::model::names(piModel_);
        static constexpr const pin_t numPhysicalPins_ = pinTables::numPhysicalPins<gpioLayout_>();
    };
}

#endif
