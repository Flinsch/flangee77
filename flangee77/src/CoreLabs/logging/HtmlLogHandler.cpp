#include "HtmlLogHandler.h"

#include <CoreLabs/fstream.h>
#include <CoreLabs/sstream.h>
#include <CoreLabs/strings.h>

#include <chrono>



namespace cl7::logging {



    HtmlLogHandler::Block::Block(std::string_view block_class)
        : block_class(block_class)
    {
        _write_raw("    <div class=\"" + this->block_class + "\">\n");
    }

    HtmlLogHandler::Block::~Block()
    {
        _write_raw("    </div>\n");
        _write_raw("    <div></div><div></div><div></div>\n");
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    HtmlLogHandler::HtmlLogHandler()
    {
        constexpr std::string_view title = "flangee77 &mdash; log.html";

        const std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm tm{};
        ::localtime_s(&tm, &t);
        const auto ldt = std::put_time(&tm, "%Y-%m-%d %H:%M:%S %z");
        std::ostringstream ldt_ss;
        ldt_ss << ldt;

        std::ostringstream ss;
        ss << "<!doctype html>\n";
        ss << "<html lang=\"en\">\n";
        ss << "<head>\n";
        ss << "  <meta charset=\"utf-8\">\n";
        ss << "  <title>" << title << "</title>\n";
        ss << "  <style>\n";
        ss << "    body { font-family: sans-serif; }\n";
        ss << "    .container { display: grid; gap: 0.25rem 1rem; grid-template-columns: 1fr auto auto auto; }\n";
        ss << "    .container > * {  }\n";
        ss << "    .container > * > .badge { border: 0.125rem solid; border-radius: 0.25rem; font-weight: bold; padding: 0 0.125rem; text-transform: capitalize; }\n";
        ss << "    .container > .info { color: #085786; }\n";
        ss << "    .container > .info > .badge { background-color: #085786; border-color: #085786; color: #ffffff; }\n";
        ss << "    .container > .success { color: #386a1f; }\n";
        ss << "    .container > .success > .badge { background-color: #386a1f; border-color: #386a1f; color: #ffffff; }\n";
        ss << "    .container > .warning { color: #8f4f00; }\n";
        ss << "    .container > .warning > .badge { background-color: #8f4f00; border-color: #8f4f00; color: #ffffff; }\n";
        ss << "    .container > .error { color: #b3261e; }\n";
        ss << "    .container > .error > .badge { background-color: #b3261e; border-color: #b3261e; color: #ffffff; }\n";
        ss << "    .container > .caption { font-weight: bold; }\n";
        ss << "    .container > .section { font-style: italic; }\n";
        ss << "    .container > .list { display: grid; gap: 0.125rem 1rem; grid-template-columns: auto 1fr; margin-left: 1rem; }\n";
        ss << "    .container > .list > .item {  }\n";
        ss << "    .container > .list > .item.key {  }\n";
        ss << "    .container > .list > .item.value { font-weight: bold; }\n";
        ss << "    .container > .list > .item.pass { color: #386a1f; }\n";
        ss << "    .container > .list > .item.fail { color: #b3261e; }\n";
        ss << "    .container > .code { font-family: monospace; margin-left: 1rem; }\n";
        ss << "    .container > .meta {  }\n";
        ss << "    .container > .comment { font-family: monospace; }\n";
        ss << "    .container > .other {  }\n";
        ss << "    .container > .file-path { font-family: sans-serif; font-size: small; font-style: oblique; }\n";
        ss << "    .container > .line-number { font-family: sans-serif; font-size: small; font-style: oblique; text-align: right; }\n";
        ss << "    .container > .function-name { font-family: monospace; font-size: small; font-style: oblique; }\n";
        ss << "  </style>\n";
        ss << "</head>\n";
        ss << "<body>\n";
        ss << "  <h1>" << title << "</h1>\n";
        ss << "  <p>Session start: " << _escape(ldt_ss.str()) << "</p>\n";
        ss << "  <div class=\"container\">\n";

        _write_raw(ss.str(), true);
    }

    HtmlLogHandler::~HtmlLogHandler()
    {
        _block_ptr.reset();

        const std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm tm{};
        ::localtime_s(&tm, &t);
        const auto ldt = std::put_time(&tm, "%Y-%m-%d %H:%M:%S %z");
        std::ostringstream ldt_ss;
        ldt_ss << ldt;

        std::ostringstream ss;
        ss << "  </div>\n";
        ss << "  <p>Session end: " << _escape(ldt_ss.str()) << "</p>\n";
        ss << "</body>\n";
        ss << "</html>\n";

        _write_raw(ss.str());
    }



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Writes the specified log entry.
     */
    void HtmlLogHandler::_write(const LogEntry& log_entry)
    {
        std::string_view entry_class;
        std::string_view block_class;
        std::string_view badge_class;
        switch (log_entry.type)
        {
        case cl7::logging::LogType::Info:       entry_class = "info";        badge_class = "info";      break;
        case cl7::logging::LogType::Success:    entry_class = "success";     badge_class = "success";   break;
        case cl7::logging::LogType::Warning:    entry_class = "warning";     badge_class = "warning";   break;
        case cl7::logging::LogType::Error:      entry_class = "error";       badge_class = "error";     break;
        case cl7::logging::LogType::Caption:    entry_class = "caption";     break;
        case cl7::logging::LogType::Section:    entry_class = "section";     break;
        case cl7::logging::LogType::Item:       entry_class = "item";        block_class = "list";      break;
        case cl7::logging::LogType::ItemPass:   entry_class = "item pass";   block_class = "list";      break;
        case cl7::logging::LogType::ItemFail:   entry_class = "item fail";   block_class = "list";      break;
        case cl7::logging::LogType::Code:       entry_class = "code";        block_class = "code";      break;
        case cl7::logging::LogType::Meta:       entry_class = "meta";        break;
        case cl7::logging::LogType::Comment:    entry_class = "comment";     break;
        case cl7::logging::LogType::Other:      entry_class = "other";       break;
        default: break; // Nothing to do here.
        }

        std::ostringstream ss;

        if (block_class.empty())
        {
            _block_ptr.reset();

            ss << "    <div class=\"" << entry_class << "\">\n";
            ss << "      ";
            if (!badge_class.empty())
                ss << "<span class=\"badge\">" << badge_class << "</span> ";
            ss << _escape(log_entry.message) << "\n";
            ss << "    </div>\n";
        }
        else // => block
        {
            if (_block_ptr && _block_ptr->block_class != block_class)
                _block_ptr.reset();
            if (!_block_ptr)
                _block_ptr = std::make_unique<Block>(block_class);

            if (log_entry.type == cl7::logging::LogType::Code)
            {
                ss << "      " << _escape(log_entry.message) << "<br>\n";
            }
            else // => list
            {
                cl7::string_view key;
                cl7::string_view value;
                size_t p = log_entry.message.find(TEXT('\t'));
                if (p == cl7::string::npos)
                    key = log_entry.message;
                else
                    key = log_entry.message.substr(0, p),
                    value = log_entry.message.substr(p + 1);

                ss << "      <div class=\"" << entry_class << " key\">\n";
                ss << "        " << _escape(key) << "\n";
                ss << "      </div>\n";
                ss << "      <div class=\"" << entry_class << " value\">\n";
                if (!value.empty())
                    ss << "        " << _escape(value) << "\n";
                ss << "      </div>\n";
            }
        }

        _write_raw(ss.str());
        if (block_class.empty())
            _write_source_location(log_entry.file_path, log_entry.line_number, log_entry.function_name);
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Escapes the specified "raw" text and returns it as UTF-8 encoded std::string.
     */
    std::string HtmlLogHandler::_escape(cl7::string_view text)
    {
        return _escape(cl7::strings::to_utf8(text));
    }

    /**
     * Escapes the specified "raw" ASCII/Latin-1 text and returns it as UTF-8
     * encoded std::string.
     */
    std::string HtmlLogHandler::_escape(cl7::astring_view as)
    {
        if (cl7::strings::check_ascii(as))
            return _escape(cl7::strings::reinterpret_utf8(as));
        return _escape(cl7::strings::to_utf8(as));
    }

    /**
     * Escapes the specified "raw" UTF-8 text and returns it as UTF-8 encoded
     * std::string.
     */
    std::string HtmlLogHandler::_escape(cl7::u8string_view u8s)
    {
        std::ostringstream ss;

        for (size_t i = 0; i < u8s.size(); ++i)
        {
            switch (const auto chr = u8s[i])
            {
            case '"':       ss << "&quot;";   break;
            case '&':       ss << "&amp;";    break;
            case '\'':      ss << "&#39;";    break;
            case '<':       ss << "&lt;";     break;
            case '>':       ss << "&gt;";     break;

            // Check for \u00a0, which is 0xc2 0xa0 in UTF-8.
            case 0xc2:
                if (i + 1 < u8s.size() && u8s[i + 1] == 0xa0)
                {
                    ++i;
                    ss << "&nbsp;";
                    break;
                }

            default:
                ss << static_cast<char>(chr);
            }
        } // for each character

        return ss.str();
    }

    /**
     * Writes the specified "raw" ASCII/Latin-1 or UTF-8 encoded std::string to the
     * log file without escaping it. If specified, the file is initially truncated.
     */
    void HtmlLogHandler::_write_raw(std::string_view raw, bool truncate)
    {
        auto mode = cl7::aofstream::binary | cl7::aofstream::out;
        if (truncate)
            mode |= cl7::aofstream::trunc;
        else
            mode |= cl7::aofstream::app;

        cl7::aofstream file(TEXT("log.html"), mode);

        if (!file.is_open())
            return;

        //if (truncate)
        //    file.write(reinterpret_cast<const char*>(cl7::strings::to_bytes(u8"", true).data()), 3);
        file.write(raw.data(), static_cast<std::streamsize>(raw.size()));
    }

    /**
     * Writes certain information about the source code location from which the log
     * entry came to the log file.
     */
    void HtmlLogHandler::_write_source_location(const cl7::achar_type* file_path, unsigned line_number, const cl7::achar_type* function_name)
    {
#pragma message("Consider using std::source_location as a better alternative to __FILE__, __LINE__, etc.")

        std::ostringstream ss;
        ss << "    <div class=\"file-path\">\n";
        if (file_path)
        {
            constexpr std::string_view file_path_view{__FILE__};
            constexpr size_t file_path_skip = file_path_view.find("flangee77");
            static_assert(file_path_skip != std::string_view::npos);

            ss << "      " << _escape(file_path + file_path_skip) << "\n";
        }
        ss << "    </div>\n";
        ss << "    <div class=\"line-number\">\n";
        if (file_path)
        {
            ss << "      " << line_number << "\n";
        }
        ss << "    </div>\n";
        ss << "    <div class=\"function-name\">\n";
        if (function_name)
        {
            const std::string_view function_name_view{function_name};
            const size_t function_name_skip = function_name_view.rfind("::");

            ss << "      " << _escape(function_name_skip != std::string_view::npos ? function_name_view.substr(function_name_skip + 2) : function_name) << "\n";
        }
        ss << "    </div>\n";

        _write_raw(ss.str());
    }



} // namespace cl7::logging
