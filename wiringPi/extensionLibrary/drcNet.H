// ======================================================================== //
//                                                                          //
// drcNet.H                                                                 //
//                                                                          //
// ======================================================================== //
// Extend wiringPi with the DRC Network protocol (e.g. to another Pi)       //
// ======================================================================== //

#ifndef __WIRING_PI_drcNet_H
#define __WIRING_PI_drcNet_H

#include "../wiringPi.H"

namespace WiringPi
{
    namespace drc
    {
        template <typename T>
        [[nodiscard]] inline consteval T defaultServerPort()
        {
            return 6124;
        }

        // DRC commands
        namespace command
        {
            typedef enum Enum : uint32_t
            {
                PIN_MODE = 1,
                PULL_UP_DN = 2,
                DIGITAL_WRITE = 3,
                DIGITAL_WRITE8 = 4,
                ANALOG_WRITE = 5,
                PWM_WRITE = 6,
                DIGITAL_READ = 7,
                DIGITAL_READ8 = 8,
                ANALOG_READ = 9
            } type;

            template <const type command_>
            using constant = const std::integral_constant<type, command_>;
        }
    }

    // struct addrinfo
    // {
    //     int ai_flags;
    //     int ai_family;
    //     int ai_socktype;
    //     int ai_protocol;
    //     socklen_t ai_addrlen;
    //     struct sockaddr *ai_addr;
    //     char *ai_canonname;
    //     struct addrinfo *ai_next;
    // };

    struct drcNetComStruct
    {
        uint32_t pin;
        uint32_t cmd;
        uint32_t data;
    };

    static int remoteReadline(const int fd, char *buf, const int max)
    {
        int len = 0;
        char c;

        for (;;)
        {
            if (read(fd, &c, 1) < 1)
            {
                return -1;
            }

            if (c == '\n')
            {
                return len;
            }

            *buf++ = c;
            if (++len == max)
            {
                return len;
            }
        }
    }

    static char *getChallenge(const int fd)
    {
        static char buf[512];
        int num;

        for (;;)
        {
            if ((num = remoteReadline(fd, buf, 511)) < 0)
            {
                return NULL;
            }
            buf[num] = 0;

            if (strncmp(buf, "Challenge ", 10) == 0)
            {
                return &buf[10];
            }
        }
    }

    static int authenticate(const int fd, const char *pass)
    {
        char *challenge;
        char *encrypted;
        char salted[1024];

        if ((challenge = getChallenge(fd)) == NULL)
        {
            return -1;
        }

        sprintf(salted, "$6$%s$", challenge);
        encrypted = crypt(pass, salted);

        // This is an assertion, or sanity check on my part...
        //	The '20' comes from the $6$ then the 16 characters of the salt,
        //	then the terminating $.

        if (strncmp(encrypted, salted, 20) != 0)
        {
            errno = EBADE;
            return -1;
        }

        // 86 characters is the length of the SHA-256 hash

        if (write(fd, encrypted + 20, 86) == 86)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }

    int _drcSetupNet(const char *ipAddress, const char *port, const char *password)
    {
        struct addrinfo hints;
        struct addrinfo *result, *rp;
        struct in6_addr serveraddr;
        int remoteFd;

        // Start by seeing if we've been given a (textual) numeric IP address
        //	which will save lookups in getaddrinfo()

        memset(&hints, 0, sizeof(hints));
        hints.ai_flags = AI_NUMERICSERV;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = 0;

        if (inet_pton(AF_INET, ipAddress, &serveraddr) == 1) // Valid IPv4
        {
            hints.ai_family = AF_INET;
            hints.ai_flags |= AI_NUMERICHOST;
        }
        else
        {
            if (inet_pton(AF_INET6, ipAddress, &serveraddr) == 1) // Valid IPv6
            {
                hints.ai_family = AF_INET6;
                hints.ai_flags |= AI_NUMERICHOST;
            }
        }

        // Now use getaddrinfo() with the newly supplied hints

        if (getaddrinfo(ipAddress, port, &hints, &result) != 0)
        {
            return -1;
        }

        // Now try each address in-turn until we get one that connects...

        for (rp = result; rp != NULL; rp = rp->ai_next)
        {
            if ((remoteFd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) < 0)
            {
                continue;
            }

            if (connect(remoteFd, rp->ai_addr, rp->ai_addrlen) < 0)
            {
                continue;
            }

            if (authenticate(remoteFd, password) < 0)
            {
                close(remoteFd);
                errno = EACCES; // Permission denied
                return -1;
            }
            else
            {
                return remoteFd;
            }
        }

        errno = EHOSTUNREACH; // Host unreachable - may not be right, but good enough
        return -1;            // Nothing connected
    }

    class drcNet : public wiringPiNode
    {
    public:
        template <const pin_t pinBase, const pin_t nPins_>
        [[nodiscard]] drcNet(const char *ipAddress, const char *port, const char *password)
            : fd_(drcSetupNet()){};

    private:
        const int fd_;
        [[nodiscard]] int drcSetupNet() const
        {
            return 0;
        }
        static constexpr const pin_t pinBase_ = pinBase;
        static constexpr const pin_t nPins_ = nPins;
    };
}

#endif
