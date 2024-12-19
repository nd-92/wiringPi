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
    // Initialise piGpioBase and piGpioPupOffset from board model - default vals 1 and 0
    // Must be defined at namespace scope
    template <typename T, const int model>
    [[nodiscard]] inline consteval T piGpioPupOffset()
    {
        static_assert(piModelCheck(model), "Invalid Raspberry Pi model");

        switch (model)
        {
        case Pi::models::PiA():
        {
            return 0;
        }
        case Pi::models::PiB():
        {
            return 0;
        }
        case Pi::models::PiAP():
        {
            return 0;
        }
        case Pi::models::PiBP():
        {
            return 0;
        }
        case Pi::models::PiAlpha():
        {
            return 0;
        }
        case Pi::models::PiCM():
        {
            return 0;
        }
        case Pi::models::PiZero():
        {
            return 0;
        }
        case Pi::models::PiZero_W():
        {
            return GPPUD<T>();
        }
        case Pi::models::Pi4B():
        {
            return 0;
        }
        case Pi::models::Pi400():
        {
            return 0;
        }
        case Pi::models::PiCM4():
        {
            return GPPUPPDN0<T>();
        }
        default:
        {
            return GPPUD<T>();
        }
        }
    }

    class piHardware
    {
    public:
        [[nodiscard]] inline constexpr piHardware() {};

        // Returns the model code
        [[nodiscard]] inline consteval int model() const
        {
            return piModel_;
        }

        // Returns the GPIO pin layout code
        [[nodiscard]] inline consteval int gpioLayout() const
        {
            return gpioLayout_;
        }

        // Returns the revision code
        [[nodiscard]] inline consteval int revision() const
        {
            return piRev_;
        }

        // Returns the memory code
        [[nodiscard]] inline consteval int memory() const
        {
            return piMem_;
        }

        // Returns the maker code
        [[nodiscard]] inline consteval int maker() const
        {
            return piMaker_;
        }

        // Returns the warranty code
        [[nodiscard]] inline consteval int warranty() const
        {
            return piWarranty_;
        }

        // Returns a string_view of the model
        [[nodiscard]] inline consteval std::string_view modelString() const
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
        [[nodiscard]] inline consteval pin_t numPins() const
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
        static constexpr const int piModel_ = PI_MODEL<int>();
        static constexpr const int gpioLayout_ = GPIO_LAYOUT<int>();
        static constexpr const int piRev_ = PI_REV<int>();
        static constexpr const int piMem_ = PI_MEM<int>();
        static constexpr const int piMaker_ = PI_MAKER<int>();
        static constexpr const int piWarranty_ = PI_WARRANTY<int>();
        static constexpr const pin_t piGpioPupOffset_ = piGpioPupOffset<pin_t, piModel_>();
        static constexpr const std::string_view modelString_ = Pi::models::names(piModel_);
        static constexpr const pin_t numPhysicalPins_ = pinTables::numPhysicalPins<gpioLayout_>();
    };
}

#endif
