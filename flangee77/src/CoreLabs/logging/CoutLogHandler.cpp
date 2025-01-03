#include "CoutLogHandler.h"

#include <CoreLabs/strings.h>

#include <iostream>



namespace cl7::logging {



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Writes the specified log entry.
     */
    void CoutLogHandler::_write(const LogEntry& log_entry)
    {
        std::cout << cl7::strings::reinterpret_utf8(log_entry.message) << '\n';
    }



} // namespace cl7::logging
