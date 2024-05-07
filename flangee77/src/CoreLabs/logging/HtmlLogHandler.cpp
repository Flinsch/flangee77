#include "HtmlLogHandler.h"

#include <CoreLabs/fstream.h>
#include <CoreLabs/sstream.h>
#include <CoreLabs/strings.h>

#include <chrono>



namespace cl7 {
namespace logging {



    HtmlLogHandler::Block::Block(HtmlLogHandler* owner, cl7::string_view block_class)
        : _owner( owner )
        , _block_class( block_class )
    {
        _owner->_write_raw( TEXT("    <div class=\"") + _block_class + TEXT("\">\n") );
    }

    HtmlLogHandler::Block::~Block()
    {
        _owner->_write_raw( TEXT("    </div>\n") );
        _owner->_write_raw( TEXT("    <div></div><div></div><div></div>\n") );
    }



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
        ss << TEXT("    .container { display: grid; gap: 0.25rem 1rem; grid-template-columns: 1fr auto auto auto; }\n");
        ss << TEXT("    .container > * {  }\n");
        ss << TEXT("    .container > * > .badge { border: 0.125rem solid; border-radius: 0.25rem; font-weight: bold; padding: 0 0.125rem; text-transform: capitalize; }\n");
        ss << TEXT("    .container > .info { color: #085786; }\n");
        ss << TEXT("    .container > .info > .badge { background-color: #085786; border-color: #085786; color: #ffffff; }\n");
        ss << TEXT("    .container > .success { color: #386a1f; }\n");
        ss << TEXT("    .container > .success > .badge { background-color: #386a1f; border-color: #386a1f; color: #ffffff; }\n");
        ss << TEXT("    .container > .warning { color: #8f4f00; }\n");
        ss << TEXT("    .container > .warning > .badge { background-color: #8f4f00; border-color: #8f4f00; color: #ffffff; }\n");
        ss << TEXT("    .container > .error { color: #b3261e; }\n");
        ss << TEXT("    .container > .error > .badge { background-color: #b3261e; border-color: #b3261e; color: #ffffff; }\n");
        ss << TEXT("    .container > .caption { font-weight: bold; }\n");
        ss << TEXT("    .container > .section { font-style: italic; }\n");
        ss << TEXT("    .container > .list { display: grid; gap: 0.125rem 1rem; grid-template-columns: auto 1fr; margin-left: 1rem; }\n");
        ss << TEXT("    .container > .list > .item {  }\n");
        ss << TEXT("    .container > .list > .item.key {  }\n");
        ss << TEXT("    .container > .list > .item.value { font-weight: bold; }\n");
        ss << TEXT("    .container > .list > .item.pass { color: #386a1f; }\n");
        ss << TEXT("    .container > .list > .item.fail { color: #b3261e; }\n");
        ss << TEXT("    .container > .code { font-family: monospace; margin-left: 1rem; }\n");
        ss << TEXT("    .container > .meta {  }\n");
        ss << TEXT("    .container > .comment { font-family: monospace; }\n");
        ss << TEXT("    .container > .other {  }\n");
        ss << TEXT("    .container > .file-path { font-family: sans-serif; font-size: small; font-style: oblique; }\n");
        ss << TEXT("    .container > .line-number { font-family: sans-serif; font-size: small; font-style: oblique; text-align: right; }\n");
        ss << TEXT("    .container > .function-name { font-family: monospace; font-size: small; font-style: oblique; }\n");
        ss << TEXT("  </style>\n");
        ss << TEXT("</head>\n");
        ss << TEXT("<body>\n");
        ss << TEXT("  <h1>") << title << TEXT("</h1>\n");
        ss << TEXT("  <p>Session start: ") << ldt << TEXT("</p>\n");
        ss << TEXT("  <div class=\"container\">\n");

        _write_raw( ss.str(), true );
    }

    /**
     * Destructor.
     */
    HtmlLogHandler::~HtmlLogHandler()
    {
        _block_ptr.reset();

        const std::time_t t = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
        std::tm tm;
        ::localtime_s( &tm, &t );
        const auto ldt = std::put_time( &tm, TEXT("%F %T %z") );

        cl7::osstream ss;
        ss << TEXT("  </div>\n");
        ss << TEXT("  <p>Session end: ") << ldt << TEXT("</p>\n");
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
        cl7::string_view entry_class, block_class, badge_class;
        switch ( log_entry.type )
        {
        case cl7::logging::LogType::Info:       entry_class = TEXT("info");         badge_class = TEXT("info");     break;
        case cl7::logging::LogType::Success:    entry_class = TEXT("success");      badge_class = TEXT("success");  break;
        case cl7::logging::LogType::Warning:    entry_class = TEXT("warning");      badge_class = TEXT("warning");  break;
        case cl7::logging::LogType::Error:      entry_class = TEXT("error");        badge_class = TEXT("error");    break;
        case cl7::logging::LogType::Caption:    entry_class = TEXT("caption");      break;
        case cl7::logging::LogType::Section:    entry_class = TEXT("section");      break;
        case cl7::logging::LogType::Item:       entry_class = TEXT("item");         block_class = TEXT("list");     break;
        case cl7::logging::LogType::ItemPass:   entry_class = TEXT("item pass");    block_class = TEXT("list");     break;
        case cl7::logging::LogType::ItemFail:   entry_class = TEXT("item fail");    block_class = TEXT("list");     break;
        case cl7::logging::LogType::Code:       entry_class = TEXT("code");         block_class = TEXT("code");     break;
        case cl7::logging::LogType::Meta:       entry_class = TEXT("meta");         break;
        case cl7::logging::LogType::Comment:    entry_class = TEXT("comment");      break;
        case cl7::logging::LogType::Other:      entry_class = TEXT("other");        break;
        }

        cl7::osstream ss;

        if ( block_class.empty() )
        {
            _block_ptr.reset();

            ss << TEXT("    <div class=\"") << entry_class << TEXT("\">\n");
            ss << TEXT("      ");
            if ( !badge_class.empty() )
                ss << TEXT("<span class=\"badge\">") << badge_class << TEXT("</span> ");
            ss << _escape( log_entry.message ) << TEXT("\n");
            ss << TEXT("    </div>\n");
        }
        else // => block
        {
            if ( _block_ptr && _block_ptr->_block_class != block_class )
                _block_ptr.reset();
            if ( !_block_ptr )
                _block_ptr = std::make_unique<Block>( this, block_class );

            if ( log_entry.type == cl7::logging::LogType::Code )
            {
                ss << TEXT("      ") << _escape( log_entry.message ) << TEXT("<br>\n");
            }
            else // => list
            {
                cl7::string_view key, value;
                size_t p = log_entry.message.find( TEXT('\t') );
                if ( p == log_entry.message.npos )
                    key = log_entry.message;
                else
                    key = log_entry.message.substr( 0, p ),
                    value = log_entry.message.substr( p + 1 );

                ss << TEXT("      <div class=\"") << entry_class << TEXT(" key\">\n");
                ss << TEXT("        ") << _escape( key ) << TEXT("\n");
                ss << TEXT("      </div>\n");
                ss << TEXT("      <div class=\"") << entry_class << TEXT(" value\">\n");
                if ( !value.empty() )
                    ss << TEXT("        ") << _escape( value ) << TEXT("\n");
                ss << TEXT("      </div>\n");
            }
        }

        _write_raw( ss.str() );
        if ( block_class.empty() )
            _write_source_location( log_entry.file_path, log_entry.line_number, log_entry.function_name );
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
            case TEXT('"'):     ss << TEXT("&quot;");   break;
            case TEXT('&'):     ss << TEXT("&amp;");    break;
            case TEXT('\''):    ss << TEXT("&#39;");    break;
            case TEXT('<'):     ss << TEXT("&lt;");     break;
            case TEXT('>'):     ss << TEXT("&gt;");     break;
            case TEXT('\u00a0'):ss << TEXT("&nbsp;");   break;
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

    /**
     * Writes certain information about the source code location from which the log
     * entry came to the log file.
     */
    void HtmlLogHandler::_write_source_location(const cl7::char_type* file_path, unsigned line_number, const cl7::char_type* function_name)
    {
#pragma message( "Consider using std::source_location as a better alternative to __FILE__, __LINE__, etc." )

        cl7::osstream ss;
        ss << TEXT("    <div class=\"file-path\">\n");
        if ( file_path )
        {
            constexpr std::string_view file_path_view{ __FILE__ };
            constexpr size_t file_path_skip = file_path_view.find( "flangee77" );
            static_assert( file_path_skip != file_path_view.npos );

            ss << TEXT("      ") << _escape( file_path + file_path_skip ) << TEXT("\n");
        }
        ss << TEXT("    </div>\n");
        ss << TEXT("    <div class=\"line-number\">\n");
        if ( file_path )
        {
            ss << TEXT("      ") << ( line_number ) << TEXT("\n");
        }
        ss << TEXT("    </div>\n");
        ss << TEXT("    <div class=\"function-name\">\n");
        if ( function_name )
        {
            const cl7::string_view function_name_view{ function_name };
            const size_t function_name_skip = function_name_view.rfind( TEXT("::") );

            ss << TEXT("      ") << _escape( function_name_skip != function_name_view.npos ? function_name_view.substr( function_name_skip + 2 ) : function_name ) << TEXT("\n");
        }
        ss << TEXT("    </div>\n");

        _write_raw( ss.str() );
    }



} // namespace logging
} // namespace cl7
