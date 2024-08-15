// ======================================================================== //
//                                                                          //
// piHiPriority.H                                                           //
//                                                                          //
// ======================================================================== //
// Attempts to set a high priority schedule for the running program         //
// ======================================================================== //

#ifndef __WIRING_PI_piHiPriority_H
#define __WIRING_PI_piHiPriority_H

namespace WiringPi
{
    class piHiPriority
    {
    public:
        [[nodiscard]] inline piHiPriority(const int priority)
            : success_(setSched(priority)) {};

        // Returns 0 if successfully set
        // Returns -1 if error
        inline int success() const
        {
            return success_;
        }

        // Returns policy for the thread
        // Returns -1 if error
        inline int schedule() const
        {
            return sched_getscheduler();
        }

    private:
        // Set a high priority schedulling for the running program
        int setSched(const int priority) const
        {
            struct sched_param sched;

            std::memset(&sched, 0, sizeof(sched));

            if (priority > sched_get_priority_max(SCHED_RR))
            {
                sched.sched_priority = sched_get_priority_max(SCHED_RR);
            }
            else
            {
                sched.sched_priority = priority;
            }

            return sched_setscheduler(0, SCHED_RR, &sched);
        }

        const int success_;
    };
}

#endif
