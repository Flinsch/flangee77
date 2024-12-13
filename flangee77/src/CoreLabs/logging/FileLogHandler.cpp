#include "FileLogHandler.h"

#include <CoreLabs/fstream.h>
#include <CoreLabs/sstream.h>
#include <CoreLabs/strings.h>

#include <chrono>



namespace cl7::logging {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    FileLogHandler::FileLogHandler()
    {
        constexpr cl7::astring_view flangee77 = "flangee77";

        const std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm tm;
        ::localtime_s(&tm, &t);
        const auto ldt = std::put_time(&tm, "%Y-%m-%d %H:%M:%S %z");
        std::ostringstream ldt_ss;
        ldt_ss << ldt;

        cl7::aosstream ss;
        ss << flangee77 << " \xe2\x80\x94 "; // \u2014 (em dash) as UTF-8
        ss << ldt_ss.str();

        _write_line(ss.str(), true);
    }



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Writes the specified log entry.
     */
    void FileLogHandler::_write(const LogEntry& log_entry)
    {
        std::ostringstream ss;

        switch (log_entry.type)
        {
        case cl7::logging::LogType::Info:
            ss << "[INFO] ";
            break;
        case cl7::logging::LogType::Success:
            ss << "[SUCCESS] ";
            break;
        case cl7::logging::LogType::Warning:
            ss << "[WARNING] ";
            break;
        case cl7::logging::LogType::Error:
            ss << "[ERROR] ";
            break;
        default:
            break; // Nothing to do here.
        }

        ss << cl7::strings::reinterpret_utf8(cl7::strings::to_utf8(log_entry.message));

        _write_line(ss.str());
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Writes the specified "raw" ASCII or UTF-8 encoded std::string to the log
     * file. If specified, the file is initially truncated.
     */
    void FileLogHandler::_write_line(std::string_view line, bool truncate)
    {
        auto mode = cl7::aofstream::binary | cl7::aofstream::out;
        if (truncate)
            mode |= cl7::aofstream::trunc;
        else
            mode |= cl7::aofstream::app;

        cl7::aofstream file("log.txt", mode);

        if (!file.is_open())
            return;

        auto utf8 = cl7::strings::to_utf8(line);
        utf8.push_back(u8'\n');

        //if (truncate)
        //    file.write(reinterpret_cast<const char*>(cl7::strings::to_bytes(u8"", true).data()), 3);
        auto data = cl7::strings::reinterpret_utf8(utf8);
        file.write(data.data(), static_cast<std::streamsize>(data.size()));
    }



} // namespace cl7::logging
