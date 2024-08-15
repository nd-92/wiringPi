#include "wiringPi.H"

using namespace WiringPi;

int main(void)
{
    wiringPi<WPI_MODE_GPIO()> Pi;

    // Pi.resetPins();

    // Pi.gpioClockSet<0>(10);
    // Pi.gpioClockSet<1>(10);
    // Pi.gpioClockSet<2>(10);
    // Pi.gpioClockSet<3>(10);
    // Pi.gpioClockSet<4>(10);
    // Pi.gpioClockSet<5>(10);
    // Pi.gpioClockSet<6>(10);
    Pi.gpioClockSet<4>(10);
    // Pi.gpioClockSet<8>(10);
    // Pi.gpioClockSet<9>(10);
    // Pi.gpioClockSet<10>(10);
    // Pi.gpioClockSet<11>(10);
    // Pi.gpioClockSet<12>(10);
    // Pi.gpioClockSet<13>(10);
    // Pi.gpioClockSet<14>(10);
    // Pi.gpioClockSet<15>(10);
    // Pi.gpioClockSet<16>(10);
    // Pi.gpioClockSet<17>(10);
    // Pi.gpioClockSet<18>(10);
    // Pi.gpioClockSet<19>(10);
    // Pi.gpioClockSet<20>(10);
    // Pi.gpioClockSet<21>(10);
    // Pi.gpioClockSet<22>(10);
    // Pi.gpioClockSet<23>(10);
    // Pi.gpioClockSet<24>(10);
    // Pi.gpioClockSet<25>(10);
    // Pi.gpioClockSet<26>(10);
    // Pi.gpioClockSet<27>(10);
    // Pi.gpioClockSet<28>(10);
    // Pi.gpioClockSet<29>(10);
    // Pi.gpioClockSet<30>(10);
    // Pi.gpioClockSet<31>(10);

    // Pi.writePinsLow();

    // Pi.readPins();

    // Pi.pullUpDnControlPins();

    // for (pin_t i = 0; i < 32; i++)
    // {
    //     std::cout << "digitalReadOnboard returned " << Pi.digitalReadOnboard<i>() << std::endl;
    // }

    // while (Pi.run())
    // {

    // Pi.pwmSetMode<PWM_MODE_BAL<int>()>();

    // Pi.pwmSetMode<PWM_MODE_MS<int>()>();

    // Pi.pwmSetMode<3>();
    // }

    return 0;
}