#include "logging.h"

#include "./logging/CoutLogHandler.h"



namespace cl7 {
namespace logging {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    StandardLogger::StandardLogger(void)
        : _logger()
    {
        _logger.add_log_handler( std::make_shared<CoutLogHandler>() );
    }



    // #############################################################################
    // Utility Functions
    // #############################################################################

    /**
     * Logs the specified entry to the standard logger.
     */
    void std_log(const LogEntry& log_entry)
    {
        StandardLogger::obj()->log( log_entry );
    }

    /**
     * Logs the specified entry to the standard logger.
     */
    void std_log(const cl7::string& message, LogType type, const cl7::char_type* file_path, unsigned line_number, const cl7::char_type* function_name)
    {
        std_log( { message, type, file_path, line_number, function_name } );
    }



} // namespace logging
} // namespace cl7
