#ifndef __WIRING_PI_argHandler_H
#define __WIRING_PI_argHandler_H

namespace WiringPi
{
    class argHandler
    {
    public:
        [[nodiscard]] argHandler(const int argc, const char *argv[])
            : nArgs_(nArgsCheck(argc, argv)),
              threadTest_(threadTestCheck(argc, argv)),
              readWriteTest_(readWriteTestCheck(argc, argv)),
              i2cTest_(i2cTestCheck(argc, argv)),
              pinLayout_(pinLayoutCheck(argc, argv)),
              wiringPiMode_(wiringPiModeCheck(argc, argv)) {};

        ~argHandler() {};

        // Return if we are doing the thread test
        [[nodiscard]] bool threadTest() const
        {
            return threadTest_;
        }

        // Return if we are printing the pin layout
        [[nodiscard]] bool pinLayout() const
        {
            return pinLayout_;
        }

        // Return if we are doing the read/write test
        [[nodiscard]] bool readWriteTest() const
        {
            return readWriteTest_;
        }

        // Return if we are doing the I2C test
        [[nodiscard]] bool i2cTest() const
        {
            return i2cTest_;
        }

        // Returns the specified mode
        [[nodiscard]] int wiringPiMode() const
        {
            return wiringPiMode_;
        }

        // Returns the number of arguments
        [[nodiscard]] std::size_t nArgs() const
        {
            return nArgs_;
        }

    private:
        // Generic function to check for arguments matching argumentString
        [[nodiscard]] bool argumentStringMatch(const int argc, const char *argv[], const char *argumentString) const
        {
            if (argc > 0)
            {
                for (std::size_t i = 0; i < static_cast<std::size_t>(argc); i++)
                {
                    if (strcmp(argv[i], argumentString) == 0)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        // Number of supplied input arguments
        const std::size_t nArgs_;
        [[nodiscard]] std::size_t nArgsCheck(const int argc, const char *argv[]) const
        {
            // Print out the user-supplied command
            if (argc > 1)
            {
                std::cout << "Executing ";
                for (std::size_t i = 0; i < static_cast<std::size_t>(argc); i++)
                {
                    std::cout << argv[i] << " ";
                }
                std::cout << std::endl;
            }

            // Check for a bad number of supplied arguments
            if (argc < 0)
            {
                return std::numeric_limits<std::size_t>::max();
            }
            else
            {
                return static_cast<std::size_t>(argc);
            }
        }

        const bool threadTest_;
        [[nodiscard]] bool threadTestCheck(const int argc, const char *argv[]) const
        {
            return argumentStringMatch(argc, argv, "-threadTest");
        }

        // Are we doing the read/write test
        const bool readWriteTest_;
        [[nodiscard]] bool readWriteTestCheck(const int argc, const char *argv[]) const
        {
            return argumentStringMatch(argc, argv, "-readWriteTest");
        }

        // Are we doing the I2C test
        const bool i2cTest_;
        [[nodiscard]] bool i2cTestCheck(const int argc, const char *argv[]) const
        {
            return argumentStringMatch(argc, argv, "-i2cTest");
        }

        // Are we doing the layout
        const bool pinLayout_;
        [[nodiscard]] bool pinLayoutCheck(const int argc, const char *argv[]) const
        {
            return argumentStringMatch(argc, argv, "-pinLayout");
        }

        // Check the supplied mode
        const wiringPiModes::type wiringPiMode_;
        [[nodiscard]] wiringPiModes::type wiringPiModeCheck(const int argc, const char *argv[]) const
        {
            if (argc > 0)
            {
                for (std::size_t i = 0; i < static_cast<std::size_t>(argc); i++)
                {
                    if (strcmp(argv[i], "-mode") == 0)
                    {
                        // First check to see that i + 1 is not out of bounds
                        if (i + 1 < nArgs_)
                        {
                            if (strcmp(argv[i + 1], "pins") == 0)
                            {
                                return wiringPiModes::pins;
                            }
                            else if (strcmp(argv[i + 1], "gpio") == 0)
                            {
                                return wiringPiModes::gpio;
                            }
                            else if (strcmp(argv[i + 1], "gpioSys") == 0)
                            {
                                return wiringPiModes::gpioSys;
                            }
                            else if (strcmp(argv[i + 1], "phys") == 0)
                            {
                                return wiringPiModes::phys;
                            }
                            else if (strcmp(argv[i + 1], "piFace") == 0)
                            {
                                return wiringPiModes::piFace;
                            }
                            else if (strcmp(argv[i + 1], "all") == 0)
                            {
                                return wiringPiModes::all;
                            }
                            else
                            {
                                std::cout << "Invalid mode " << argv[i + 1] << " supplied" << std::endl;
                                std::cout << "Valid modes are: pins, gpio, gpioSys, phys, piFace, all" << std::endl;
                                return wiringPiModes::uninitialised;
                            }
                        }
                        // Otherwise it is out of bounds: default to Pins
                        else
                        {
                            return wiringPiModes::pins;
                        }
                    }
                }
            }
            return wiringPiModes::uninitialised;
        }
    };
}

#endif