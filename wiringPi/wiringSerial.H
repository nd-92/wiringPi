// ======================================================================== //
//                                                                          //
// wiringSerial.H                                                           //
//                                                                          //
// ======================================================================== //
// Handle a serial port                                                     //
// ======================================================================== //

#ifndef __WIRING_PI_wiringSerial_H
#define __WIRING_PI_wiringSerial_H

#include "wiringPiIncludes.H"
#include "wiringPiDefines.H"

namespace WiringPi
{
    class wiringSerial
    {
    public:
        // Open and initialise the serial port
        [[nodiscard]] wiringSerial(const name_t &deviceName, const speed_t baud) : deviceName_(deviceName), baud_(baud), fd_(openPort()) {};

        // Release the serial port upon destruction
        ~wiringSerial()
        {
            close(fd_);
        };

        // Returns the device name
        [[nodiscard]] inline name_t deviceName() const
        {
            return deviceName_;
        }

        // Returns the baud rate
        [[nodiscard]] inline speed_t baud() const
        {
            return baud_;
        }

        // Returns the file handle
        [[nodiscard]] inline int fd() const
        {
            return fd_;
        }

        // Flush the serial buffers
        inline void flush() const
        {
            tcflush(fd_, TCIOFLUSH);
        }

        // Send a single character to the serial port
        inline void putChar(const unsigned char c) const
        {
            assert((write(fd_, &c, 1) == 1) && "Error writing to file descriptor");
        }

        // Send a string to the serial port
        inline void putString(const name_t &s) const
        {
            assert((write(fd_, std::string(s).c_str(), s.length()) == static_cast<ssize_t>(s.length())) && "Error writing to file descriptor");
        }

        // Return the number of bytes of data avalable to be read in the serial port
        [[nodiscard]] inline int available() const
        {
            int result = 0;

            if (ioctl(fd_, FIONREAD, &result) == -1)
            {
                return -1;
            }

            return result;
        }

        // inline void printf(const char *message, ...) const
        // {
        //     va_list argp;
        //     char buffer[1024];

        //     va_start(argp, message);
        //     vsnprintf(buffer, 1023, message, argp);
        //     va_end(argp);

        //     putString( buffer);
        // }

        // Get a single character from the serial device
        // [[nodiscard]] inline uint8_t getChar() const
        [[nodiscard]] inline int getChar() const
        {
            uint8_t x = 0;

            // if (read(fd_, &x, 1) != 1)
            // {
            //     return static_cast<uint8_t>(-1);
            // }
            // return x;

            if (read(fd_, &x, 1) != 1)
            {
                return -1;
            }
            return (static_cast<int>(x)) & 0xFF;
        }

    private:
        const name_t deviceName_;
        const speed_t baud_;
        const int fd_;

        [[nodiscard]] int openPort() const
        {
            struct termios options;
            speed_t myBaud = 0;
            int status = -1;
            int _fd = -1;

            switch (baud_)
            {
            case 50:
                myBaud = B50;
                break;
            case 75:
                myBaud = B75;
                break;
            case 110:
                myBaud = B110;
                break;
            case 134:
                myBaud = B134;
                break;
            case 150:
                myBaud = B150;
                break;
            case 200:
                myBaud = B200;
                break;
            case 300:
                myBaud = B300;
                break;
            case 600:
                myBaud = B600;
                break;
            case 1200:
                myBaud = B1200;
                break;
            case 1800:
                myBaud = B1800;
                break;
            case 2400:
                myBaud = B2400;
                break;
            case 4800:
                myBaud = B4800;
                break;
            case 9600:
                myBaud = B9600;
                break;
            case 19200:
                myBaud = B19200;
                break;
            case 38400:
                myBaud = B38400;
                break;
            case 57600:
                myBaud = B57600;
                break;
            case 115200:
                myBaud = B115200;
                break;
            case 230400:
                myBaud = B230400;
                break;
            case 460800:
                myBaud = B460800;
                break;
            case 500000:
                myBaud = B500000;
                break;
            case 576000:
                myBaud = B576000;
                break;
            case 921600:
                myBaud = B921600;
                break;
            case 1000000:
                myBaud = B1000000;
                break;
            case 1152000:
                myBaud = B1152000;
                break;
            case 1500000:
                myBaud = B1500000;
                break;
            case 2000000:
                myBaud = B2000000;
                break;
            case 2500000:
                myBaud = B2500000;
                break;
            case 3000000:
                myBaud = B3000000;
                break;
            case 3500000:
                myBaud = B3500000;
                break;
            case 4000000:
                myBaud = B4000000;
                break;

            default:
                return -2;
            }

            if ((_fd = open(std::string(deviceName_).c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1)
            {
                return -1;
            }

            fcntl(_fd, F_SETFL, O_RDWR);

            // Get and modify current options:

            tcgetattr(_fd, &options);

            cfmakeraw(&options);
            cfsetispeed(&options, myBaud);
            cfsetospeed(&options, myBaud);

            options.c_cflag |= static_cast<tcflag_t>(CLOCAL | CREAD);
            options.c_cflag &= static_cast<tcflag_t>(~PARENB);
            options.c_cflag &= static_cast<tcflag_t>(~CSTOPB);
            options.c_cflag &= static_cast<tcflag_t>(~CSIZE);
            options.c_cflag |= static_cast<tcflag_t>(CS8);
            options.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO | ECHOE | ISIG));
            options.c_oflag &= static_cast<tcflag_t>(~OPOST);

            options.c_cc[VMIN] = 0;
            options.c_cc[VTIME] = 100; // Ten seconds (100 deciseconds)

            tcsetattr(_fd, TCSANOW, &options);

            ioctl(_fd, TIOCMGET, &status);

            status |= TIOCM_DTR;
            status |= TIOCM_RTS;

            ioctl(_fd, TIOCMSET, &status);

            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            return _fd;
        }
    };
}

#endif
