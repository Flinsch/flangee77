#pragma once
#ifndef CL7_LOGGING_H
#define CL7_LOGGING_H
#include <CoreLabs/creational/Singleton.h>

#include <CoreLabs/logging/Logger.h>



namespace cl7 {
namespace logging {



class StandardLogger final
    : public creational::Singleton<StandardLogger>
{
    friend creational::Singleton<StandardLogger>;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
private:
    /**
     * Default constructor.
     */
    StandardLogger();

    /**
     * Destructor.
     */
    ~StandardLogger() = default;

private:
    /** Copy constructor. */
    StandardLogger(const StandardLogger&) = delete;
    /** Copy assignment operator. */
    StandardLogger& operator = (const StandardLogger&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The actual logger.
     */
    Logger _logger;



    // #############################################################################
    // Dereferenciation
    // #############################################################################
public:
    /**
     * Returns the actual logger.
     */
    static Logger& instance() { return creational::Singleton<StandardLogger>::instance()._logger; }

    /**
     * Returns the actual logger.
     */
    Logger* operator -> () { return &_logger; }

}; // class StandardLogger final



    // #############################################################################
    // Utility Functions
    // #############################################################################

    /**
     * Logs the specified entry to the standard logger.
     */
    void std_log(const LogEntry& log_entry);

    /**
     * Logs the specified entry to the standard logger.
     */
    void std_log(const cl7::string& message, LogType type, const cl7::char_type* file_path = nullptr, unsigned line_number = 0, const cl7::char_type* function_name = nullptr);



} // namespace logging
} // namespace cl7



#define LOG(msg)                    std_log( msg, cl7::logging::LogType::Log,       TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__) )
#define LOG_INFO(msg)               std_log( msg, cl7::logging::LogType::Info,      TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__) )
#define LOG_SUCCESS(msg)            std_log( msg, cl7::logging::LogType::Success,   TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__) )
#define LOG_WARNING(msg)            std_log( msg, cl7::logging::LogType::Warning,   TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__) )
#define LOG_ERROR(msg)              std_log( msg, cl7::logging::LogType::Error,     TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__) )
#define LOG_TYPE(msg, type)         std_log( msg, type,                             TEXT(__FILE__), __LINE__, TEXT(__FUNCTION__) )



#endif // CL7_LOGGING_H
