#include "wiringPi.H"

using namespace WiringPi;

int main(void)
{
    wiringPi wPi;

    for (pin_t pin = 0; pin < 32; pin++)
    {
        wPi.pinModeOnboard(pin, INPUT<gpio_t>());
    }

    for (pin_t pin = 0; pin < 32; pin++)
    {
        std::cout << "Pin " << pin << " mode: " << wPi.pinModeMap()[pin] << std::endl;
    }

    for (pin_t pin = 0; pin < 32; pin++)
    {
        wPi.pinModeOnboard(pin, OUTPUT<gpio_t>());
    }

    for (pin_t pin = 0; pin < 32; pin++)
    {
        std::cout << "Pin " << pin << " mode: " << wPi.pinModeMap()[pin] << std::endl;
    }

    return 0;
}