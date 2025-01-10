// ======================================================================== //
//                                                                          //
// SPI.H                                                                    //
//                                                                          //
// ======================================================================== //
// This class contains a registry of all registered SPI devices             //
// Includes all devices under the SPI/ folder                               //
// ======================================================================== //

#ifndef __WIRING_PI_SPI_H
#define __WIRING_PI_SPI_H

#include "../wiringPi.H"

namespace WiringPi
{
    namespace spi
    {
        // Modes of execution
        namespace mode
        {
            // This is an enumerated type because the memory can only be one of a few select values
            typedef enum Enum : std::size_t
            {
                MODE_0 = 0,
                MODE_1 = 1,
                MODE_2 = 2,
                MODE_3 = 3
            } type;

            template <const type mode_>
            using constant = const std::integral_constant<type, mode_>;
        }

        // SPI numbers
        namespace number
        {
            typedef enum Enum : std::size_t
            {
                n_0 = 0,
                n_1 = 1,
                n_2 = 2,
                n_3 = 3,
                n_4 = 4,
                n_5 = 5,
                n_6 = 6
            } type;

            template <const type mode_>
            using constant = const std::integral_constant<type, mode_>;
        }

        // SPI channels
        namespace channel
        {
            typedef enum Enum : std::size_t
            {
                c_0 = 0,
                c_1 = 1,
                c_2 = 2
            } type;

            template <const type mode_>
            using constant = const std::integral_constant<type, mode_>;
        }

        template <typename T>
        [[nodiscard]] inline consteval T BPW()
        {
            return 8;
        }
        template <typename T>
        [[nodiscard]] inline consteval T delay()
        {
            return 0;
        }

        template <typename T>
        [[nodiscard]] inline consteval T maxNumbers()
        {
            return 7;
        }
        template <typename T>
        [[nodiscard]] inline consteval T maxChannels()
        {
            return 3;
        }

        template <const uint32_t speed_>
        using speed_constant = const std::integral_constant<uint32_t, speed_>;

        template <const spi::number::type number_, const spi::channel::type channel_>
        inline void channelAssert(const spi::number::constant<number_> number, const spi::channel::constant<channel_> channel)
        {
            static_assert((channel == spi::channel::c_0 || channel == spi::channel::c_1 || channel == spi::channel::c_2), "wiringPiSPI: Invalid SPI channel: valid range 0-2");

            static_assert((number == spi::number::n_0 || number == spi::number::n_1 || number == spi::number::n_2 || number == spi::number::n_3 || number == spi::number::n_4 || number == spi::number::n_5 || number == spi::number::n_6), "wiringPiSPI: Invalid SPI channel: valid range 0-2");
        }

        template <const spi::mode::type mode_>
        inline void modeAssert(const spi::mode::constant<mode_> mode)
        {
            static_assert((mode == spi::mode::MODE_0 || mode == spi::mode::MODE_1 || mode == spi::mode::MODE_2 || mode == spi::mode::MODE_3), "wiringPiSPI: Invalid mode: valid range 0-3");
        }
    }

    template <const Pi::model::type Model, const Pi::layout::type Layout, const wiringPiModes::type wiringPiMode>
    class SPI
    {
    public:
        // Default constructor: take a reference to the wiringPi object
        [[nodiscard]] inline SPI(wiringPi<Model, Layout, wiringPiMode> &RaspberryPi) : RaspberryPi_(RaspberryPi) {};

        ~SPI()
        {
            // Close all the files on exit
            closeAll();
        };

        // Open the SPI device and set it up in mode
        template <const spi::number::type number_, const spi::channel::type channel_, const uint32_t speed_, const spi::mode::type mode_>
        void setup(const spi::number::constant<number_> number, const spi::channel::constant<channel_> channel, const spi::speed_constant<speed_> speed, const spi::mode::constant<mode_> mode)
        {
            int fd = -1;

            // Check that the channel is valid
            spi::channelAssert(number, channel);

            // Check that the mode is valid
            spi::modeAssert(mode);

            const std::string spiDevString = "/dev/spidev" + std::to_string(number) + "." + std::to_string(channel);

            if ((fd = open(spiDevString.c_str(), O_RDWR)) < 0)
            {
                RaspberryPi_.failure(name_t("Unable to open SPI device " + spiDevString + ": "));
            }

            // Set SPI parameters.
            if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
            {
                RaspberryPi_.failure("SPI mode change failure: ");
            }

            constexpr const uint8_t spiBPW = spi::BPW<uint8_t>();
            // const int tempInt_1 = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW);
            if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0)
            {
                RaspberryPi_.failure("SPI BPW change failure: ");
            }

            // const int tempInt_2 = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
            if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
            {
                RaspberryPi_.failure("SPI speed change failure: ");
            }

            speeds_[number][channel] = speed;
            fds_[number][channel] = fd;
        }

        // Close the SPI device
        template <const spi::number::type number_, const spi::channel::type channel_>
        void spiClose(const spi::number::constant<number_> number, const spi::channel::constant<channel_> channel)
        {
            // Check that the channel is valid
            spi::channelAssert(number, channel);

            // Try to close the file
            if (fds_[number][channel] > 0)
            {
                if (close(fds_[number][channel]) != 0)
                {
                    RaspberryPi_.failure("Failed to close SPI device: ");
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
            spi::channelAssert(number, channel);

            // Return the file descriptor
            return fds_[number][channel];
        }

        template <const spi::number::type number_, const spi::channel::type channel_>
        [[nodiscard]] inline int readWrite(const spi::number::constant<number_> number, const spi::channel::constant<channel_> channel, const unsigned char *data, const uint32_t len) const
        {
            // Check that the channel is valid
            spi::channelAssert(number, channel);
            if (fds_[number][channel] == -1)
            {
                RaspberryPi_.failure("wiringPiSPI: Invalid SPI number/channel (need to call setup before read/write): ");
            }

            struct spi_ioc_transfer spi_
            {
                data, data, len, speeds_[number][channel], spi::delay<__u16>(), spi::BPW<__u8>(), 0, 0, 0, 0, 0
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
            spi::channelAssert(spi::number::constant<number_>(), spi::channel::constant<channel_>());

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
            spi::channelAssert(spi::number::constant<number_>(), spi::channel::constant<channel_>());

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
