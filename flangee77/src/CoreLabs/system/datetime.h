#pragma once
#ifndef CL7_SYSTEM_DATETIME_H
#define CL7_SYSTEM_DATETIME_H

#include <CoreLabs/root.h>



namespace cl7 {
namespace system {



class datetime
{

    // #############################################################################
    // Static Methods
    // #############################################################################
public:
    /**
     * Returns the number of milliseconds since the "epoch". Attention: This epoch
     * is local. Thus, the returned value should be shared only locally, not across
     * different systems and/or processes. But the clock in use is "steady", so it
     * is okay to share durations between two points in time.
     * 
     */
    static long long current_msecs_since_epoch();

}; // class datetime



} // namespace system
} // namespace cl7

#endif // CL7_SYSTEM_DATETIME_H
