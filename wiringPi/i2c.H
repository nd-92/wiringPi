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
    template <const int layout, const pin_t devID>
    class i2c
    {
    public:
        [[nodiscard]] inline i2c()
            : fd_(setupInterface(std::string(deviceName_)))
        {
            static_assert(layoutCheck(layout));

            assert((!(fd_ < 0)));
            std::cout << "fd = " << fd_ << std::endl;

            assert((!(ioctl(fd_, I2C::slave<unsigned long int>(), devID) < 0)));
            std::cout << "ioctl = " << (ioctl(fd_, I2C::slave<unsigned long int>(), devID) < 0) << std::endl;
        };

        // Simple read
        [[nodiscard]] inline I2C_t read() const
        {
            union i2c_smbus_data data;

            if (I2C_smbus_access(I2C::smbusRead<uint8_t>(), 0, I2C::smbusByte<uint32_t>(), &data))
            {
                return static_cast<I2C_t>(-1);
            }
            else
            {
                return static_cast<I2C_t>(data.byte & 0xFF);
            }
        }

        // Read an 8 bit value from a register
        [[nodiscard]] inline I2C_t read8(const uint8_t reg) const
        {
            union i2c_smbus_data data;

            if (I2C_smbus_access(I2C::smbusRead<uint8_t>(), reg, I2C::smbusByteData<uint32_t>(), &data))
            {
                return static_cast<I2C_t>(-1);
            }
            else
            {
                return static_cast<I2C_t>(data.byte & 0xFF);
            }
        }

        // Read a 16 bit value from a register
        [[nodiscard]] inline I2C_t read16(const uint8_t reg) const
        {
            union i2c_smbus_data data;

            if (I2C_smbus_access(I2C::smbusRead<uint8_t>(), reg, I2C::smbusWordData<uint32_t>(), &data))
            {
                return static_cast<I2C_t>(-1);
            }
            else
            {
                return static_cast<I2C_t>(data.word & 0xFFFF);
            }
        }

        // Read the block data
        [[nodiscard]] inline I2C_t readBlockData(const uint8_t reg, uint8_t *values, const uint8_t size) const
        {
            union i2c_smbus_data data;

            const uint8_t blockSize = std::min(size, I2C::smbusBlockMax<uint8_t>());

            data.block[0] = blockSize;
            const I2C_t result = static_cast<I2C_t>(I2C_smbus_access(I2C::smbusRead<uint8_t>(), reg, I2C::smbusI2CBlockData<uint32_t>(), &data));
            if (result == static_cast<I2C_t>(-1))
            {
                return result;
            }
            memcpy(values, &data.block[1], blockSize);
            return static_cast<I2C_t>(data.block[0]);
        }

        [[nodiscard]] inline I2C_t readRaw(uint8_t *values, const size_t size) const
        {
            return read(fd_, values, size);
        }

        [[nodiscard]] inline consteval std::string_view deviceName() const
        {
            return deviceName_;
        }

        // Write functions
        // Simple write
        [[nodiscard]] inline I2C_t write(const uint8_t data) const
        {
            return I2C_smbus_access(I2C::smbusWrite<uint8_t>(), data, I2C::smbusByte<uint32_t>(), NULL);
        }

        // Write an 8 bit value to a register
        [[nodiscard]] inline I2C_t write8(const uint8_t reg, const uint8_t value) const
        {
            union i2c_smbus_data data;

            data.byte = value;
            return I2C_smbus_access(I2C::smbusWrite<uint8_t>(), reg, I2C::smbusByteData<uint32_t>(), &data);
        }

        // Write a 18 bit value to a register
        [[nodiscard]] inline I2C_t write16(const uint8_t reg, const uint16_t value) const
        {
            union i2c_smbus_data data;

            data.word = value;
            return I2C_smbus_access(I2C::smbusWrite<uint8_t>(), reg, I2C::smbusWordData<uint32_t>(), &data);
        }

        // Write the block data
        [[nodiscard]] inline I2C_t writeBlock(const uint8_t reg, const uint8_t *values, const uint8_t size) const
        {
            union i2c_smbus_data data;

            const uint8_t blockSize = std::min(size, I2C::smbusBlockMax<uint8_t>());

            data.block[0] = blockSize;
            memcpy(&data.block[1], values, blockSize);
            return I2C_smbus_access(I2C::smbusWrite<uint8_t>(), reg, I2C::smbusBlockData<uint32_t>(), &data);
        }

        [[nodiscard]] inline I2C_t writeRaw(const uint8_t *values, const size_t size) const
        {
            return write(fd_, values, size);
        }

    private:
        const int fd_;
        [[nodiscard]] inline int setupInterface(const std::string device) const
        {
            return open(device.c_str(), O_RDWR);
        }

        inline int I2C_smbus_access(const uint8_t rw, const uint8_t command, const uint32_t size, union i2c_smbus_data *data) const
        {
            struct i2c_smbus_ioctl_data args
            {
                rw, command, size, data
            };
            return ioctl(fd_, I2C::smbus<unsigned long>(), &args);
        }

        static constexpr const std::string_view deviceName_ = I2C::deviceName<layout>();
    };
}

#endif
