#pragma once
#ifndef CL7_LOGGING_LOGGER_H
#define CL7_LOGGING_LOGGER_H

#include "./AbstractLogHandler.h"
#include "./LogEntry.h"

#include <vector>



namespace cl7 {
namespace logging {



class Logger
{

public:
    enum class Type
    {
        Log,
        Info,
        Success,
        Warning,
        Error,
        Caption,
        Section,
        Item,
        ItemPass,
        ItemFail,
        Code,
        Meta,
        Other
    }; // enum class Type

public:
    typedef std::shared_ptr<AbstractLogHandler> LogHandlerPtr;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    Logger(void) = default;

    /**
     * Destructor.
     */
    ~Logger(void) = default;

private:
    /** Copy constructor. */
    Logger(const Logger&) = delete;
    /** Copy assignment operator. */
    Logger& operator = (const Logger&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * A "list" of log handlers.
     */
    std::vector<LogHandlerPtr> _log_handlers;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * (Re-)adds the specified log handler.
     */
    Logger& add_log_handler(LogHandlerPtr log_handler);

    /**
     * Removes the specified log handler. Does nothing if not found.
     */
    Logger& remove_log_handler(LogHandlerPtr log_handler);

    /**
     * Removes all log handlers.
     */
    Logger& clear_log_handlers();

    /**
     * Logs the specified entry invoking the registered handlers.
     */
    Logger& log(const LogEntry& log_entry);



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Searches for the specified log handler and returns a matching iterator (if
     * found).
     */
    std::vector<LogHandlerPtr>::iterator _find_log_handler(LogHandlerPtr log_handler);

}; // class Logger



} // namespace logging
} // namespace cl7

#endif // CL7_LOGGING_LOGGER_H
