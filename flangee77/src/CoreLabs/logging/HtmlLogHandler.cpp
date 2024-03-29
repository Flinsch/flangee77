#include "HtmlLogHandler.h"

#include <CoreLabs/fstream.h>
#include <CoreLabs/sstream.h>
#include <CoreLabs/strings.h>

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
        constexpr cl7::string_view flangee77 = TEXT("flangee77");
        constexpr cl7::string_view title = TEXT("flangee77 &mdash; log.html");

        const std::time_t t = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
        std::tm tm;
        ::localtime_s( &tm, &t );
        const auto ldt = std::put_time( &tm, TEXT("%F %T %z") );

        cl7::osstream ss;
        ss << TEXT("<!doctype html>\n");
        ss << TEXT("<html lang=\"en\">\n");
        ss << TEXT("<head>\n");
        ss << TEXT("  <meta charset=\"utf-8\">\n");
        ss << TEXT("  <title>") << title << TEXT("</title>\n");
        ss << TEXT("  <style>\n");
        ss << TEXT("    body { font-family: sans-serif; }\n");
        ss << TEXT("    .container { display: grid; gap: 0.25rem 1rem; }\n");
        ss << TEXT("    .container > * {  }\n");
        ss << TEXT("    .container > * > .badge { border: 0.125rem solid; border-radius: 0.25rem; font-weight: bold; padding: 0 0.125rem; }\n");
        ss << TEXT("    .container > .info > .badge { background-color: #085786; border-color: #085786; color: #ffffff; }\n");
        ss << TEXT("    .container > .success > .badge { background-color: #386a1f; border-color: #386a1f; color: #ffffff; }\n");
        ss << TEXT("    .container > .warning > .badge { background-color: #8f4f00; border-color: #8f4f00; color: #ffffff; }\n");
        ss << TEXT("    .container > .error > .badge { background-color: #b3261e; border-color: #b3261e; color: #ffffff; }\n");
        ss << TEXT("    .container > .caption { font-weight: bold; }\n");
        ss << TEXT("    .container > .section { font-style: italic; }\n");
        ss << TEXT("    .container > .item {  }\n");
        ss << TEXT("    .container > .item.pass { color: #386a1f; }\n");
        ss << TEXT("    .container > .item.fail { color: #b3261e; }\n");
        ss << TEXT("    .container > .code { font-family: monospace; }\n");
        ss << TEXT("    .container > .meta {  }\n");
        ss << TEXT("    .container > .comment { font-family: monospace; }\n");
        ss << TEXT("    .container > .other {  }\n");
        ss << TEXT("  </style>\n");
        ss << TEXT("</head>\n");
        ss << TEXT("<body>\n");
        ss << TEXT("  <h1>") << title << TEXT("</h1>\n");
        ss << TEXT("  <div class=\"container\">\n");

        _write_raw( ss.str(), true );
    }

    /**
     * Destructor.
     */
    HtmlLogHandler::~HtmlLogHandler()
    {
        cl7::osstream ss;
        ss << TEXT("  </div>\n");
        ss << TEXT("</body>\n");
        ss << TEXT("</html>\n");

        _write_raw( ss.str() );
    }



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Writes the specified log entry.
     */
    void HtmlLogHandler::_write(const LogEntry& log_entry)
    {
        cl7::string_view entry_class;
        switch ( log_entry.type )
        {
        case cl7::logging::LogType::Info:       entry_class = TEXT("info");         break;
        case cl7::logging::LogType::Success:    entry_class = TEXT("success");      break;
        case cl7::logging::LogType::Warning:    entry_class = TEXT("warning");      break;
        case cl7::logging::LogType::Error:      entry_class = TEXT("error");        break;
        case cl7::logging::LogType::Caption:    entry_class = TEXT("caption");      break;
        case cl7::logging::LogType::Section:    entry_class = TEXT("section");      break;
        case cl7::logging::LogType::Item:       entry_class = TEXT("item");         break;
        case cl7::logging::LogType::ItemPass:   entry_class = TEXT("item pass");    break;
        case cl7::logging::LogType::ItemFail:   entry_class = TEXT("item fail");    break;
        case cl7::logging::LogType::Code:       entry_class = TEXT("code");         break;
        case cl7::logging::LogType::Meta:       entry_class = TEXT("meta");         break;
        case cl7::logging::LogType::Comment:    entry_class = TEXT("comment");      break;
        case cl7::logging::LogType::Other:      entry_class = TEXT("other");        break;
        default:
            entry_class = TEXT("");
        }

        cl7::osstream ss;
        ss << TEXT("  <div class=\"") << entry_class << TEXT("\">\n");

        switch ( log_entry.type )
        {
        case cl7::logging::LogType::Info:
            ss << TEXT("<span class=\"badge\">Info</span> ") << _escape( log_entry.message );
            break;
        case cl7::logging::LogType::Success:
            ss << TEXT("<span class=\"badge\">Success</span> ") << _escape( log_entry.message );
            break;
        case cl7::logging::LogType::Warning:
            ss << TEXT("<span class=\"badge\">Warning</span> ") << _escape( log_entry.message );
            break;
        case cl7::logging::LogType::Error:
            ss << TEXT("<span class=\"badge\">Error</span> ") << _escape( log_entry.message );
            break;
        default:
            ss << _escape( log_entry.message );
        }

        ss << TEXT("  </div>\n");

        _write_raw( ss.str() );
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Escapes the specified "raw" text.
     */
    cl7::string HtmlLogHandler::_escape(cl7::string_view text) const
    {
        cl7::osstream ss;

        for ( const auto& chr : text )
        {
            switch ( chr )
            {
            case TEXT('&'):     ss << TEXT("&amp;");    break;
            case TEXT('<'):     ss << TEXT("&lt;");     break;
            case TEXT('>'):     ss << TEXT("&gt;");     break;
            case TEXT('"'):     ss << TEXT("&quot;");   break;
            case TEXT('\''):    ss << TEXT("&#39;");    break;
            default:
                ss << chr;
            }
        } // for each character

        return ss.str();
    }

    /**
     * Writes the specified "raw" text to the log file without escaping it. If
     * specified, the file is initially truncated.
     */
    void HtmlLogHandler::_write_raw(cl7::string_view text, bool truncate)
    {
        auto utf8 = cl7::strings::to_utf8( text );

        return _write_raw( utf8, truncate );
    }

    /**
     * Writes the specified "raw" UTF-8 text to the log file without escaping it. If
     * specified, the file is initially truncated.
     */
    void HtmlLogHandler::_write_raw(cl7::u8string_view utf8, bool truncate)
    {
        auto mode = cl7::aofstream::binary | cl7::aofstream::out;
        if ( truncate )
            mode |= cl7::aofstream::trunc;
        else
            mode |= cl7::aofstream::app;

        cl7::aofstream file( TEXT("log.html"), mode );

        if ( !file.is_open() )
            return;

        //if ( truncate )
        //    file.write( reinterpret_cast<const char*>( cl7::strings::to_bytes( u8"", true ).data() ), 3 );
        file.write( reinterpret_cast<const char*>( utf8.data() ), utf8.size() );
    }



} // namespace logging
} // namespace cl7
