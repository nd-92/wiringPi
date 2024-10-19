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
        [[nodiscard]] inline consteval bool isOnboard(const pin_t pinNumber) const
        {
            return pinNumber < numPhysicalPins_;
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
        static constexpr const pin_t piGpioPupOffset_ = piGpioPupOffset<pin_t, PI_MODEL<int>()>();
        // static constexpr const std::string_view modelString_ = piModelStrings()[PI_MODEL<size_t>()];
        static constexpr const std::string_view modelString_ = piModelStrings(PI_MODEL<size_t>());
        static constexpr const pin_t numPhysicalPins_ = numPhysicalPins<GPIO_LAYOUT<int>()>();
    };
}

#endif
