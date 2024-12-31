#include "wiringPi.H"
#include "wiringSerial.H"
#include "argHandler.H"
#include "bmp180.H"

using namespace WiringPi;

#define COMPILE_WIRINGPI_TESTS

#ifdef COMPILE_WIRINGPI_TESTS
int main(const int argc, const char *argv[])
#else
int main()
#endif
{

    // constexpr const gpio_t A = a;

    // std::cout << A << std::endl;

    // Try wiringShift
    // {
    //     // Create the wiringPi object
    //     wiringPi<wiringPiModes::pins()> RaspberryPi;
    //     const gpio_t i = RaspberryPi.shiftIn<0, 1, shift::MSBFIRST()>();
    //     std::cout << "i = " << i << std::endl;

    //     const gpio_t j = RaspberryPi.shiftIn<0, 1, shift::LSBFIRST()>();
    //     std::cout << "j = " << j << std::endl;
    // }

#ifndef COMPILE_WIRINGPI_TESTS

    // Try mcp23008
    {
        // Define a pin
        constexpr const pin_t myPin = 0x77;
        constexpr const pin_t temperatureChannel = myPin;
        constexpr const pin_t pressureChannel = myPin + 1;

        // Initialise wiringPi on the correct mode
        wiringPi<Pi::models::Pi4B, Pi::layout::DEFAULT, wiringPiModes::pins> RaspberryPi;

        // Create a bmp180 sensor
        bmp180<myPin, RaspberryPi.piHardwareInfo().model(), RaspberryPi.piHardwareInfo().gpioLayout(), RaspberryPi.mode()> sensor(RaspberryPi);

        // Set the altitude to 100
        sensor.analogWrite(pin_constant<myPin>(), 100);

        // Get the temperature
        const gpio_t temperature = sensor.analogRead(pin_constant<temperatureChannel>());
        std::cout << "Temperature: " << (temperature - 273.15) * 0.1 << " K" << std::endl;

        // Get the pressure
        const gpio_t pressure = sensor.analogRead(pin_constant<pressureChannel>());
        std::cout << "Pressure: " << pressure << " Pa" << std::endl;
    }

#endif

#ifdef COMPILE_WIRINGPI_TESTS

    // Handle the input arguments
    const argHandler args(argc, argv);

    // Test softPwm implementation
    if (args.threadTest())
    {
        constexpr const pin_t myPin = 31;
        constexpr const pin_t initialValue = 10;
        constexpr const pin_t writeValue = 50;
        constexpr const pin_t pwmRange = 100;
        if ((args.wiringPiMode() == wiringPiModes::pins) | (args.wiringPiMode() == wiringPiModes::all))
        {
            // Create the wiringPi object
            wiringPi<Pi::models::Pi4B, Pi::layout::DEFAULT, wiringPiModes::pins> RaspberryPi;

            // Create a softPwm thread
            RaspberryPi.softPwm<myPin>(initialValue, pwmRange);

            // Wait for 2 seconds and then write
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            RaspberryPi.softPwmWrite<myPin>(writeValue);

            // Wait for 2 seconds and then end
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            RaspberryPi.softPwmEnd<myPin>();
            std::cout << "Software PWM thread test on mode pins passed on pin " << myPin << std::endl;
        }
        if ((args.wiringPiMode() == wiringPiModes::gpio) | (args.wiringPiMode() == wiringPiModes::all))
        {
            // Create the wiringPi object
            wiringPi<Pi::models::Pi4B, Pi::layout::DEFAULT, wiringPiModes::gpio> RaspberryPi;

            // Create a softPwm thread
            RaspberryPi.softPwm<myPin>(initialValue, pwmRange);

            // Wait for 2 seconds and then write
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            RaspberryPi.softPwmWrite<myPin>(writeValue);

            // Wait for 2 seconds and then end
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            RaspberryPi.softPwmEnd<myPin>();
            std::cout << "Software PWM thread test on mode gpio passed on pin " << myPin << std::endl;
        }
        if ((args.wiringPiMode() == wiringPiModes::phys) | (args.wiringPiMode() == wiringPiModes::all))
        {
            // Create the wiringPi object
            wiringPi<Pi::models::Pi4B, Pi::layout::DEFAULT, wiringPiModes::phys> RaspberryPi;

            // Create a softPwm thread
            RaspberryPi.softPwm<myPin>(initialValue, pwmRange);

            // Wait for 2 seconds and then write
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            RaspberryPi.softPwmWrite<myPin>(writeValue);

            // Wait for 2 seconds and then end
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            RaspberryPi.softPwmEnd<myPin>();
            std::cout << "Software PWM thread test on mode phys passed on pin " << myPin << std::endl;
        }
    }

    // Write the pin layout
    if (args.pinLayout())
    {
        if ((args.wiringPiMode() == wiringPiModes::pins) | (args.wiringPiMode() == wiringPiModes::all))
        {
            wiringPi<Pi::models::Pi4B, Pi::layout::DEFAULT, wiringPiModes::pins> RaspberryPi;
            RaspberryPi.pinLayout();
        }
        if ((args.wiringPiMode() == wiringPiModes::gpio) | (args.wiringPiMode() == wiringPiModes::all))
        {
            wiringPi<Pi::models::Pi4B, Pi::layout::DEFAULT, wiringPiModes::gpio> RaspberryPi;
            RaspberryPi.pinLayout();
        }
        if ((args.wiringPiMode() == wiringPiModes::phys) | (args.wiringPiMode() == wiringPiModes::all))
        {
            wiringPi<Pi::models::Pi4B, Pi::layout::DEFAULT, wiringPiModes::phys> RaspberryPi;
            RaspberryPi.pinLayout();
        }
    }

    // Perform the read/write unit test
    if (args.readWriteTest())
    {
        if ((args.wiringPiMode() == wiringPiModes::pins) | (args.wiringPiMode() == wiringPiModes::all))
        {
            wiringPi<Pi::models::Pi4B, Pi::layout::DEFAULT, wiringPiModes::pins> RaspberryPi;
            if (RaspberryPi.readWriteTest() == unit_test_pass())
            {
                std::cout << "Unit test passed on mode PINS" << std::endl;
            }
            else
            {
                std::cout << "Unit test failed on mode PINS" << std::endl;
            }
        }
        if ((args.wiringPiMode() == wiringPiModes::gpio) | (args.wiringPiMode() == wiringPiModes::all))
        {
            wiringPi<Pi::models::Pi4B, Pi::layout::DEFAULT, wiringPiModes::gpio> RaspberryPi;
            if (RaspberryPi.readWriteTest() == unit_test_pass())
            {
                std::cout << "Unit test passed on mode GPIO" << std::endl;
            }
            else
            {
                std::cout << "Unit test failed on mode GPIO" << std::endl;
            }
        }
        if ((args.wiringPiMode() == wiringPiModes::phys) | (args.wiringPiMode() == wiringPiModes::all))
        {
            wiringPi<Pi::models::Pi4B, Pi::layout::DEFAULT, wiringPiModes::phys> RaspberryPi;
            if (RaspberryPi.readWriteTest() == unit_test_pass())
            {
                std::cout << "Unit test passed on mode PHYS" << std::endl;
            }
            else
            {
                std::cout << "Unit test failed on mode PHYS" << std::endl;
            }
        }
    }

#endif

    // No matter what, create a wiringPi object and reset all pins before exit
    // wiringPi<wiringPiModes::pins()> RaspberryPi;
    // RaspberryPi.resetPins();

    return 0;
}