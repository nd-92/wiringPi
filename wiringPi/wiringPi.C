#include "wiringPi.H"
#include "dummyDevice.H"

using namespace WiringPi;

int main(void)
{

    wiringPi<WPI_MODE_GPIO()> wPi;

    wPi.addDevice(dummyDevice(0));

    wPi.addDevice(dummyDevice(8));

    wPi.printPinRegistry();

    return 0;
}