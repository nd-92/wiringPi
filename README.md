The aim of this library is to replicate the functionality of the original wiringPi source code in a header-only library.

The use of linked lists for wiringPiNodeStruct is being replaced with std::array.

The wiringPi class is templated to allow execution in various modes; pins, gpio, phys, etc.

Static assertion checking is done to check for invalid modes of execution for certain functions/methods that require execution in a particular mode. The same is being done for valid pins, hence the liberal use of constexpr.

Compatibility for the Raspberry Pi 5 has not been added yet. There is also no support for offboard pins yet.

This is still a very early work in progress and doesn't contain anything close to the full functionality of the original library.

** TO DO **
1) Implement softTone ** DONE **
2) Implement wiringSerial
3) Implement wiringShift
4) Implement drcNet
5) Implement pseudoPins
6) Implement wpiExtensions
