#include "wiringPi.H"
#include "dummyDevice.H"

using namespace WiringPi;

int main(void)
{

    // wiringPiSetupGpio();

    // std::cout << "retVal = " << retVal << std::endl;

    // pullUpDnControl(2, 1);
    // for (int i = 1; i < 41; i++)
    // {
    //     pinMode(i, INPUT<int>());
    // }

    wiringPi wPi;

    std::cout << wPi.micros() << " microseconds" << std::endl;

    for (size_t i = 0; i < 40; i++)
    {
        static_cast<void>(wPi.digitalReadOnboard(i));
    }

    for (size_t i = 0; i < 40; i++)
    {
        wPi.digitalWriteOnboard(i, 0);
    }

    std::cout << wPi.micros() << " microseconds" << std::endl;

    std::cout << wPi.onboardNames() << std::endl;

    // wPi.addDevice(dummyDevice(0, 0));

    // wPi.addDevice(dummyDevice(8, 0));

    // wPi.printPinRegistry();

    // for (size_t i = 1; i < 41; i++)
    // {
    //     wPi.pullUpDnControlOnboard(i, 1);
    // }

    // wPi.pullUpDnControlOnboard(2, 1);

    return 0;
}