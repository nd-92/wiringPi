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
        virtual inline int digitalRead() const
        {
            return 0;
        }

        // Set an output bit
        virtual inline void digitalWrite() const
        {
        }

        // Read the analog value of a given Pin
        virtual inline int analogRead() const
        {
            return 0;
        }

        // Write the analog value to the given Pin.
        virtual inline void analogWrite() const
        {
        }

        // Set an output PWM value
        virtual inline void pwmWrite() const
        {
        }

        // Return the device name
        virtual inline std::string deviceName() const
        // virtual inline std::string deviceName() const
        {
            return "wiringPiNode";
        }

        virtual inline constexpr size_t pinBase() const
        {
            return 0;
        }

        virtual inline constexpr size_t numPins() const
        {
            return 0;
        }

        virtual inline constexpr size_t pinMax() const
        {
            return 0;
        }

    private:
        const size_t pinBase_ = 0;
        const size_t numPins_ = 0;
        const size_t pinMax_ = 0;
    };
}

#endif