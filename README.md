The aim of this library is to replicate the functionality of the original wiringPi source code in a template library.

The use of linked lists for wiringPiNodeStruct is being replaced with std::array.

The wiringPi class will be templated to allow execution in various modes; pins, gpio, phys, etc.

This is still a very early work in progress and doesn't contain anything close to the full functionality of the original library.
