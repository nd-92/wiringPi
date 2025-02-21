#include "wiringPi.H"
#include "argHandler.H"

using namespace WiringPi;

// #define DO_THIS_BIT

#ifdef DO_THIS_BIT
int main(const int argc, const char *argv[])
#else
int main()
#endif
{
    // ADS1115 test
    {
        // Define a pin
        constexpr const pin_constant<70> myPin;

        // Initialise wiringPi on the correct mode
        wiringPi<Pi::model::Pi4B, Pi::layout::DEFAULT, wiringPiModes::pins> RaspberryPi;

        ADS1115 sensor(myPin, RaspberryPi);

        std::cout << sensor.data(pin_constant<0>()) << std::endl;
        std::cout << sensor.data(pin_constant<1>()) << std::endl;
        std::cout << sensor.dataRate(pin_constant<6>()) << std::endl;

        sensor.analogWrite(myPin, 0);
        sensor.digitalWrite(myPin, 0);

        std::cout << sensor.analogRead(myPin) << std::endl;
    }

    // SPI test
    // {
    //     // Initialise wiringPi on the correct mode
    //     wiringPi<Pi::model::Pi4B, Pi::layout::DEFAULT, wiringPiModes::pins> RaspberryPi;

    //     // Create an SPI handler
    //     SPI spi(RaspberryPi);
    // }

#ifdef DO_THIS_BIT

    // Handle the input arguments
    const argHandler args(argc, argv);

    // Do the I2C test
    if (args.i2cTest())
    {
        // Define a pin
        constexpr const pin_constant<0x77> myPin;
        constexpr const pin_t temperatureChannel = myPin;
        constexpr const pin_t pressureChannel = myPin + 1;

        // Initialise wiringPi on the correct mode
        wiringPi<Pi::model::Pi4B, Pi::layout::DEFAULT, wiringPiModes::pins> RaspberryPi;

        // Create a BMP180 sensor
        BMP180 sensor(pin_constant<myPin>(), RaspberryPi);

        // Set the altitude to 100
        sensor.analogWrite(pin_constant<myPin>(), 100);

        // Get the temperature
        const gpio_t temperature = sensor.analogRead(pin_constant<temperatureChannel>());
        std::cout << "Temperature: " << temperature * 10.0 << " K" << std::endl;

        // Get the pressure
        const gpio_t pressure = sensor.analogRead(pin_constant<pressureChannel>());
        std::cout << "Pressure: " << pressure << " Pa" << std::endl;
    }

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
            wiringPi<Pi::model::Pi4B, Pi::layout::DEFAULT, wiringPiModes::pins> RaspberryPi;

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
            wiringPi<Pi::model::Pi4B, Pi::layout::DEFAULT, wiringPiModes::gpio> RaspberryPi;

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
            wiringPi<Pi::model::Pi4B, Pi::layout::DEFAULT, wiringPiModes::phys> RaspberryPi;

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
            wiringPi<Pi::model::Pi4B, Pi::layout::DEFAULT, wiringPiModes::pins> RaspberryPi;
            RaspberryPi.pinLayout();
        }
        if ((args.wiringPiMode() == wiringPiModes::gpio) | (args.wiringPiMode() == wiringPiModes::all))
        {
            wiringPi<Pi::model::Pi4B, Pi::layout::DEFAULT, wiringPiModes::gpio> RaspberryPi;
            RaspberryPi.pinLayout();
        }
        if ((args.wiringPiMode() == wiringPiModes::phys) | (args.wiringPiMode() == wiringPiModes::all))
        {
            wiringPi<Pi::model::Pi4B, Pi::layout::DEFAULT, wiringPiModes::phys> RaspberryPi;
            RaspberryPi.pinLayout();
        }
    }

    // Perform the read/write unit test
    if (args.readWriteTest())
    {
        if ((args.wiringPiMode() == wiringPiModes::pins) | (args.wiringPiMode() == wiringPiModes::all))
        {
            wiringPi<Pi::model::Pi4B, Pi::layout::DEFAULT, wiringPiModes::pins> RaspberryPi;
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
            wiringPi<Pi::model::Pi4B, Pi::layout::DEFAULT, wiringPiModes::gpio> RaspberryPi;
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
            wiringPi<Pi::model::Pi4B, Pi::layout::DEFAULT, wiringPiModes::phys> RaspberryPi;
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

    return 0;
}