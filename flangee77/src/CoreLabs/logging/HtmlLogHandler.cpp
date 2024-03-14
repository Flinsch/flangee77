#include "HtmlLogHandler.h"

#include <CoreLabs/fstream.h>
#include <CoreLabs/sstream.h>

#include <chrono>



namespace cl7 {
namespace logging {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    HtmlLogHandler::HtmlLogHandler()
    {
        const std::time_t t = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
        std::tm tm;
        ::localtime_s( &tm, &t );
        const auto ldt = std::put_time( &tm, TEXT("%F %T %z") );

        cl7::osstream ss;
        ss << TEXT("flangee77 - ");
        ss << ldt;

        
    }



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Writes the specified log entry.
     */
    void HtmlLogHandler::_write(const LogEntry& log_entry)
    {
        
    }



} // namespace logging
} // namespace cl7
