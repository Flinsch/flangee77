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
    void std_log(cl7::string_view message, LogType type, const cl7::achar_type* file_path, unsigned line_number, const cl7::achar_type* function_name)
    {
        std_log({message, type, file_path, line_number, function_name});
    }



} // namespace cl7::logging
