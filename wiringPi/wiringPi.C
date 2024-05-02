#include "wiringPi.H"
#include "wiringPiNode.H"
#include "dummyDevice.H"

int main(void)
{

    WiringPi::wiringPi<WPI_MODE_GPIO_SYS<int>()> wPi;

    WiringPi::dummyDevice<11> dev;

    WiringPi::dummyDevice<9> dev2;

    wPi.addDevice(dev.pinBase(), dev.numPins());

    wPi.addDevice(dev2.pinBase(), dev2.numPins());

    // const bool i = wPi.validateDeviceRegistry();

    wPi.printDeviceRegistry();

    // std::cout << i << std::endl;

    return 0;
}