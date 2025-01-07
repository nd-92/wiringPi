// ======================================================================== //
//                                                                          //
// SPI.H                                                                    //
//                                                                          //
// ======================================================================== //
// This class contains a registry of all registered SPI devices             //
// ======================================================================== //

#ifndef __WIRING_PI_SPI_H
#define __WIRING_PI_SPI_H

#include "wiringPi.H"

namespace WiringPi
{
    template <const Pi::model::type Model, const Pi::layout::type Layout, const wiringPiModes::type wiringPiMode>
    class SPI
    {
    public:
        // Default constructor: take a reference to the wiringPi object
        [[nodiscard]] inline SPI(wiringPi<Model, Layout, wiringPiMode> &RaspberryPi) : RaspberryPi_(RaspberryPi) {};

        // Close all the files on exit
        ~SPI()
        {
            closeAll();
        };

        // Open the SPI device and set it up in mode
        template <const spi::number::type number_, const spi::channel::type channel_, const uint32_t speed_, const spi::mode::type mode_>
        void setup(const spi::number::constant<number_> number, const spi::channel::constant<channel_> channel, const spi::speed_constant<speed_> speed, const spi::mode::constant<mode_> mode)
        {
            int fd = -1;

            // Check that the channel is valid
            spiChannelAssert<number_, channel_>();

            spiModeAssert<mode_>();

            const std::string spiDevString = "/dev/spidev" + std::to_string(number) + "." + std::to_string(channel);
            // std::cout << spiDevString << std::endl;

            if ((fd = open(spiDevString.c_str(), O_RDWR)) < 0)
            {
                RaspberryPi_.failure(name_t("Unable to open SPI device " + spiDevString + ": " + strerror(errno)));
            }

            // Set SPI parameters.
            if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
            {
                RaspberryPi_.failure(name_t("SPI mode change failure: " + std::string(strerror(errno))));
            }

            constexpr const uint8_t spiBPW = spi::BPW<uint8_t>();
            // const int tempInt_1 = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW);
            if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0)
            {
                RaspberryPi_.failure(name_t("SPI BPW change failure: " + std::string(strerror(errno))));
            }

            // const int tempInt_2 = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
            if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
            {
                RaspberryPi_.failure(name_t("SPI speed change failure: " + std::string(strerror(errno))));
            }

            speeds_[number][channel] = speed;
            fds_[number][channel] = fd;
        }

        // Close the SPI device
        template <const spi::number::type number_, const spi::channel::type channel_>
        void spiClose(const spi::number::constant<number_> number, const spi::channel::constant<channel_> channel)
        {
            // Check that the channel is valid
            spiChannelAssert<number_, channel_>();

            // Try to close the file
            if (fds_[number][channel] > 0)
            {
                if (close(fds_[number][channel]) != 0)
                {
                    RaspberryPi_.failure();
                }
            }

            // Reset the speeds and file descriptors
            speeds_[number][channel] = 0;
            fds_[number][channel] = -1;
        }

        // Return the file descriptor for the given channel
        template <const spi::number::type number_, const spi::channel::type channel_>
        [[nodiscard]] inline int fd(const spi::number::constant<number_> number, const spi::channel::constant<channel_> channel) const
        {
            // Check that the channel is valid
            spiChannelAssert<number_, channel_>();

            // Return the file descriptor
            return fds_[number][channel];
        }

        template <const spi::number::type number_, const spi::channel::type channel_>
        [[nodiscard]] inline int readWrite(const spi::number::constant<number_> number, const spi::channel::constant<channel_> channel, const unsigned char *data, const uint32_t len) const
        {
            // Check that the channel is valid
            spiChannelAssert<number_, channel_>();
            if (fds_[number][channel] == -1)
            {
                RaspberryPi_.failure("wiringPiSPI: Invalid SPI number/channel (need to call setup before read/write)");
            }

            struct spi_ioc_transfer spi_
            {
                data,                         // tx_buf
                    data,                     // rx_buf
                    len,                      // len
                    speeds_[number][channel], // speed_hz
                    spi::delay<__u16>(),      // delay_usecs
                    spi::BPW<__u8>(),         // bits_per_word
                    0,                        // cs_change
                    0,                        // tx_nbits
                    0,                        // rx_nbits
                    0,                        // word_delay_usecs
                    0                         // pad
            };

            return ioctl(fds_[number][channel], SPI_IOC_MESSAGE(1), &spi_);
        }

    private:
        wiringPi<Model, Layout, wiringPiMode> &RaspberryPi_;

        uint32_t speeds_[7][3] = {
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0},
            {0, 0, 0}};

        int fds_[7][3] = {
            {-1, -1, -1},
            {-1, -1, -1},
            {-1, -1, -1},
            {-1, -1, -1},
            {-1, -1, -1},
            {-1, -1, -1},
            {-1, -1, -1}};

        template <const spi::number::type number_ = spi::number::n_0, const spi::channel::type channel_ = spi::channel::c_0>
        void closeAll()
        {
            // Check that the channel is valid
            spiChannelAssert<number_, channel_>();

            // Close the channels in this number
            close_channel<number_, channel_>();

            // Advance to the next number
            if constexpr (number_ < spi::number::n_6 - 1)
            {
                closeAll<spi::number::type(number_ + 1), channel_>();
            }
        }

        template <const spi::number::type number_, const spi::channel::type channel_>
        void close_channel()
        {
            // Check that the channel is valid
            spiChannelAssert<number_, channel_>();

            // Close this combination of number and channel
            spiClose(spi::number::constant<number_>(), spi::channel::constant<channel_>());

            // Advance to the next channel
            if constexpr (channel_ < spi::channel::c_2 - 1)
            {
                close_channel<number_, spi::channel::type(channel_ + 1)>();
            }
        }
    };
}

#endif
