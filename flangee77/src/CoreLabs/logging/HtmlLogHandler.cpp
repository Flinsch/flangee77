#include "HtmlLogHandler.h"

#include <CoreLabs/sstream.h>
#include <CoreLabs/text/codec.h>

#include <chrono>
#include <fstream>



namespace cl7::logging {



    HtmlLogHandler::Block::Block(cl7::u8string_view block_class)
        : block_class(block_class)
    {
        try
        {
            _write_raw(u8"    <div class=\"" + this->block_class + u8"\">\n");
        }
        catch (...) {}
    }

    HtmlLogHandler::Block::~Block() noexcept
    {
        try
        {
            _write_raw(u8"    </div>\n");
            _write_raw(u8"    <div></div><div></div><div></div>\n");
        }
        catch (...) {}
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    HtmlLogHandler::HtmlLogHandler()
    {
        try
        {
            constexpr cl7::u8string_view title = u8"flangee77 &mdash; log.html";

            const std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm tm{};
            ::localtime_s(&tm, &t);
            std::wostringstream ldt_woss;
            ldt_woss << std::put_time(&tm, L"%Y-%m-%d %H:%M:%S %z");

            cl7::u8osstream oss;
            oss << "<!doctype html>\n";
            oss << "<html lang=\"en\">\n";
            oss << "<head>\n";
            oss << "  <meta charset=\"utf-8\">\n";
            oss << "  <title>" << title << "</title>\n";
            oss << "  <style>\n";
            oss << "    body { font-family: sans-serif; }\n";
            oss << "    .container { display: grid; gap: 0.25rem 1rem; grid-template-columns: 1fr auto auto auto; }\n";
            oss << "    .container > * {  }\n";
            oss << "    .container > * > .badge { border: 0.125rem solid; border-radius: 0.25rem; font-weight: bold; padding: 0 0.125rem; text-transform: capitalize; }\n";
            oss << "    .container > .info { color: #085786; }\n";
            oss << "    .container > .info > .badge { background-color: #085786; border-color: #085786; color: #ffffff; }\n";
            oss << "    .container > .success { color: #386a1f; }\n";
            oss << "    .container > .success > .badge { background-color: #386a1f; border-color: #386a1f; color: #ffffff; }\n";
            oss << "    .container > .warning { color: #8f4f00; }\n";
            oss << "    .container > .warning > .badge { background-color: #8f4f00; border-color: #8f4f00; color: #ffffff; }\n";
            oss << "    .container > .error { color: #b3261e; }\n";
            oss << "    .container > .error > .badge { background-color: #b3261e; border-color: #b3261e; color: #ffffff; }\n";
            oss << "    .container > .caption { font-weight: bold; }\n";
            oss << "    .container > .section { font-style: italic; }\n";
            oss << "    .container > .list { display: grid; gap: 0.125rem 1rem; grid-template-columns: auto 1fr; margin-left: 1rem; }\n";
            oss << "    .container > .list > .item {  }\n";
            oss << "    .container > .list > .item.key {  }\n";
            oss << "    .container > .list > .item.value { font-weight: bold; }\n";
            oss << "    .container > .list > .item.pass { color: #386a1f; }\n";
            oss << "    .container > .list > .item.fail { color: #b3261e; }\n";
            oss << "    .container > .code { font-family: monospace; margin-left: 1rem; }\n";
            oss << "    .container > .meta {  }\n";
            oss << "    .container > .comment { font-family: monospace; }\n";
            oss << "    .container > .other {  }\n";
            oss << "    .container > .file-path { font-family: sans-serif; font-size: small; font-style: oblique; }\n";
            oss << "    .container > .line-number { font-family: sans-serif; font-size: small; font-style: oblique; text-align: right; }\n";
            oss << "    .container > .function-name { font-family: monospace; font-size: small; font-style: oblique; }\n";
            oss << "  </style>\n";
            oss << "</head>\n";
            oss << "<body>\n";
            oss << "  <h1>" << title << "</h1>\n";
            oss << "  <p>Session start: " << _escape(cl7::text::codec::to_utf8(cl7::text::codec::reinterpret_utfx(ldt_woss.str()))) << "</p>\n";
            oss << "  <div class=\"container\">\n";

            _write_raw(oss.str(), true);
        }
        catch (...) {}
    }

    HtmlLogHandler::~HtmlLogHandler() noexcept
    {
        try
        {
            _block_ptr.reset();

            const std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm tm{};
            ::localtime_s(&tm, &t);
            std::wostringstream ldt_woss;
            ldt_woss << std::put_time(&tm, L"%Y-%m-%d %H:%M:%S %z");

            cl7::u8osstream oss;
            oss << "  </div>\n";
            oss << "  <p>Session end: " << _escape(cl7::text::codec::to_utf8(cl7::text::codec::reinterpret_utfx(ldt_woss.str()))) << "</p>\n";
            oss << "</body>\n";
            oss << "</html>\n";

            _write_raw(oss.str());
        }
        catch (...) {}
    }



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Writes the specified log entry.
     */
    void HtmlLogHandler::_write(const LogEntry& log_entry)
    {
        cl7::u8string_view entry_class;
        cl7::u8string_view block_class;
        cl7::u8string_view badge_class;
        switch (log_entry.type)
        {
        case cl7::logging::LogType::Info:       entry_class = u8"info";        badge_class = u8"info";      break;
        case cl7::logging::LogType::Success:    entry_class = u8"success";     badge_class = u8"success";   break;
        case cl7::logging::LogType::Warning:    entry_class = u8"warning";     badge_class = u8"warning";   break;
        case cl7::logging::LogType::Error:      entry_class = u8"error";       badge_class = u8"error";     break;
        case cl7::logging::LogType::Caption:    entry_class = u8"caption";     break;
        case cl7::logging::LogType::Section:    entry_class = u8"section";     break;
        case cl7::logging::LogType::Item:       entry_class = u8"item";        block_class = u8"list";      break;
        case cl7::logging::LogType::ItemPass:   entry_class = u8"item pass";   block_class = u8"list";      break;
        case cl7::logging::LogType::ItemFail:   entry_class = u8"item fail";   block_class = u8"list";      break;
        case cl7::logging::LogType::Code:       entry_class = u8"code";        block_class = u8"code";      break;
        case cl7::logging::LogType::Meta:       entry_class = u8"meta";        break;
        case cl7::logging::LogType::Comment:    entry_class = u8"comment";     break;
        case cl7::logging::LogType::Other:      entry_class = u8"other";       break;
        default: break; // Nothing to do here.
        }

        cl7::u8osstream oss;

        if (block_class.empty())
        {
            _block_ptr.reset();

            oss << "    <div class=\"" << entry_class << "\">\n";
            oss << "      ";
            if (!badge_class.empty())
                oss << "<span class=\"badge\">" << badge_class << "</span> ";
            oss << _escape(log_entry.message) << "\n";
            oss << "    </div>\n";
        }
        else // => block
        {
            if (_block_ptr && _block_ptr->block_class != block_class)
                _block_ptr.reset();
            if (!_block_ptr)
                _block_ptr = std::make_unique<Block>(block_class);

            if (log_entry.type == cl7::logging::LogType::Code)
            {
                oss << "      " << _escape(log_entry.message) << "<br>\n";
            }
            else // => list
            {
                cl7::u8string_view key;
                cl7::u8string_view value;
                size_t p = log_entry.message.find(u8'\t');
                if (p == cl7::u8string::npos)
                    key = log_entry.message;
                else
                    key = log_entry.message.substr(0, p),
                    value = log_entry.message.substr(p + 1);

                oss << "      <div class=\"" << entry_class << " key\">\n";
                oss << "        " << _escape(key) << "\n";
                oss << "      </div>\n";
                oss << "      <div class=\"" << entry_class << " value\">\n";
                if (!value.empty())
                    oss << "        " << _escape(value) << "\n";
                oss << "      </div>\n";
            }
        }

        _write_raw(oss.str());
        if (block_class.empty())
            _write_source_location(log_entry.file_path, log_entry.line_number, log_entry.function_name);
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Escapes the specified "raw" std::string and returns it as UTF-8 encoded text.
     */
    cl7::u8string HtmlLogHandler::_escape(std::string_view s)
    {
        return _escape(cl7::text::codec::reinterpret_utf8(s));
    }

    /**
     * Escapes the specified "raw" UTF-8 text.
     */
    cl7::u8string HtmlLogHandler::_escape(cl7::u8string_view u8s)
    {
        cl7::u8osstream oss;

        for (size_t i = 0; i < u8s.size(); ++i)
        {
            switch (const auto chr = u8s[i])
            {
            case u8'"':     oss << u8"&quot;";    break;
            case u8'&':     oss << u8"&amp;";     break;
            case u8'\'':    oss << u8"&#39;";     break;
            case u8'<':     oss << u8"&lt;";      break;
            case u8'>':     oss << u8"&gt;";      break;

            // Check for \u00a0 (no-break space), which is {0xc2, 0xa0} in UTF-8.
            case 0xc2:
                if (i + 1 < u8s.size() && u8s[i + 1] == 0xa0)
                {
                    ++i;
                    oss << u8"&nbsp;";
                    break;
                }

            default:
                oss << chr;
            }
        } // for each character

        return oss.str();
    }

    /**
     * Writes the specified "raw" UTF-8 encoded text to the log file without
     * escaping it. If specified, the file is initially truncated.
     */
    void HtmlLogHandler::_write_raw(cl7::u8string_view raw, bool truncate)
    {
        auto mode = std::ofstream::binary | std::ofstream::out;
        if (truncate)
            mode |= std::ofstream::trunc;
        else
            mode |= std::ofstream::app;

        std::ofstream file("log.html", mode);

        if (!file.is_open())
            return;

        //if (truncate)
        //    file.write(reinterpret_cast<const char*>(cl7::text::to_bytes(u8"", true).data()), 3);
        file.write(reinterpret_cast<const char*>(raw.data()), static_cast<std::streamsize>(raw.size()));
    }

    /**
     * Writes certain information about the source code location from which the log
     * entry came to the log file.
     */
    void HtmlLogHandler::_write_source_location(const char* file_path, unsigned line_number, const char* function_name)
    {
#pragma message("Consider using std::source_location as a better alternative to __FILE__, __LINE__, etc.")

        cl7::u8osstream oss;
        oss << "    <div class=\"file-path\">\n";
        if (file_path)
        {
            constexpr std::string_view file_path_view{__FILE__};
            constexpr size_t file_path_skip = file_path_view.find("flangee77");
            static_assert(file_path_skip != std::string_view::npos);

            oss << "      " << _escape(file_path + file_path_skip) << "\n";
        }
        oss << "    </div>\n";
        oss << "    <div class=\"line-number\">\n";
        if (file_path)
        {
            oss << "      " << line_number << "\n";
        }
        oss << "    </div>\n";
        oss << "    <div class=\"function-name\">\n";
        if (function_name)
        {
            const std::string_view function_name_view{function_name};
            const size_t function_name_skip = function_name_view.rfind("::");

            oss << "      " << _escape(function_name_skip != std::string_view::npos ? function_name_view.substr(function_name_skip + 2) : function_name) << "\n";
        }
        oss << "    </div>\n";

        _write_raw(oss.str());
    }



} // namespace cl7::logging
