#ifndef __WIRING_PI_NODE_H
#define __WIRING_PI_NODE_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"
#include "wiringPi.H"

// This class replicates wiringPiNodeStruct

namespace WiringPi
{
    // template <size_t pinBaseVal, size_t numPinsVal>
    class wiringPiNode
    {
    public:
        constexpr inline wiringPiNode() {};

        ~wiringPiNode() {};

        // Sets the mode of a pin to be input, output or PWM output
        // virtual inline void pinMode() const
        // {
        // }

        // // Control the internal pull - up / down resistors on a GPIO pin
        // virtual inline void pullUpDnControl() const
        // {
        // }
        // virtual inline void pullUpDnControl(
        //     [[maybe_unused]] gpio_t volatile *gpioPtr,
        //     [[maybe_unused]] const gpio_t value,
        //     [[maybe_unused]] const int piGpioPupOffset) const
        // {
        //     // std::cout << "Called dummy " << (*gpioPtr + offsetValue + value + static_cast<gpio_t>(piGpioPupOffset)) << std::endl;
        // }

        // Read the value of a given Pin, returning HIGH or LOW
        // Overloaded to accept ptrs to onboard gpio pins
        [[nodiscard]] virtual inline gpio_t digitalRead() const volatile
        {
            return 0;
        }
        [[nodiscard]] virtual inline gpio_t digitalRead([[maybe_unused]] const volatile pin_t *gpioPtr, [[maybe_unused]] const pin_t gpioToGPLEV) const volatile
        {
            return 0;
        }

        // Set an output bit
        // Overloaded to accept ptrs to onboard gpio pins
        virtual inline void digitalWrite([[maybe_unused]] const gpio_t value) const
        {
        }
        virtual inline void digitalWrite([[maybe_unused]] volatile gpio_t *gpioPtr, [[maybe_unused]] const gpio_t offsetValue) const volatile
        {
        }

        // Set an output pwm value
        // virtual inline void pwmWrite([[maybe_unused]] const pwm_t value) const
        // {
        // }
        // virtual inline void pwmWrite([[maybe_unused]] pwm_t *pwmPtr, [[maybe_unused]] const int offsetValue, [[maybe_unused]] const int value) const volatile
        // {
        // }

        // Read the analog value of a given Pin
        // [[nodiscard]] virtual inline int analogRead() const
        // {
        //     return 0;
        // }

        // // Write the analog value to the given Pin.
        // virtual inline void analogWrite([[maybe_unused]] const int value) const
        // {
        // }

        // // Set an output PWM value
        // virtual inline void pwmWrite() const
        // {
        // }

        // Return the device name
        [[nodiscard]] virtual inline consteval std::string_view deviceName() const
        {
            return "wiringPiNode";
        }

        [[nodiscard]] virtual inline constexpr pin_t pinBase() const
        {
            return pinBase_;
        }

        [[nodiscard]] virtual inline constexpr pin_t numPins() const
        {
            return numPins_;
        }

        [[nodiscard]] virtual inline constexpr pin_t pinMax() const
        {
            return pinMax_;
        }

        inline void delay(const time_t howLong) const
        {
            struct timespec sleeper, dummy;

            sleeper.tv_sec = howLong / 1000;
            sleeper.tv_nsec = (howLong % 1000) * 1000000;

            nanosleep(&sleeper, &dummy);
        }

        inline void delayMicrosecondsHard(const time_t howLong) const
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

        inline void delayMicroseconds(const time_t howLong) const
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
        static constexpr const pin_t pinMode_ = 0;
        static constexpr const pin_t pinBase_ = 0;
        static constexpr const pin_t numPins_ = 0;
        static constexpr const pin_t pinMax_ = 0;
    };
}

#endif