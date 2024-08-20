#include "wiringPi.H"

using namespace WiringPi;

int main(void)
{
    const i2c<2, 13> I2C;

    std::cout << "I2C device: " << I2C.deviceName() << std::endl;

    uint8_t value = 0xFF;

    // std::cout << "I2C.read() returning " << I2C.i2cRead() << ". False return is equal to " << static_cast<gpio_t>(-1) << std::endl;

    // std::cout << "I2C.read8() returning " << I2C.i2cRead8(0) << ". False return is equal to " << static_cast<gpio_t>(-1) << std::endl;

    // std::cout << "I2C.read16() returning " << I2C.i2cRead16(0) << ". False return is equal to " << static_cast<gpio_t>(-1) << std::endl;

    // std::cout << "I2C.readBlockData() returning " << I2C.i2cReadBlockData(0, &value, 1) << ". False return is equal to " << static_cast<gpio_t>(-1) << std::endl;

    // std::cout << "I2C.readRaw() returning " << I2C.i2cReadRaw(&value, 1) << ". False return is equal to " << static_cast<gpio_t>(-1) << std::endl;

    std::cout << "I2C.i2cRead() returning " << I2C.i2cRead() << ". False return is equal to " << static_cast<gpio_t>(-1) << std::endl;

    std::cout << "I2C.i2cRead8() returning " << I2C.i2cRead8(0) << ". False return is equal to " << static_cast<gpio_t>(-1) << std::endl;

    std::cout << "I2C.i2cRead16() returning " << I2C.i2cRead16(0) << ". False return is equal to " << static_cast<gpio_t>(-1) << std::endl;

    std::cout << "I2C.i2cReadBlockData() returning " << I2C.i2cReadBlockData(0, &value, 1) << ". False return is equal to " << static_cast<gpio_t>(-1) << std::endl;

    std::cout << "I2C.i2cReadRaw() returning " << I2C.i2cReadRaw(&value, 1) << ". False return is equal to " << static_cast<gpio_t>(-1) << std::endl;

    // std::cout << "I2C.read8() returning " << I2C.read8(0) << ". False return is equal to " << static_cast<gpio_t>(-1) << std::endl;

    return 0;
}