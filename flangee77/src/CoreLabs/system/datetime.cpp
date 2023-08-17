#include "datetime.h"

#include <chrono>



namespace cl7 {
namespace system {



    // #############################################################################
    // Static Methods
    // #############################################################################

    /**
     * Returns the number of milliseconds since the "epoch". Attention: This epoch
     * is local. Thus, the returned value should be shared only locally, not across
     * different systems and/or processes. But the clock in use is "steady", so it
     * is okay to share durations between two points in time.
     */
    long long datetime::current_msecs_since_epoch()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now().time_since_epoch() ).count();
    }



} // namespace system
} // namespace cl7
