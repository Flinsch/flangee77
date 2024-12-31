#ifndef CL7_LOGGING_LOGGER_H
#define CL7_LOGGING_LOGGER_H

#include "./AbstractLogHandler.h"
#include "./LogEntry.h"

#include <vector>



namespace cl7::logging {



class Logger final
{

public:
    using LogHandler = AbstractLogHandler;
    using LogHandlerPtr = std::shared_ptr<LogHandler>;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Logger() = default;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    ~Logger() noexcept = default;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * (Re-)adds the specified log handler.
     */
    Logger& add_log_handler(const LogHandlerPtr& log_handler);

    /**
     * Removes the specified log handler. Does nothing if not found.
     */
    Logger& remove_log_handler(const LogHandlerPtr& log_handler);

    /**
     * Removes all log handlers.
     */
    Logger& clear_log_handlers();

    /**
     * Logs the specified entry invoking the registered handlers.
     */
    Logger& log(const LogEntry& log_entry);



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Searches for the specified log handler and returns a matching iterator (if
     * found).
     */
    std::vector<LogHandlerPtr>::iterator _find_log_handler(const LogHandlerPtr& log_handler);



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * A "list" of log handlers.
     */
    std::vector<LogHandlerPtr> _log_handlers;

}; // class Logger



} // namespace cl7::logging

#endif // CL7_LOGGING_LOGGER_H
