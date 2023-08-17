#include "CoutLogHandler.h"

#include <iostream>



namespace cl7 {
namespace logging {



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Writes the specified log entry.
     */
    void CoutLogHandler::_write(const LogEntry& log_entry)
    {
#ifdef UNICODE
        std::wcout << log_entry.message << std::endl;
#else
        std::cout << log_entry.message << std::endl;
#endif
    }



} // namespace logging
} // namespace cl7
