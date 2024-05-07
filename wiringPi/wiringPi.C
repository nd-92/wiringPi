#include "wiringPi.H"
// #include "wiringPiNode.H"
#include "dummyDevice.H"
#include "GPIO.H"

int main(void)
{

    WiringPi::wiringPi<WPI_MODE_GPIO<int>()> wPi;

    const WiringPi::dummyDevice dev(8);

    const WiringPi::dummyDevice dev2(17);

    // const WiringPi::GPIO gpio(0, wPi);

    // WiringPi::dummyDevice<11> dev;

    // WiringPi::dummyDevice<17> dev2;

    // WiringPi::dummyDevice<11> dev3;

    // WiringPi::dummyDevice<0> dev2;

    // wPi.addDevice(gpio);

    wPi.addDevice(dev);

    wPi.addDevice(dev2);

    // wPi.addDevice(gpio);

    // wPi.addDevice(dev2.pinBase(), dev2.numPins(), dev2.deviceName());

    // wPi.addDevice(dev3.pinBase(), dev3.numPins(), dev3.deviceName());

    // const bool i = wPi.validateDeviceRegistry();

    // dev2.digitalRead();

    // dev2.digitalWrite();

    wPi.printPinRegistry();

    // wPi.digitalRead(dev);

    // std::cout << wPi.devicePtrs_[0].digitalRead() << std::endl;

    // wPi.devicePtrs_[0].digitalWrite();

    // wPi.devicePtrs_.emplace_back(std::make_unique<WiringPi::dummyDevice>(dev));
    // wPi.devicePtrs_.emplace_back(std::make_unique<WiringPi::dummyDevice>(dev2));

    // std::cout << wPi.devicePtrs_[0]->digitalRead() << std::endl;
    // std::cout << wPi.devicePtrs_[1]->digitalRead() << std::endl;

    wPi.digitalRead(0);
    wPi.digitalRead(1);
    wPi.digitalRead(2);
    // wPi.digitalRead(3);

    // std::cout << wPi.devicePtrs_[0] << std::endl;
    // std::cout << wPi.devicePtrs_[1] << std::endl;

    // std::cout << static_cast<long long int>(66 & (0xFFFFFFC0)) << std::endl;

    // std::cout << i << std::endl;

    return 0;
}