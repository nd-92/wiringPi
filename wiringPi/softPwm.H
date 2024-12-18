// ======================================================================== //
//                                                                          //
// softPwm.H                                                                //
//                                                                          //
// ======================================================================== //
// This class implements the functionality of softPwm.h from wiringPi       //
// ======================================================================== //

#ifndef __WIRING_PI_softPwm_H
#define __WIRING_PI_softPwm_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"
#include "wiringPiPinTables.H"

namespace WiringPi
{
    inline void nullFunc()
    {
        return;
    }

    [[nodiscard]] inline std::array<std::thread, MAX_PINS<size_t>()> defaultThreads()
    {
        std::array<std::thread, MAX_PINS<size_t>()> myThreads;
        for (size_t i = 0; i < MAX_PINS<size_t>(); i++)
        {
            myThreads[i] = std::thread(nullFunc);
        }
        for (size_t i = 0; i < MAX_PINS<size_t>(); i++)
        {
            myThreads[i].join();
        }
        return myThreads;
    }

    class softPwm
    {
    public:
        [[nodiscard]] inline softPwm()
            : marks_(softPwmMarksI()),
              range_(softPwmRangeI()) {};

        ~softPwm() {};

        template <const pin_t pinNumber>
        [[nodiscard]] inline int create(const pwm_t initialValue, const pwm_t pwmRange) const
        {
            // Check that pinNumber < 64
            static_assert(static_cast<size_t>(pinNumber) < MAX_PINS<size_t>());

            // Check that thread is not already running on this pin
            assert((range_[pinNumber] != 0));

            // Check that the range is not less than 0
            assert((pwmRange <= 0));
        }

    private:
        // Default initialised to 0
        pinTable marks_;

        // Default initialised to 0
        pinTable range_;

        // Initialised to a null function
        std::array<std::thread, MAX_PINS<size_t>()> threads_ = defaultThreads();
    };
}

#endif