The aim of this library is to replicate the functionality of the original wiringPi source code in a header-only library.

The readSystemInfo folder compiles and runs a program which generates a header file containing the hardware information about the Raspberry Pi on which it is running. This header file is by default installed under `/usr/local/include`. Entering the readSystemInfo folder and running `make install` will generate the header file and install it. This provides easy access to information about the current hardware for use at compile time. This program effectively performs the same task as the `piBoardId` and `piGpioLayout` functions called from within `wiringPiSetup`, except the result is then made available at compile time. The models, versions, manufacturers, memory specs, board layout and warranty are now enumerated types, as is the wiringPiMode and the pinMode.

The `wiringPi` class is templated with 3 parameters: the model of Raspberry Pi, the GPIO layout and the mode of execution (pins, GPIO or phys). The use of a template class allows for the compiler to perform certain compile-time safety checks. In addition, static assertion checking is done to check for invalid modes of execution for certain functions/methods that require execution in a particular mode.

This is still a very early work in progress and doesn't contain anything close to the full functionality of the original library.

** TO DO **
1) Implement softTone ** DONE **
2) Implement wiringSerial ** DONE EXCEPT FOR PRINTF **
3) Implement wiringShift ** DONE **
4) Implement drcNet
5) Implement pseudoPins
6) Implement wpiExtensions
