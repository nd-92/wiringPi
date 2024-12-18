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

    // Static assertion check for valid program mode
    template <const int __mode>
    inline void setupModeCheck(const mode_constant<__mode> mode)
    {
        static_assert(((mode() == WPI_MODE_PINS()) || (mode() == WPI_MODE_PHYS()) || (mode() == WPI_MODE_GPIO())), "Invalid wiringPiMode: valid modes are PINS, PHYS, GPIO.");
    }

    // Static assertion check for non-GPIO mode on certain models
    template <const int __model, const int __mode>
    inline void computeModelCheck(const pi_model_constant<__model> model, const mode_constant<__mode> mode)
    {
        static_assert(!(model() == PI_MODEL_CM<int>() && mode() != WPI_MODE_GPIO()), "Only WPI_MODE_GPIO available on PI_MODEL_CM");
        static_assert(!(model() == PI_MODEL_CM3<int>() && mode() != WPI_MODE_GPIO()), "Only WPI_MODE_GPIO available on PI_MODEL_CM3");
        static_assert(!(model() == PI_MODEL_CM3P<int>() && mode() != WPI_MODE_GPIO()), "Only WPI_MODE_GPIO available on PI_MODEL_CM3P");
    }

    template <const pin_t __nPins, const pin_t __pinNumber>
    inline void validPinCheck(const pin_constant<__nPins> nPins, const pin_constant<__pinNumber> pinNumber)
    {
        static_assert(pinNumber() != nullPin<pin_t>(), "Pin number is null");
        static_assert(pinNumber() < nPins(), "Pin number is not onboard (onboard pins range from 0 to 31)");
    }

    // Check for a valid model
    [[nodiscard]] inline consteval bool piModelCheck(const int model)
    {
        return (model == PI_MODEL_A<int>() || model == PI_MODEL_B<int>() || model == PI_MODEL_AP<int>() || model == PI_MODEL_BP<int>() || model == PI_MODEL_2<int>() || model == PI_MODEL_ALPHA<int>() || model == PI_MODEL_CM<int>() || model == PI_MODEL_07<int>() || model == PI_MODEL_3B<int>() || model == PI_MODEL_ZERO<int>() || model == PI_MODEL_CM3<int>() || model == PI_MODEL_ZERO_W<int>() || model == PI_MODEL_3BP<int>() || model == PI_MODEL_3AP<int>() || model == PI_MODEL_CM3P<int>() || model == PI_MODEL_4B<int>() || model == PI_MODEL_ZERO_2W<int>() || model == PI_MODEL_400<int>() || model == PI_MODEL_CM4<int>() || model == PI_MODEL_CM4S<int>() || model == PI_MODEL_5<int>());
    }

    // Check for a valid board layout
    // template <const int layout>
    [[nodiscard]] inline consteval bool piLayoutCheck(const int layout)
    {
        return (layout == 1 || layout == 2);
    }
}

#endif