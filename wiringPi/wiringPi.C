#include "wiringPi.H"

using namespace WiringPi;

int main(void)
{
    wiringPi Pi;

    Pi.readAll();

    // for (pin_t pin = 0; pin < 32; pin++)
    // {
    //     Pi.pinModeOnboard(pin, OUTPUT<gpio_t>());
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