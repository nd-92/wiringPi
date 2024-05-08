#include "wiringPi.H"
// #include "wiringPiNode.H"
#include "dummyDevice.H"
#include "GPIO.H"

using namespace WiringPi;

int main(void)
{

    wiringPi<WPI_MODE_GPIO<int>()> wPi;

    const dummyDevice dev(0);

    const dummyDevice dev2(8);

    wPi.addDevice(dev);

    wPi.addDevice(dev2);

    wPi.printPinRegistry();

    std::cout << "wpi.readOffboard(1) = " << wPi.digitalReadOffboard(1) << std::endl;
    wPi.digitalWriteOnboard(1, 0);
    wPi.digitalWriteOffboard(1, 0);

    // for (size_t i = 0; i < 64; i++)
    // {
    //     if (((i % 8) == 0) && (i > 0))
    //     {
    //         std::cout << std::endl;
    //     }
    //     std::cout << wPi.pinToGpio()[i] << " ";
    // }
    // std::cout << std::endl;
    // std::cout << std::endl;
    // for (size_t i = 0; i < 64; i++)
    // {
    //     if (((i % 8) == 0) && (i > 0))
    //     {
    //         std::cout << std::endl;
    //     }
    //     std::cout << wPi.sysFds()[i] << " ";
    // }
    // std::cout << std::endl;

    return 0;
}