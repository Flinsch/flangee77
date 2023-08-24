#include "FileLogHandler.h"

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
    FileLogHandler::FileLogHandler(void)
    {
        const std::time_t t = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
        std::tm tm;
        ::localtime_s( &tm, &t );
        const auto ldt = std::put_time( &tm, TEXT("%F %T %z") );

        cl7::osstream ss;
        ss << TEXT("flangee77 - ");
        ss << ldt;

        _write_line( ss.str(), true );
    }



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Writes the specified log entry.
     */
    void FileLogHandler::_write(const LogEntry& log_entry)
    {
        switch ( log_entry.type )
        {
        case cl7::logging::LogType::Info:
            _write_line( TEXT("[INFO] ") + cl7::string{log_entry.message} );
            break;
        case cl7::logging::LogType::Success:
            _write_line( TEXT("[SUCCESS] ") + cl7::string{log_entry.message} );
            break;
        case cl7::logging::LogType::Warning:
            _write_line( TEXT("[WARNING] ") + cl7::string{log_entry.message} );
            break;
        case cl7::logging::LogType::Error:
            _write_line( TEXT("[ERROR] ") + cl7::string{log_entry.message} );
            break;
        default:
            _write_line( log_entry.message );
        }
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Writes the specified text line to the log file.
     */
    void FileLogHandler::_write_line(cl7::string_view line, bool truncate)
    {
        auto mode = cl7::ofstream::out;
        if ( truncate )
            mode |= cl7::ofstream::trunc;
        else
            mode |= cl7::ofstream::app;

        cl7::ofstream file( TEXT("log.txt"), mode );

        if ( !file.is_open() )
            return;

        file << line << std::endl;
    }



} // namespace logging
} // namespace cl7
