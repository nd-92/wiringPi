// ======================================================================== //
//                                                                          //
// wiringPiSignals.H                                                        //
//                                                                          //
// ======================================================================== //
// Handle error signals properly                                            //
// ======================================================================== //

#ifndef __WIRING_PI_wiringPiSignals_H
#define __WIRING_PI_wiringPiSignals_H

// #include "wiringPiIncludes.H"
// #include "wiringPiDefines.H"
#include "wiringPi.H"

namespace WiringPi
{
    static constinit std::atomic<bool> quit(false);

    void sigHandler(const int signal)
    {
        if (quit == false)
        {
            std::cout << "Exiting on signal " << signal << std::endl;
            std::cout << "Performing cleanup" << std::endl;
        }
        quit.store(true);
    }

    void safe_exit(const int signal, const name_t &errorMessage = "Undefined exception")
    {
        if (quit == false)
        {
            std::cout << "wiringPi exiting: " << errorMessage << std::endl;
            sigHandler(signal);
        }
    }

    void fatalAssert(const bool result, const name_t &errorMessage = "Undefined exception")
    {
        assert((result) && std::string(errorMessage).c_str());
    }

    // template <const int wiringPiMode>
    // void failure(const wiringPi<wiringPiMode> &RaspberryPi, const name_t errorMessage = "Undefined exception")
    // {
    //     std::cerr << errorMessage << std::endl;
    //     RaspberryPi.resetPins();
    //     std::exit(EXIT_FAILURE);
    // }

    // void failure(const int fatal, const char *message, ...)
    // {
    //     va_list argp;
    //     char buffer[1024];

    //     va_start(argp, message);
    //     vsnprintf(buffer, 1023, message, argp);
    //     va_end(argp);

    //     fprintf(stderr, "%s", buffer);
    //     std::exit(EXIT_FAILURE);
    // }
}

#endif
