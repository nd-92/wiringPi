#include "wiringPi.H"

using namespace WiringPi;

double getWallTime()
{
    struct timeval time;
    if (gettimeofday(&time, NULL))
    {
        return 0;
    }
    else
    {
        return static_cast<double>(time.tv_sec) + static_cast<double>(time.tv_usec) * .000001;
    }
}

int main(void)
{
    // wiringPi<WPI_MODE_PINS()> Pi;

    // const double i = getWallTime();

    wiringPi<WPI_MODE_PINS()> Pi;

    // const double i = getWallTime();
    // // size_t counter = 0;
    // //  while (counter < 10000000)
    // // for (size_t counter = 0; counter < 10000000; counter++)
    // // {
    // //     Pi.resetPins();
    // // }

    // for (pin_t pin = 0; pin < 1000; pin++)
    // {
    //     std::cout << Pi.digitalReadOnboard<3>() << std::endl;
    // }

    // const double j = getWallTime();

    // std::cout << "Time = " << j - i << std ::endl;

    // Pi.readAll();

    // Pi.pinModeOnboard<31>(OUTPUT<gpio_t>());

    // const double i_ = getWallTime();

    Pi.pullUpDnControlOnboard<31>(PUD_OFF<int>());

    // const double j_ = getWallTime();

    // std::cout << "Time = " << j - i << std ::endl;

    // Pi.pullUpDnControlOnboard<30>(PUD_OFF<int>());

    // constexpr pin_t pinNumber = 40;

    // const std::string toPrint = std::string("Pin number " + std::to_string(pinNumber) + " is not onboard (onboard pins 0 -> 31)");

    // std::cout << toPrint << std::endl;

    // for (pin_t pin = 0; pin < 32; pin++)
    // {
    //     Pi.pinModeOnboard(pin, OUTPUT<gpio_t>());
    // }

    // for (pin_t pin = 0; pin < 32; pin++)
    // {
    // constexpr pin_t pin = 18;
    // std::cout << "Digital read returned " << Pi.digitalReadOnboard<pin>() << " on pin " << pin << std::endl;
    // }

    // for (pin_t pin = 0; pin < 64; pin++)
    // {
    //     std::cout << "Name:" << Pi.physNames()[pin] << std::endl;
    // }

    // while (Pi.run())
    // {
    //     sleep(1);
    //     // if (quit.load())
    //     // {
    //     //     break;
    //     // }
    // }

    // for (pin_t pin = 0; pin < 32; pin++)
    // {
    //     std::cout << "Pin " << pin << " mode: " << Pi.pinModeMap()[pin] << std::endl;
    // }

    return 0;
}