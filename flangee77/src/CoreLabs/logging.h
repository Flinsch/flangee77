#ifndef CL7_LOGGING_H
#define CL7_LOGGING_H

#include <CoreLabs/logging/StandardLogger.h>



namespace cl7::logging {



    /**
     * Logs the specified entry to the standard logger.
     */
    void std_log(const LogEntry& log_entry);

    /**
     * Logs the specified entry to the standard logger.
     */
    void std_log(cl7::u8string_view message, LogType type, const char* file_path = nullptr, unsigned line_number = 0, const char* function_name = nullptr);



} // namespace cl7::logging



#define LOG(msg)                    std_log(msg, cl7::logging::LogType::Log,       __FILE__, __LINE__, __FUNCTION__)
#define LOG_INFO(msg)               std_log(msg, cl7::logging::LogType::Info,      __FILE__, __LINE__, __FUNCTION__)
#define LOG_SUCCESS(msg)            std_log(msg, cl7::logging::LogType::Success,   __FILE__, __LINE__, __FUNCTION__)
#define LOG_WARNING(msg)            std_log(msg, cl7::logging::LogType::Warning,   __FILE__, __LINE__, __FUNCTION__)
#define LOG_ERROR(msg)              std_log(msg, cl7::logging::LogType::Error,     __FILE__, __LINE__, __FUNCTION__)
#define LOG_TYPE(msg, type)         std_log(msg, type,                             __FILE__, __LINE__, __FUNCTION__)



#endif // CL7_LOGGING_H
