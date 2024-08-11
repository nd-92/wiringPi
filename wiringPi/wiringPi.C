#include "wiringPi.H"

using namespace WiringPi;

int main(void)
{
    // wiringPi<WPI_MODE_PINS()> Pi;

    wiringPi<WPI_MODE_GPIO()> Pi;

    // Pi.readAll();

    Pi.pinModeOnboard<31>(OUTPUT<gpio_t>());

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