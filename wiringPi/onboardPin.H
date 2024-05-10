#ifndef __WIRING_PI_onboardPin_H
#define __WIRING_PI_onboardPin_H

// #include "wiringPiIncludes.H"
// #include "wiringPiDefines.H"
#include "wiringPi.H"
#include "wiringPiNode.H"

// GPIO device occupying 1 pin

namespace WiringPi
{
    class onboardPin : public wiringPiNode
    {
    public:
        constexpr onboardPin(
            const size_t pinBaseVal)
            : pinBase_(pinBaseVal){};

        // inline void digitalWrite() const
        // {
        //     std::cout << deviceName() << " digitalWrite writing 99" << std::endl;
        // }

        inline gpio_t digitalRead(const gpio_t *gpioPtr, const pin_t gpioToGPLEV) const
        {
            // Unsure exactly what this expression does
            // Can parts of this be computed ahead of time?
            if ((*(gpioPtr + gpioToGPLEV) & (1 << (static_cast<pin_t>(pinBase_) & 31))) != 0)
            {
                // std::cout << deviceName() << " digitalRead returning 1" << std::endl;
                return HIGH<gpio_t>();
            }
            else
            {
                // std::cout << deviceName() << " digitalRead returning 0" << std::endl;
                return LOW<gpio_t>();
            }

            // const int k = (*(gpioPtr + gpioToGPLEV[pin]) & (1 << (static_cast<pin_t>(pin) & 31)));
            // return std::min(k, k / k);
        }

        inline void digitalWrite(gpio_t *gpioPtr, const gpio_t offsetValue)
        {
            // if (value == LOW())
            // {
            //     *(gpioPtr + gpioToGPCLR[pinBase_]) = 1 << (pinBase_ & 31);
            // }
            // else
            // {
            //     *(gpioPtr + gpioToGPSET[pinBase_]) = 1 << (pinBase_ & 31);
            // }
            // std::cout << "Writing 1 << (pinBase_ & 31) = " << 1 << (pinBase_ & 31) << std::endl;
            *(gpioPtr + offsetValue) = 1 << (pinBase_ & 31);
        }

        inline void pwmWrite(int *pwmPtr, const int offsetValue, const int value)
        {
            *(pwmPtr + offsetValue) = value;
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
    };
}

#endif