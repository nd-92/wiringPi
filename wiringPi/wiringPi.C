#include "wiringPi.H"
// #include "wiringPiNode.H"
#include "dummyDevice.H"
#include "GPIO.H"

using namespace WiringPi;

// inline constexpr pin_t h(const pin_t i)
// {
//     return 7 + (i / 64) + (i / 32);
//     // return 7 + (i / 32);
// }

inline constexpr size_t g(const size_t i)
{
    return 13 + (i / 32);
}

int main(void)
{

    wiringPi<WPI_MODE_GPIO()> wPi;

    wPi.addDevice(dummyDevice(0));

    wPi.addDevice(dummyDevice(8));

    wPi.printPinRegistry();

    std::vector<wiringPiNode> f = {GPIO(0), dummyDevice(0), dummyDevice(8)};

    for (size_t i = 0; i < 3; i++)
    {
        std::cout << f[i].deviceName() << ".digitalRead() = " << f[1].digitalRead() << std::endl;
    }

    std::cout << "wpi.readOnboard(1) = " << wPi.digitalReadOnboard(1) << std::endl;
    // wPi.digitalWriteOnboard(63, 0);

    return 0;
}