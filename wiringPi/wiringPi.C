#include "wiringPi.H"

using namespace WiringPi;

int main(void)
{
    wiringPi<WPI_MODE_GPIO()> Pi;

    Pi.pwmSetMode<PWM_MODE_BAL<int>()>();

    Pi.pwmSetMode<PWM_MODE_MS<int>()>();

    // Pi.pwmSetMode<3>();

    return 0;
}