#include "FileLogHandler.h"

#include <CoreLabs/sstream.h>
#include <CoreLabs/strings/codec.h>

#include <chrono>
#include <fstream>



namespace cl7::logging {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    FileLogHandler::FileLogHandler()
    {
        constexpr cl7::u8string_view flangee77 = u8"flangee77";

        const std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm tm{};
        ::localtime_s(&tm, &t);
        std::wostringstream ldt_woss;
        ldt_woss << std::put_time(&tm, L"%Y-%m-%d %H:%M:%S %z");

        cl7::u8osstream oss;
        oss << flangee77 << u8" \xe2\x80\x94 "; // \u2014 (em dash) as UTF-8
        oss << cl7::strings::codec::to_utf8(cl7::strings::codec::reinterpret_utfx(ldt_woss.str()));

        _write_line(oss.str(), true);
    }



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Writes the specified log entry.
     */
    void FileLogHandler::_write(const LogEntry& log_entry)
    {
        cl7::u8osstream oss;

        switch (log_entry.type)
        {
        case cl7::logging::LogType::Info:
            oss << "[INFO] ";
            break;
        case cl7::logging::LogType::Success:
            oss << "[SUCCESS] ";
            break;
        case cl7::logging::LogType::Warning:
            oss << "[WARNING] ";
            break;
        case cl7::logging::LogType::Error:
            oss << "[ERROR] ";
            break;
        default:
            break; // Nothing to do here.
        }

        oss << log_entry.message;

        _write_line(oss.str());
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Writes the specified "raw" UTF-8 encoded string to the log file. If
     * specified, the file is initially truncated.
     */
    void FileLogHandler::_write_line(cl7::u8string_view line, bool truncate)
    {
        auto mode = std::ofstream::binary | std::ofstream::out;
        if (truncate)
            mode |= std::ofstream::trunc;
        else
            mode |= std::ofstream::app;

        std::ofstream file("log.txt", mode);

        if (!file.is_open())
            return;

        //if (truncate)
        //    file.write(reinterpret_cast<const char*>(cl7::strings::to_bytes(u8"", true).data()), 3);
        file.write(reinterpret_cast<const char*>(line.data()), static_cast<std::streamsize>(line.size()));
        file.put('\n');
    }



} // namespace cl7::logging
