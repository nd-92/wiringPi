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
        if ((args.wiringPiMode() == WPI_MODE_PINS()) | (args.wiringPiMode() == WPI_MODE_ALL()))
        {
            wiringPi<WPI_MODE_PINS()> Pi;
            Pi.pinLayout();
        }
        if ((args.wiringPiMode() == WPI_MODE_GPIO()) | (args.wiringPiMode() == WPI_MODE_ALL()))
        {
            wiringPi<WPI_MODE_GPIO()> Pi;
            Pi.pinLayout();
        }
        if ((args.wiringPiMode() == WPI_MODE_PHYS()) | (args.wiringPiMode() == WPI_MODE_ALL()))
        {
            wiringPi<WPI_MODE_PHYS()> Pi;
            Pi.pinLayout();
        }
    }

    // Perform the read/write unit test
    if (args.readWriteTest())
    {
        if ((args.wiringPiMode() == WPI_MODE_PINS()) | (args.wiringPiMode() == WPI_MODE_ALL()))
        {
            wiringPi<WPI_MODE_PINS()> Pi;
            if (Pi.readWriteTest() == unit_test_pass())
            {
                std::cout << "Unit test passed on mode PINS" << std::endl;
            }
            else
            {
                std::cout << "Unit test failed on mode PINS" << std::endl;
            }
        }
        if ((args.wiringPiMode() == WPI_MODE_GPIO()) | (args.wiringPiMode() == WPI_MODE_ALL()))
        {
            wiringPi<WPI_MODE_GPIO()> Pi;
            if (Pi.readWriteTest() == unit_test_pass())
            {
                std::cout << "Unit test passed on mode GPIO" << std::endl;
            }
            else
            {
                std::cout << "Unit test failed on mode GPIO" << std::endl;
            }
        }
        if ((args.wiringPiMode() == WPI_MODE_PHYS()) | (args.wiringPiMode() == WPI_MODE_ALL()))
        {
            wiringPi<WPI_MODE_PHYS()> Pi;
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