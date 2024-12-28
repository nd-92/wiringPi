#include "wiringPi.H"
#include "wiringSerial.H"
#include "argHandler.H"

using namespace WiringPi;

int main(const int argc, const char *argv[])
{
    // Handle the input arguments
    const argHandler args(argc, argv);

    // Test softPwm implementation
    if (args.threadTest())
    {
        constexpr const pin_t myPin = 31;
        constexpr const pin_t initialValue = 10;
        constexpr const pin_t writeValue = 50;
        constexpr const pin_t pwmRange = 100;
        if ((args.wiringPiMode() == wiringPiModes::pins()) | (args.wiringPiMode() == wiringPiModes::all()))
        {
            // Create the wiringPi object
            wiringPi<wiringPiModes::pins()> Pi;

            // Create a softPwm thread
            Pi.softPwm<myPin>(initialValue, pwmRange);

            // Wait for 2 seconds and then write
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            Pi.softPwmWrite<myPin>(writeValue);

            // Wait for 2 seconds and then end
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            Pi.softPwmEnd<myPin>();
            std::cout << "Software PWM thread test on mode pins passed on pin " << myPin << std::endl;
        }
        if ((args.wiringPiMode() == wiringPiModes::gpio()) | (args.wiringPiMode() == wiringPiModes::all()))
        {
            // Create the wiringPi object
            wiringPi<wiringPiModes::gpio()> Pi;

            // Create a softPwm thread
            Pi.softPwm<myPin>(initialValue, pwmRange);

            // Wait for 2 seconds and then write
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            Pi.softPwmWrite<myPin>(writeValue);

            // Wait for 2 seconds and then end
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            Pi.softPwmEnd<myPin>();
            std::cout << "Software PWM thread test on mode gpio passed on pin " << myPin << std::endl;
        }
        if ((args.wiringPiMode() == wiringPiModes::phys()) | (args.wiringPiMode() == wiringPiModes::all()))
        {
            // Create the wiringPi object
            wiringPi<wiringPiModes::phys()> Pi;

            // Create a softPwm thread
            Pi.softPwm<myPin>(initialValue, pwmRange);

            // Wait for 2 seconds and then write
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            Pi.softPwmWrite<myPin>(writeValue);

            // Wait for 2 seconds and then end
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            Pi.softPwmEnd<myPin>();
            std::cout << "Software PWM thread test on mode phys passed on pin " << myPin << std::endl;
        }
    }

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