#ifndef __WIRING_PI_NODE_H
#define __WIRING_PI_NODE_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"
#include "../version.H"
#include "wiringPi.H"

// This class replicates wiringPiNodeStruct
// Device setups are from array of classes

namespace WiringPi
{
    template <int pinBaseVal, int numPinsVal>
    class wiringPiNode
    {
    public:
        wiringPiNode(){};

        ~wiringPiNode(){};

        constexpr int pinBase() const
        {
            return pinBase_;
        }

        constexpr int numPins() const
        {
            return numPins_;
        }

        constexpr int pinMax() const
        {
            return pinMax_;
        }

    private:
        static constexpr int pinBase_ = pinBaseVal;
        static constexpr int numPins_ = numPinsVal;
        static constexpr int pinMax_ = pinBaseVal + numPinsVal - 1;
    };
}

#endif