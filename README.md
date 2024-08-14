The aim of this library is to replicate the functionality of the original wiringPi source code in a header-only library.

The use of linked lists for wiringPiNodeStruct is being replaced with std::array.

The wiringPi class is templated to allow execution in various modes; pins, gpio, phys, etc.

Static assertion checking is done to check for invalid modes of execution for certain functions/methods that require execution in a particular mode.

This is still a very early work in progress and doesn't contain anything close to the full functionality of the original library.
