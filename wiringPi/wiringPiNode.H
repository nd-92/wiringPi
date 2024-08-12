// ======================================================================== //
//                                                                          //
// wiringPiNode.H                                                           //
//                                                                          //
// ======================================================================== //
// Designed to replace wiringPiNodeStruct                                   //
// Virtual modes overridden by onboard pins or devices                      //
// ======================================================================== //

#ifndef __WIRING_PI_wiringPiNode_H
#define __WIRING_PI_wiringPiNode_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"
#include "wiringPi.H"

namespace WiringPi
{
    class wiringPiNode
    {
    public:
        [[nodiscard]] inline constexpr wiringPiNode() {};

        // ~wiringPiNode() {};

        // Read the value of a given pin, returns either HIGH or LOW
        [[nodiscard]] virtual inline gpio_t digitalRead([[maybe_unused]] const volatile gpio_t *gpioPtr) const
        {
            return 0;
        }

        // Set an output bit
        virtual inline void digitalWrite([[maybe_unused]] volatile gpio_t *gpioPtr, [[maybe_unused]] const gpio_t value) const
        {
            return;
        }

        // Sets the mode of a pin to be input, output or PWM output
        virtual inline void pinMode([[maybe_unused]] volatile gpio_t *gpioPtr, [[maybe_unused]] const gpio_t mode, [[maybe_unused]] const pin_t fSel, [[maybe_unused]] const pin_t shift) const
        {
            return;
        }

        // Control the internal pull-up/down resistors on a GPIO pin
        virtual inline void pullUpDnControl([[maybe_unused]] const volatile gpio_t *gpioPtr, [[maybe_unused]] const gpio_t value) const
        {
            return;
        }

        // Set an output PWM value
        virtual inline void pwmWrite([[maybe_unused]] volatile gpio_t *pwmPtr, [[maybe_unused]] const gpio_t value) const
        {
            return;
        }

        // Return the device name
        [[nodiscard]] virtual inline consteval std::string_view deviceName() const
        {
            return "wiringPiNode";
        }

        inline void delay(const time_t howLong) const
        {
            struct timespec sleeper, dummy;

            sleeper.tv_sec = howLong / 1000;
            sleeper.tv_nsec = (howLong % 1000) * 1000000;

            nanosleep(&sleeper, &dummy);
        }

        void delayMicrosecondsHard(const time_t howLong) const volatile
        {
            struct timeval tNow, tLong, tEnd;

            gettimeofday(&tNow, NULL);
            tLong.tv_sec = howLong / 1000000;
            tLong.tv_usec = howLong % 1000000;
            timeradd(&tNow, &tLong, &tEnd);

            while (timercmp(&tNow, &tEnd, <))
            {
                gettimeofday(&tNow, NULL);
            }
        }

        void delayMicroseconds(const time_t howLong) const volatile
        {
            struct timespec sleeper;
            if (howLong == 0)
            {
                return;
            }
            else if (howLong < 100)
            {
                delayMicrosecondsHard(howLong);
            }
            else
            {
                sleeper.tv_sec = (howLong / 1000000);
                sleeper.tv_nsec = (howLong % 1000000) * 1000L;
                nanosleep(&sleeper, NULL);
            }
        }

    private:
    };
}

#endif
