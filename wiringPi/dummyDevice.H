#ifndef __WIRING_PI_DUMMY_DEVICE_H
#define __WIRING_PI_DUMMY_DEVICE_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"
#include "../version.H"
#include "wiringPi.H"
#include "wiringPiNode.H"

// Dummy device occupying 8 pins by default

namespace WiringPi
{
    template <int pinBaseVal>
    class dummyDevice : public wiringPiNode<pinBaseVal, 8>
    {
    public:
        inline int digitalRead() const
        {
            std::cout << "Returning 99" << std::endl;
            return 99;
        }

    private:
    };
}

#endif