#include "logging.h"



namespace cl7::logging {



    /**
     * Logs the specified entry to the standard logger.
     */
    void std_log(const LogEntry& log_entry)
    {
        StandardLogger::instance().log(log_entry);
    }

    /**
     * Logs the specified entry to the standard logger.
     */
    void std_log(cl7::u8string_view message, LogType type, const char* file_path, unsigned line_number, const char* function_name)
    {
        std_log({.message = message, .type = type, .file_path = file_path, .line_number = line_number, .function_name = function_name});
    }



} // namespace cl7::logging
