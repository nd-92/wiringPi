// ======================================================================== //
//                                                                          //
// i2c.H                                                                    //
//                                                                          //
// ======================================================================== //
// Implements access to I2C                                                 //
// ======================================================================== //

#ifndef __WIRING_PI_i2c_H
#define __WIRING_PI_i2c_H

#include "wiringPi.H"

namespace WiringPi
{
    template <const pin_t devID, const Pi::models::type Model, const Pi::layout::type Layout, const wiringPiModes::type wiringPiMode>
    class i2c
    {
    public:
        // Default constructor: take a reference to the wiringPi object
        [[nodiscard]] inline i2c(wiringPi<Model, Layout, wiringPiMode> &RaspberryPi) : RaspberryPi_(RaspberryPi), fd_(setupInterface()) {};

        // Simple read
        [[nodiscard]] uint8_t read() const
        {
            union i2c_smbus_data data;

            // Perform the read and check for failure
            if (I2C_smbus_access(I2C::smbusRead<uint8_t>(), 0, I2C::smbusByte<uint32_t>(), &data) == -1)
            {
                RaspberryPi_.failure("Failed to read data from i2c smbus");
            }

            // Return the data
            return (data.byte & std::numeric_limits<uint8_t>::max());
        }

        // Read an 8 bit value from a register
        [[nodiscard]] uint8_t read8(const uint8_t reg) const
        {
            union i2c_smbus_data data;

            // Perform the read and check for failure
            if (I2C_smbus_access(I2C::smbusRead<uint8_t>(), reg, I2C::smbusByteData<uint32_t>(), &data) == -1)
            {
                RaspberryPi_.failure("Failed to read uint8_t data from i2c smbus");
            }

            // Return the data
            return (data.byte & std::numeric_limits<uint8_t>::max());
        }

        // Read a 16 bit value from a register
        [[nodiscard]] uint16_t read16(const uint8_t reg) const
        {
            union i2c_smbus_data data;

            // Perform the read and check for failure
            if (I2C_smbus_access(I2C::smbusRead<uint8_t>(), reg, I2C::smbusWordData<uint32_t>(), &data) == -1)
            {
                RaspberryPi_.failure("Failed to read uint16_t data from i2c smbus");
            }

            // Return the data
            return (data.word & std::numeric_limits<uint16_t>::max());
        }

        // Read the block data
        [[nodiscard]] I2C_t readBlockData(const uint8_t reg, uint8_t *values, const uint8_t size) const
        {
            union i2c_smbus_data data;

            const uint8_t blockSize = std::min(size, I2C::smbusBlockMax<uint8_t>());

            data.block[0] = blockSize;

            // Perform the read and check for failure
            if (I2C_smbus_access(I2C::smbusRead<uint8_t>(), reg, I2C::smbusI2CBlockData<uint32_t>(), &data) == -1)
            {
                RaspberryPi_.failure("Failed to read block data from i2c smbus");
            }
            memcpy(values, &data.block[1], blockSize);

            // Return the data
            return static_cast<I2C_t>(data.block[0]);
        }

        [[nodiscard]] inline consteval name_t deviceName() const
        {
            return "i2c";
        }

        // Simple write
        void write(const uint8_t data) const
        {
            if (I2C_smbus_access(I2C::smbusWrite<uint8_t>(), data, I2C::smbusByte<uint32_t>(), NULL))
            {
                RaspberryPi_.failure("Failed to write data to i2c smbus");
            }
        }

        // Write an 8 bit value to a register
        void write8(const uint8_t reg, const uint8_t value) const
        {
            union i2c_smbus_data data;
            data.byte = value;

            // Perform the write and check for failure
            if (I2C_smbus_access(I2C::smbusWrite<uint8_t>(), reg, I2C::smbusByteData<uint32_t>(), &data) == -1)
            {
                RaspberryPi_.failure("Failed to write uint8_t data to i2c smbus");
            }
        }

        // Write a 16 bit value to a register
        void write16(const uint8_t reg, const uint16_t value) const
        {
            union i2c_smbus_data data;
            data.word = value;

            // Perform the write and check for failure
            if (I2C_smbus_access(I2C::smbusWrite<uint8_t>(), reg, I2C::smbusWordData<uint32_t>(), &data) == -1)
            {
                RaspberryPi_.failure("Failed to write uint16_t data to i2c smbus");
            }
        }

        // Write the block data
        void writeBlock(const uint8_t reg, const uint8_t *values, const uint8_t size) const
        {
            union i2c_smbus_data data;

            const uint8_t blockSize = std::min(size, I2C::smbusBlockMax<uint8_t>());
            data.block[0] = blockSize;
            memcpy(&data.block[1], values, blockSize);

            // Perform the write and check for failure
            if (I2C_smbus_access(I2C::smbusWrite<uint8_t>(), reg, I2C::smbusBlockData<uint32_t>(), &data) == -1)
            {
                RaspberryPi_.failure("Failed to write block data to i2c smbus");
            }
        }

        [[nodiscard]] inline int fd() const
        {
            return fd_;
        }

    private:
        wiringPi<Model, Layout, wiringPiMode> &RaspberryPi_;
        const int fd_;

        [[nodiscard]] inline int setupInterface() const
        {
            int fd = -1;
            const std::string deviceName_ = std::string(deviceDirName(RaspberryPi_.piHardwareInfo().gpioLayout()));

            if ((fd = open(deviceName_.c_str(), O_RDWR)) < 0)
            {
                RaspberryPi_.failure("Failed to setup i2c interface");
            }

            if (ioctl(fd, I2C::slave<unsigned long>(), devID) < 0)
            {
                RaspberryPi_.failure(("Unable to select I2C device: " + deviceName_));
            }

            return fd;
        }

        [[nodiscard]] inline int I2C_smbus_access(const uint8_t rw, const uint8_t command, const uint32_t size, union i2c_smbus_data *data) const
        {
            struct i2c_smbus_ioctl_data args
            {
                rw, command, size, data
            };
            return ioctl(fd_, I2C::smbus<unsigned long>(), &args);
        }

        // This should really be constexpr
        // Need to figure out how to supply layout from RaspberryPi_ at compile time despite it not being constexpr!
        [[nodiscard]] const name_t deviceDirName(const Pi::layout::type layout) const
        {
            if (layout == Pi::layout::PI1_REV1)
            {
                return "/dev/i2c-0";
            }
            else
            {
                return "/dev/i2c-1";
            }
        }
    };
}

#endif
