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
    [[nodiscard]] inline consteval bool setupModeCheck(const int mode)
    {
        return (mode == wiringPiModes::pins() || mode == wiringPiModes::gpio() || mode == wiringPiModes::phys());
    }

    // Assert that the mode must be valid
    template <const int mode_>
    inline void setupModeAssert(const mode_constant<mode_> mode)
    {
        static_assert(setupModeCheck(mode()), "Invalid wiringPiMode: valid modes are PINS, PHYS, GPIO.");
    }

    // Check that the mode works for PWM
    // This is currently identical to setupModeCheck as no other setup modes have been implemented yet
    [[nodiscard]] inline consteval bool pwmModeCheck(const int mode)
    {
        return (mode == wiringPiModes::pins() || mode == wiringPiModes::gpio() || mode == wiringPiModes::phys());
    }

    // Assert that the mode must work for PWM
    template <const int mode_>
    inline void pwmModeAssert(const mode_constant<mode_> mode)
    {
        static_assert(pwmModeCheck(mode()), "Invalid wiringPiMode for PWM operation: valid modes are PINS, PHYS, GPIO.");
    }

    // Assert that the mode must be GPIO on certain models
    template <const int model_, const int mode_>
    inline void computeModelAssert(const pi_model_constant<model_> model, const mode_constant<mode_> mode)
    {
        static_assert(!(model() == Pi::models::PiCM() && mode() != wiringPiModes::gpio()), "Only WPI_MODE_GPIO available on Pi::models::PiCM");
        static_assert(!(model() == Pi::models::PiCM3() && mode() != wiringPiModes::gpio()), "Only WPI_MODE_GPIO available on Pi::models::PiCM3");
        static_assert(!(model() == Pi::models::PiCM3P() && mode() != wiringPiModes::gpio()), "Only WPI_MODE_GPIO available on Pi::models::PiCM3P");
    }

    // Check that the model must be valid
    [[nodiscard]] inline consteval bool piModelCheck(const int model)
    {
        return (model == Pi::models::PiA() || model == Pi::models::PiB() || model == Pi::models::PiAP() || model == Pi::models::PiBP() || model == Pi::models::Pi2() || model == Pi::models::PiAlpha() || model == Pi::models::PiCM() || model == Pi::models::Pi07() || model == Pi::models::Pi3B() || model == Pi::models::PiZero() || model == Pi::models::PiCM3() || model == Pi::models::PiZero_W() || model == Pi::models::Pi3BP() || model == Pi::models::Pi3AP() || model == Pi::models::PiCM3P() || model == Pi::models::Pi4B() || model == Pi::models::PiZero_2W() || model == Pi::models::Pi400() || model == Pi::models::PiCM4() || model == Pi::models::PiCM4S() || model == Pi::models::Pi5());
    }

    // Assert that the model must be valid
    template <const int model_>
    inline void piModelAssert(const pi_model_constant<model_> model)
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