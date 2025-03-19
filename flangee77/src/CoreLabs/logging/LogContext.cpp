#include "LogContext.h"

#include <CoreLabs/logging/StandardLogger.h>



namespace cl7::logging {



    /**
     * Tries to log the specified entry.
     */
    void LogContext::try_log(const LogEntry& log_entry) const
    {
        if (logging_enabled)
        {
            (logger ? logger : &StandardLogger::instance())->log(log_entry);
        }
    }

    /**
     * Tries to log the specified entry.
     */
    void LogContext::try_log(cl7::u8string_view message, LogType type, const char* file_path, unsigned line_number, const char* function_name) const
    {
        try_log({.message=message, .type=type, .file_path=file_path, .line_number=line_number, .function_name=function_name});
    }



} // namespace cl7::logging
