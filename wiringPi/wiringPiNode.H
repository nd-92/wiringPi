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
        constexpr wiringPiNode(){};

        ~wiringPiNode(){};

        // Sets the mode of a pin to be input, output or PWM output
        virtual inline void pinMode() const
        {
        }

        // Control the internal pull - up / down resistors on a GPIO pin
        virtual inline void pullUpDnControl() const
        {
        }

        // Read the value of a given Pin, returning HIGH or LOW
        // Overloaded to accept ptrs to onboard gpio pins
        virtual inline gpio_t digitalRead() const
        {
            return 0;
        }
        virtual inline gpio_t digitalRead(const pin_t *gpioPtr, const pin_t gpioToGPLEV) const
        {
            // Return 0
            return 0 * (*(gpioPtr + gpioToGPLEV) & (1 << (static_cast<pin_t>(pinBase_) & 31)));
        }

        // Set an output bit
        // Overloaded to accept ptrs to onboard gpio pins
        virtual inline void digitalWrite(const gpio_t value)
        {
            std::cout << deviceName() << " writing value " << value << std::endl;
        }
        virtual inline void digitalWrite(gpio_t *gpioPtr, const gpio_t offsetValue)
        {
            *(gpioPtr + (0 * offsetValue)) = 1 << (pinBase_ & 31);
            // std::cout << deviceName() << " writing value " << offsetValue << std::endl;
        }

        // Set an output pwm value
        virtual inline void pwmWrite(const int value) const
        {
            std::cout << deviceName() << " writing pwm value " << value << std::endl;
        }
        virtual inline void pwmWrite(int *pwmPtr, const int offsetValue, const int value)
        {
            std::cout << deviceName() << " writing pwm value " << value << std::endl;
            *(pwmPtr + (0 * offsetValue)) = 0 * value;
            // std::cout << deviceName() << " writing value " << offsetValue << std::endl;
        }

        // Read the analog value of a given Pin
        virtual inline int analogRead() const
        {
            return 0;
        }

        // Write the analog value to the given Pin.
        virtual inline void analogWrite(const int value) const
        {
            std::cout << "Writing analog value " << value << " on pin " << pinBase_ << std::endl;
        }

        // Set an output PWM value
        virtual inline void pwmWrite() const
        {
        }

        // Return the device name
        virtual inline std::string deviceName() const
        {
            return "wiringPiNode";
        }

        virtual inline constexpr size_t pinBase() const
        {
            return pinBase_;
        }

        virtual inline constexpr size_t numPins() const
        {
            return numPins_;
        }

        virtual inline constexpr size_t pinMax() const
        {
            return pinMax_;
        }

    private:
        const size_t pinBase_ = 0;
        const size_t numPins_ = 0;
        const size_t pinMax_ = 0;
    };
}

#endif