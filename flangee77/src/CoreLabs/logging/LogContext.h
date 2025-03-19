#ifndef CL7_LOGGING_LOGCONTEXT_H
#define CL7_LOGGING_LOGCONTEXT_H

#include "./Logger.h"



namespace cl7::logging {



struct LogContext
{

    /** The flag specifying whether logging is enabled. */
    bool logging_enabled = true;
    /** The specific logger instance (NULL: standard logger). */
    Logger* logger = nullptr;



    /**
     * Tries to log the specified entry.
     */
    void try_log(const LogEntry& log_entry) const;

    /**
     * Tries to log the specified entry.
     */
    void try_log(cl7::u8string_view message, LogType type, const char* file_path = nullptr, unsigned line_number = 0, const char* function_name = nullptr) const;

}; // class LogEntry



} // namespace cl7::logging

#endif // CL7_LOGGING_LOGCONTEXT_H
