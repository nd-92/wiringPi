// ======================================================================== //
//                                                                          //
// wiringPiStaticAsserts.H                                                  //
//                                                                          //
// ======================================================================== //
// Static assertions used to ensure code correctness and perform unit tests //
// ======================================================================== //

#ifndef __WIRING_PI_wiringPiStaticAsserts_H
#define __WIRING_PI_wiringPiStaticAsserts_H

#include "wiringPiDefines.H"

namespace WiringPi
{
    using unit_test_fail = const std::integral_constant<bool, 0>;
    using unit_test_pass = const std::integral_constant<bool, 1>;

    // Check that the given pin is not null
    [[nodiscard]] inline consteval bool nullPinCheck(const pin_t pin)
    {
        return (pin != nullPin<pin_t>());
    }

    // Assert that the given pin must not be null
    template <const pin_t pin_>
    inline void nullPinAssert(const pin_constant<pin_> pin)
    {
        static_assert(nullPinCheck(pin()), "Pin number is null");
    }

    // Assert that the given pin must not be null nor greater than the max number of pins
    template <const pin_t __nPins, const pin_t pin_>
    inline void validPinAssert(const pin_constant<__nPins> nPins, const pin_constant<pin_> pin)
    {
        nullPinAssert(pin_constant<pin()>());
        static_assert(pin() < nPins(), "Pin number is not onboard (onboard pins range from 0 to 31)");
    }

    // Check that the mode is valid
    // This is currently identical to pwmModeCheck as no other setup modes have been implemented yet
    [[nodiscard]] inline consteval bool setupModeCheck(const wiringPiModes::type mode)
    {
        return (mode == wiringPiModes::pins || mode == wiringPiModes::gpio || mode == wiringPiModes::phys);
    }

    // Assert that the mode must be valid
    template <const wiringPiModes::type mode_>
    inline void setupModeAssert(const wiringPiModes::constant<mode_> mode)
    {
        static_assert(setupModeCheck(mode()), "Invalid wiringPiMode: valid modes are PINS, PHYS, GPIO.");
    }

    // Check that the mode works for PWM
    // This is currently identical to setupModeCheck as no other setup modes have been implemented yet
    [[nodiscard]] inline consteval bool pwmModeCheck(const wiringPiModes::type mode)
    {
        return (mode == wiringPiModes::pins || mode == wiringPiModes::gpio || mode == wiringPiModes::phys);
    }

    // Assert that the mode must work for PWM
    template <const wiringPiModes::type mode_>
    inline void pwmModeAssert(const wiringPiModes::constant<mode_> mode)
    {
        static_assert(pwmModeCheck(mode()), "Invalid wiringPiMode for PWM operation: valid modes are PINS, PHYS, GPIO.");
    }

    // Assert that the mode must be GPIO on certain models
    template <const Pi::model::type model_, const wiringPiModes::type mode_>
    inline void computeModelAssert(const Pi::model::constant<model_> model, const wiringPiModes::constant<mode_> mode)
    {
        static_assert(!(model() == Pi::model::PiCM && mode() != wiringPiModes::gpio), "Only WPI_MODE_GPIO available on Pi::model::PiCM");
        static_assert(!(model() == Pi::model::PiCM3 && mode() != wiringPiModes::gpio), "Only WPI_MODE_GPIO available on Pi::model::PiCM3");
        static_assert(!(model() == Pi::model::PiCM3P && mode() != wiringPiModes::gpio), "Only WPI_MODE_GPIO available on Pi::model::PiCM3P");
    }

    // Check that the model must be valid
    [[nodiscard]] inline consteval bool piModelCheck(const Pi::model::type model)
    {
        return (model == Pi::model::PiA || model == Pi::model::PiB || model == Pi::model::PiAP || model == Pi::model::PiBP || model == Pi::model::Pi2 || model == Pi::model::PiAlpha || model == Pi::model::PiCM || model == Pi::model::Pi07 || model == Pi::model::Pi3B || model == Pi::model::PiZero || model == Pi::model::PiCM3 || model == Pi::model::PiZero_W || model == Pi::model::Pi3BP || model == Pi::model::Pi3AP || model == Pi::model::PiCM3P || model == Pi::model::Pi4B || model == Pi::model::PiZero_2W || model == Pi::model::Pi400 || model == Pi::model::PiCM4 || model == Pi::model::PiCM4S || model == Pi::model::Pi5);
    }

    // Assert that the model must be valid
    template <const Pi::model::type model_>
    inline void piModelAssert(const Pi::model::constant<model_> model)
    {
        static_assert(piModelCheck(model()), "Invalid model of Raspberry pi");
    }

    // Check that the board layout must be valid
    [[nodiscard]] inline consteval bool layoutCheck(const int layout)
    {
        return (layout == 1 || layout == 2);
    }

    // Assert that the mode is valid
    template <const int layout_>
    inline void layoutAssert(const layout_constant<layout_> layout)
    {
        static_assert(layoutCheck(layout()), "Invalid GPIO layout");
    }
}

#endif