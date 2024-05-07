#ifndef __WIRING_PI_GPIO_H
#define __WIRING_PI_GPIO_H

// #include "wiringPiIncludes.H"
// #include "wiringPiDefines.H"
#include "wiringPi.H"
#include "wiringPiNode.H"

// GPIO device occupying 1 pin

namespace WiringPi
{
    class GPIO : public wiringPiNode
    {
    public:
        GPIO(
            const size_t pinBaseVal)
            : pinBase_(pinBaseVal){};

        // inline int digitalRead() const
        // {
        //     // Unsure exactly what this expression does
        //     if ((*(wPi_.gpioPtr() + wPi_.gpioToGPLEV()[pinBase_]) & (1 << (pinBase_ & 31))) != 0)
        //     // if (0 == 0)
        //     {
        //         std::cout << "GPIO digitalRead returning 1" << std::endl;
        //         return HIGH<int>();
        //     }
        //     else
        //     {
        //         std::cout << "GPIO digitalRead returning 0" << std::endl;
        //         return LOW<int>();
        //     }
        // }

        inline int digitalRead() const
        {
            std::cout << deviceName() << " digitalRead returning 1" << std::endl;
            return HIGH<int>();
        }

        inline void digitalWrite() const
        {
            std::cout << deviceName() << " digitalWrite writing 99" << std::endl;
        }

        inline std::string deviceName() const
        {
            return "GPIO";
        }

        inline constexpr size_t pinBase() const
        {
            return pinBase_;
        }

        inline constexpr size_t numPins() const
        {
            return numPins_;
        }

        inline constexpr size_t pinMax() const
        {
            return pinMax_;
        }

    private:
        const size_t pinBase_;
        const size_t numPins_ = 1;
        const size_t pinMax_ = pinBase_ + numPins_ - 1;

        // const wiringPi<WPI_MODE_GPIO<int>()> &wPi_;

        // Const ptr to gpio - probably should not be raw ptr
        // const int *gpio_;

        // Ref to gpioToGPLEV vector
        // const std::vector<int> &gpioToGPLEV_;
    };
}

#endif