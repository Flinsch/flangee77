#ifndef CL7_SYSTEM_DATETIME_H
#define CL7_SYSTEM_DATETIME_H

#include <CoreLabs/root.h>



namespace cl7::system {



struct datetime
{

    /**
     * Returns the number of milliseconds since the "epoch". Attention: This epoch
     * is local. Thus, the returned value should be shared only locally, not across
     * different systems and/or processes. But the clock in use is "steady", so it
     * is okay to share durations between two points in time.
     * 
     */
    static long long current_msecs_since_epoch();

}; // struct datetime



} // namespace cl7::system

#endif // CL7_SYSTEM_DATETIME_H
