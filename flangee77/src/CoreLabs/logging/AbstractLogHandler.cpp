#include "AbstractLogHandler.h"



namespace cl7 {
namespace logging {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Logs the specified entry.
     */
    void AbstractLogHandler::on_log(const LogEntry& log_entry)
    {
        _write( log_entry );
    }



} // namespace logging
} // namespace cl7
