#include "AbstractLogHandler.h"



namespace cl7::logging {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Logs the specified entry.
     */
    void AbstractLogHandler::on_log(const LogEntry& log_entry)
    {
        _write(log_entry);
    }



} // namespace cl7::logging
