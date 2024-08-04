#ifndef __WIRING_PI_onboardPins_H
#define __WIRING_PI_onboardPins_H

// #include "wiringPiIncludes.H"
// #include "wiringPiDefines.H"
#include "wiringPi.H"
#include "wiringPiNode.H"

// GPIO device occupying 1 pin
// Default constructor allows setting of mode
// Only acceptable modes are INPUT/OUTPUT for now

namespace WiringPi
{
    [[nodiscard]] inline consteval std::array<gpio_t, 40> pinModeI()
    {
        return std::array<gpio_t, 40>{
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //
    }

    [[nodiscard]] inline consteval std::array<gpio_t, 40> pullRegI()
    {
        std::array<gpio_t, 40> arr;
        for (size_t i = 0; i < 40; i++)
        {
            arr[i] = GPPUPPDN0<gpio_t>() + (i >> 4);
        }
        return arr;
    }

    [[nodiscard]] inline consteval std::array<gpio_t, 40> pullShiftI()
    {
        std::array<gpio_t, 40> arr;
        for (size_t i = 0; i < 40; i++)
        {
            arr[i] = (i & 0xf) << 1;
        }
        return arr;
    }

    class onboardPins
    {
    public:
        [[nodiscard]] inline constexpr onboardPins() {};

        // inline void digitalWrite() const
        // {
        //     std::cout << deviceName() << " digitalWrite writing 99" << std::endl;
        // }

        // Offset to the GPIO Input level registers for each GPIO pin
        // This is equivalent to the gpioToGPLEV table
        // Returns 13 on pins 0 -> 31, 14 on pins 32 -> 63
        [[nodiscard]] inline constexpr gpio_t gpioToGPLEV(const pin_t i) const
        {
            return 13 + (i / 32);
        }

        // Offset to the GPIO Clear and Set registers for each GPIO pin
        // This is equivalent to the gpioToGPCLR and gpioToGPSET tables
        // Value passed to this function should be deviceNo + (64 * !value)
        // Returns 10 -> 11 for digital value LOW, 7 -> 8 for digital value HIGH
        [[nodiscard]] inline constexpr gpio_t gpioCLRSET(const pin_t i) const
        {
            return 7 + (i / 64) + (i / 32);
        }

        [[nodiscard]] inline gpio_t digitalRead(const volatile gpio_t *gpioPtr, const pin_t pinNumber) const
        {
            // Unsure exactly what this expression does
            // Can parts of this be computed ahead of time?
            if ((*(gpioPtr + gpioToGPLEV(pinNumber)) & (1 << (static_cast<pin_t>(pinNumber) & 31))) != 0)
            {
                std::cout << "Returning HIGH from address " << &gpioPtr + gpioToGPLEV(pinNumber) << std::endl;
                return HIGH<gpio_t>();
            }
            else
            {
                std::cout << "Returning LOW from address " << &gpioPtr + gpioToGPLEV(pinNumber) << std::endl;
                return LOW<gpio_t>();
            }
        }

        inline void digitalWrite(volatile gpio_t *gpioPtr, const pin_t pinNumber, const gpio_t value)
        {
            std::cout << "Setting " << (1 << (pinNumber & 31)) << " on address " << &gpioPtr + gpioCLRSET(pinNumber + (64 * !value)) << std::endl;
            // std::cout << "Setting " << 1 << (pinNumber & 31) << " on address " << gpioPtr << std::endl;
            // *(gpioPtr + gpioCLRSET(pinNumber + (64 * !value))) = 1 << (pinNumber & 31);
        }

        // inline void pwmWrite(gpio_t *pwmPtr, const gpio_t offsetValue, const gpio_t value)
        // {
        //     *(pwmPtr + offsetValue) = value;
        // }

        // // TEMPLATE THIS FOR PIGPIOPUPOFFSET -> GET RID OF BRANCH
        // inline void pullUpDnControl(gpio_t *gpioPtr, const gpio_t value, const int piGpioPupOffset)
        // {
        //     std::cout << "piGpioPupOffset = " << piGpioPupOffset << std::endl;
        //     if (piGpioPupOffset == GPPUPPDN0<int>())
        //     {
        //         // Pi 4B pull up/down method
        //         // static constexpr const gpio_t pullreg = GPPUPPDN0<gpio_t>() + (pinBase_ >> 4);
        //         // static constexpr const gpio_t pullshift = (pinBase_ & 0xf) << 1;
        //         gpio_t pull;

        //         switch (value)
        //         {
        //         case PUD_OFF<gpio_t>():
        //             pull = 0;
        //             break;
        //         case PUD_UP<gpio_t>():
        //             pull = 1;
        //             break;
        //         case PUD_DOWN<gpio_t>():
        //             pull = 2;
        //             break;
        //         default:
        //             return; /* An illegal value */
        //         }

        //         gpio_t pullbits = *(gpioPtr + pullReg_);
        //         pullbits &= pullbitsModuloVal_;
        //         pullbits |= (pull << pullShift_);
        //         //*(gpioPtr + pullReg_) = pullbits;
        //         std::cout << "pullBits = " << pullbits << std::endl;
        //     }
        //     else
        //     {
        //         // legacy pull up/down method
        //         // *(gpioPtr + GPPUD<gpio_t>()) = value & 3;
        //         // delayMicroseconds(5);
        //         // *(gpioPtr + offsetValue) = 1 << (pinBase_ & 31);
        //         // delayMicroseconds(5);

        //         // *(gpioPtr + GPPUD<gpio_t>()) = 0;
        //         // delayMicroseconds(5);
        //         // *(gpioPtr + offsetValue) = 0;
        //         // delayMicroseconds(5);
        //     }
        // }

        [[nodiscard]] inline consteval std::string_view deviceName() const
        {
            return "GPIO";
        }

        // inline constexpr size_t pinBase() const
        // {
        //     return pinBase_;
        // }

        // inline constexpr size_t numPins() const
        // {
        //     return numPins_;
        // }

        // inline constexpr size_t pinMax() const
        // {
        //     return pinMax_;
        // }

    private:
        static constexpr const std::array<pin_t, 40> pinMode_ = pinModeI();
        static constexpr const std::array<gpio_t, 40> pullReg_ = pullRegI();
        static constexpr const std::array<gpio_t, 40> pullShift_ = pullShiftI();
        // static constexpr const gpio_t pullReg_ = GPPUPPDN0<gpio_t>() + (pinBase_ >> 4);
        //  static constexpr const gpio_t pullShift_ = (pinBase_ & 0xf) << 1;
        //  static constexpr const gpio_t pullbitsModuloVal_ = static_cast<gpio_t>(~(3 << pullShift_));
    };
}
#endif