#include "CoutLogHandler.h"

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
#ifdef UNICODE
        std::wcout << log_entry.message << '\n';
#else
        std::cout << log_entry.message << '\n';
#endif
    }



} // namespace cl7::logging
