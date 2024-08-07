#include "wiringPi.H"
// #include "dummyDevice.H"
// #include <cmath>

using namespace WiringPi;

int main(void)
{
    wiringPi wPi;

    // for (pin_t pin = 0; pin < 32; pin++)
    // {
    //     const gpio_t j = wPi.digitalReadOnboard(pin);
    //     std::cout << "digitalReadOnboard returned " << j << " from pin " << wPi.pinMap()[pin] << std::endl;
    // }

    // for (pin_t pin = 0; pin < 32; pin++)
    // {
    //     wPi.digitalWriteOnboard(pin, 0);
    //     std::cout << "digitalWriteOnboard wrote " << 0 << " to pin " << wPi.pinMap()[pin] << std::endl;
    // }

    for (pin_t pin = 0; pin < 32; pin++)
    {
        wPi.digitalWriteOnboard(pin, 1);
        std::cout << "digitalWriteOnboard wrote " << 1 << " to pin " << wPi.pinMap()[pin] << std::endl;
    }

    for (pin_t pin = 0; pin < 32; pin++)
    {
        const gpio_t j = wPi.digitalReadOnboard(pin);
        std::cout << "digitalReadOnboard returned " << j << " from pin " << wPi.pinMap()[pin] << std::endl;
    }

    return 0;
}