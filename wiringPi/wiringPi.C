#include "wiringPi.H"
#include "argHandler.H"

using namespace WiringPi;

int main(const int argc, const char *argv[])
{
    // Handle the input arguments
    const argHandler args(argc, argv);

    // Write the pin layout
    if (args.pinLayout())
    {
        if ((args.wiringPiMode() == wiringPiModes::pins()) | (args.wiringPiMode() == wiringPiModes::all()))
        {
            wiringPi<wiringPiModes::pins()> Pi;
            Pi.pinLayout();
        }
        if ((args.wiringPiMode() == wiringPiModes::gpio()) | (args.wiringPiMode() == wiringPiModes::all()))
        {
            wiringPi<wiringPiModes::gpio()> Pi;
            Pi.pinLayout();
        }
        if ((args.wiringPiMode() == wiringPiModes::phys()) | (args.wiringPiMode() == wiringPiModes::all()))
        {
            wiringPi<wiringPiModes::phys()> Pi;
            Pi.pinLayout();
        }
    }

    // Perform the read/write unit test
    if (args.readWriteTest())
    {
        if ((args.wiringPiMode() == wiringPiModes::pins()) | (args.wiringPiMode() == wiringPiModes::all()))
        {
            wiringPi<wiringPiModes::pins()> Pi;
            if (Pi.readWriteTest() == unit_test_pass())
            {
                std::cout << "Unit test passed on mode PINS" << std::endl;
            }
            else
            {
                std::cout << "Unit test failed on mode PINS" << std::endl;
            }
        }
        if ((args.wiringPiMode() == wiringPiModes::gpio()) | (args.wiringPiMode() == wiringPiModes::all()))
        {
            wiringPi<wiringPiModes::gpio()> Pi;
            if (Pi.readWriteTest() == unit_test_pass())
            {
                std::cout << "Unit test passed on mode GPIO" << std::endl;
            }
            else
            {
                std::cout << "Unit test failed on mode GPIO" << std::endl;
            }
        }
        if ((args.wiringPiMode() == wiringPiModes::phys()) | (args.wiringPiMode() == wiringPiModes::all()))
        {
            wiringPi<wiringPiModes::phys()> Pi;
            if (Pi.readWriteTest() == unit_test_pass())
            {
                std::cout << "Unit test passed on mode PHYS" << std::endl;
            }
            else
            {
                std::cout << "Unit test failed on mode PHYS" << std::endl;
            }
        }
    }

    return 0;
}