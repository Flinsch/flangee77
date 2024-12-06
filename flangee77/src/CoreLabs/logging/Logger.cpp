#include "Logger.h"

#include <algorithm>



namespace cl7 {
namespace logging {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * (Re-)adds the specified log handler.
     */
    Logger& Logger::add_log_handler(LogHandlerPtr log_handler)
    {
        remove_log_handler( log_handler );

        _log_handlers.push_back( log_handler );

        return *this;
    }

    /**
     * Removes the specified log handler. Does nothing if not found.
     */
    Logger& Logger::remove_log_handler(LogHandlerPtr log_handler)
    {
        auto it = _find_log_handler( log_handler );
        if ( it != _log_handlers.end() )
            _log_handlers.erase( it );

        return *this;
    }

    /**
     * Removes all log handlers.
     */
    Logger& Logger::clear_log_handlers()
    {
        _log_handlers.clear();

        return *this;
    }

    /**
     * Logs the specified entry invoking the registered handlers.
     */
    Logger& Logger::log(const LogEntry& log_entry)
    {
        for ( LogHandlerPtr& log_handler : _log_handlers )
            log_handler->on_log( log_entry );

        return *this;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Searches for the specified log handler and returns a matching iterator (if
     * found).
     */
    std::vector<Logger::LogHandlerPtr>::iterator Logger::_find_log_handler(LogHandlerPtr log_handler)
    {
        return std::find( _log_handlers.begin(), _log_handlers.end(), log_handler );
    }



} // namespace logging
} // namespace cl7
