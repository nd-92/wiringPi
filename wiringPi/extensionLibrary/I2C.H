// ======================================================================== //
//                                                                          //
// I2C.H                                                                    //
//                                                                          //
// ======================================================================== //
// Implements access to I2C                                                 //
// Includes all devices under the I2C/ folder                               //
// ======================================================================== //

#ifndef __WIRING_PI_I2C_H
#define __WIRING_PI_I2C_H

#include "../wiringPi.H"

namespace WiringPi
{
    typedef ssize_t I2C_t;
    template <const I2C_t I2C_>
    using I2C_constant = const std::integral_constant<I2C_t, I2C_>;

    // I2C defines
    namespace i2c
    {
        template <typename T>
        [[nodiscard]] inline consteval T slave()
        {
            return 0x0703;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbus()
        {
            return 0x0720;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusRead()
        {
            return 1;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusWrite()
        {
            return 0;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusQuick()
        {
            return 0;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusByte()
        {
            return 1;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusByteData()
        {
            return 2;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusWordData()
        {
            return 3;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusProcCall()
        {
            return 4;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusBlockData()
        {
            return 5;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusI2CBlockBroken()
        {
            return 6;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusBlockProcCall()
        {
            return 7;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusI2CBlockData()
        {
            return 8;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusBlockMax()
        {
            return 32;
        }
        template <typename T>
        [[nodiscard]] inline consteval T smbusI2CBlockMax()
        {
            return 32;
        }
        template <const Pi::layout::type layout>
        [[nodiscard]] consteval const name_t deviceName()
        {
            if constexpr (layout == Pi::layout::DEFAULT)
            {
                return "/dev/i2c-0";
            }
            else
            {
                return "/dev/i2c-1";
            }
        }
    }

    union i2c_smbus_data
    {
        uint8_t byte;
        uint16_t word;
        uint8_t block[i2c::smbusBlockMax<std::size_t>() + 2];
    };

    struct i2c_smbus_ioctl_data
    {
        const uint8_t read_write;
        const uint8_t command;
        const uint32_t size;
        union i2c_smbus_data *data;
    };

    template <const pin_t devID, const Pi::model::type Model, const Pi::layout::type Layout, const wiringPiModes::type wiringPiMode>
    class I2C
    {
    public:
        // Default constructor: take a reference to the wiringPi object
        [[nodiscard]] inline I2C(wiringPi<Model, Layout, wiringPiMode> &RaspberryPi) : RaspberryPi_(RaspberryPi), fd_(setupInterface()) {};

        // Simple read
        [[nodiscard]] uint8_t read() const
        {
            union i2c_smbus_data data;

            // Perform the read and check for failure
            if (I2C_smbus_access(i2c::smbusRead<uint8_t>(), 0, i2c::smbusByte<uint32_t>(), &data) == -1)
            {
                RaspberryPi_.failure("Failed to read data from I2C smbus: ");
            }

            // Return the data
            return (data.byte & std::numeric_limits<uint8_t>::max());
        }

        // Read an 8 bit value from a register
        [[nodiscard]] uint8_t read8(const uint8_t reg) const
        {
            union i2c_smbus_data data;

            // Perform the read and check for failure
            if (I2C_smbus_access(i2c::smbusRead<uint8_t>(), reg, i2c::smbusByteData<uint32_t>(), &data) == -1)
            {
                RaspberryPi_.failure("Failed to read uint8_t data from I2C smbus: ");
            }

            // Return the data
            return (data.byte & std::numeric_limits<uint8_t>::max());
        }

        // Read a 16 bit value from a register
        [[nodiscard]] uint16_t read16(const uint8_t reg) const
        {
            union i2c_smbus_data data;

            // Perform the read and check for failure
            if (I2C_smbus_access(i2c::smbusRead<uint8_t>(), reg, i2c::smbusWordData<uint32_t>(), &data) == -1)
            {
                RaspberryPi_.failure("Failed to read uint16_t data from I2C smbus: ");
            }

            // Return the data
            return (data.word & std::numeric_limits<uint16_t>::max());
        }

        // Read the block data
        [[nodiscard]] I2C_t readBlockData(const uint8_t reg, uint8_t *values, const uint8_t size) const
        {
            union i2c_smbus_data data;

            const uint8_t blockSize = std::min(size, i2c::smbusBlockMax<uint8_t>());

            data.block[0] = blockSize;

            // Perform the read and check for failure
            if (I2C_smbus_access(i2c::smbusRead<uint8_t>(), reg, i2c::smbusI2CBlockData<uint32_t>(), &data) == -1)
            {
                RaspberryPi_.failure("Failed to read block data from I2C smbus: ");
            }
            memcpy(values, &data.block[1], blockSize);

            // Return the data
            return static_cast<I2C_t>(data.block[0]);
        }

        [[nodiscard]] inline consteval name_t deviceName() const
        {
            return "I2C";
        }

        // Simple write
        void write(const uint8_t data) const
        {
            if (I2C_smbus_access(i2c::smbusWrite<uint8_t>(), data, i2c::smbusByte<uint32_t>(), NULL))
            {
                RaspberryPi_.failure("Failed to write data to I2C smbus: ");
            }
        }

        // Write an 8 bit value to a register
        void write8(const uint8_t reg, const uint8_t value) const
        {
            union i2c_smbus_data data;
            data.byte = value;

            // Perform the write and check for failure
            if (I2C_smbus_access(i2c::smbusWrite<uint8_t>(), reg, i2c::smbusByteData<uint32_t>(), &data) == -1)
            {
                RaspberryPi_.failure("Failed to write uint8_t data to I2C smbus: ");
            }
        }

        // Write a 16 bit value to a register
        void write16(const uint8_t reg, const uint16_t value) const
        {
            union i2c_smbus_data data;
            data.word = value;

            // Perform the write and check for failure
            if (I2C_smbus_access(i2c::smbusWrite<uint8_t>(), reg, i2c::smbusWordData<uint32_t>(), &data) == -1)
            {
                RaspberryPi_.failure("Failed to write uint16_t data to I2C smbus: ");
            }
        }

        // Write the block data
        void writeBlock(const uint8_t reg, const uint8_t *values, const uint8_t size) const
        {
            union i2c_smbus_data data;

            const uint8_t blockSize = std::min(size, i2c::smbusBlockMax<uint8_t>());
            data.block[0] = blockSize;
            memcpy(&data.block[1], values, blockSize);

            // Perform the write and check for failure
            if (I2C_smbus_access(i2c::smbusWrite<uint8_t>(), reg, i2c::smbusBlockData<uint32_t>(), &data) == -1)
            {
                RaspberryPi_.failure("Failed to write block data to I2C smbus: ");
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

            if ((fd = open(Pi::layout::deviceName<const char *, Layout>(), O_RDWR)) < 0)
            {
                RaspberryPi_.failure("Failed to setup I2C interface: ");
            }

            if (ioctl(fd, i2c::slave<unsigned long>(), devID) < 0)
            {
                RaspberryPi_.failure(name_t("Unable to select I2C device " + Pi::layout::deviceName<std::string, Layout>() + ": "));
            }

            return fd;
        }

        [[nodiscard]] inline int I2C_smbus_access(const uint8_t rw, const uint8_t command, const uint32_t size, union i2c_smbus_data *data) const
        {
            struct i2c_smbus_ioctl_data args
            {
                rw, command, size, data
            };
            return ioctl(fd_, i2c::smbus<unsigned long>(), &args);
        }
    };
}

#include "I2C/ADS1115.H"
#include "I2C/BMP180.H"

#endif
