// ======================================================================== //
//                                                                          //
// piThread.H                                                               //
//                                                                          //
// ======================================================================== //
// Implements multithreading for the wiringPi library                       //
// Uses std::thread instead of POSIX threads                                //
// Maybe implement POSIX threads in future                                  //
// Possibly implement conditional compilation with preprocessor directive   //
// ======================================================================== //

#ifndef __WIRING_PI_piThread_H
#define __WIRING_PI_piThread_H

namespace WiringPi
{
    class piThread
    {
    public:
        [[nodiscard]] inline constexpr piThread() {};

        // Activate a mutex
        template <const size_t key>
        inline void lock() const
        {
            static_assert(key < 5);
            piMutexes_[key].lock();
        }

        // Deactivate a mutex
        template <const size_t key>
        inline void unlock() const
        {
            static_assert(key < 5);
            piMutexes_[key].unlock();
        }

    private:
        // Array of mutexes
        std::array<std::mutex, 4> piMutexes_;
    };
}

#endif
