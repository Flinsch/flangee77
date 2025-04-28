#ifndef DL7_JSON_UTIL_ESCAPER_H
#define DL7_JSON_UTIL_ESCAPER_H

#include <CoreLabs/logging/LogContext.h>
#include <CoreLabs/sstream.h>
#include <CoreLabs/string.h>



namespace dl7::json::util {



/**
 * Escapes a raw string for inclusion in a JSON string literal. Replaces control
 * characters, quotation marks (depending on the specified quotation character),
 * backslashes, and (optionally) non-ASCII characters. It does not add
 * surrounding quotation marks to the result.
 */
class Escaper
{

public:
    enum struct QuoteChar : cl7::u8char_t
    {
        DoubleQuote = u8'"',
        SingleQuote = u8'\'',
    };

    struct Options
    {
        /** Specifies which quotation character is assumed to surround the JSON string literal. This setting is important for correctly escaping the chosen quote character within the string content. */
        QuoteChar quote_char = QuoteChar::DoubleQuote;
        /** Whether to escape non-ASCII characters as hexadecimal UTF-16 code units and/or surrogate pairs (`\uXXXX` format). */
        bool escape_unicode = false;
    };

    static constexpr Options DEFAULT_OPTIONS = {.quote_char = QuoteChar::DoubleQuote, .escape_unicode = false};



    explicit Escaper(cl7::logging::LogContext log_context = {}) : _log_context(log_context) {}



    /**
     * Escapes a raw string for inclusion in a JSON string literal and returns the
     * result as a new string.
     */
    cl7::u8string escape_string(cl7::u8string_view raw_string, const Options& options = DEFAULT_OPTIONS) const;

    /**
     * Escapes a raw string for inclusion in a JSON string literal and writes the
     * result to the provided output stream.
     */
    void escape_string(cl7::u8osstream& out, cl7::u8string_view raw_string, const Options& options = DEFAULT_OPTIONS) const;



private:
    void _do_escape_utf8_encoded(cl7::u8osstream& out, cl7::u8string_view raw_string, const Options& options) const;
    void _do_escape_utf16_format(cl7::u8osstream& out, cl7::u8string_view raw_string, const Options& options) const;
    static bool _try_escape_basic_helper(cl7::u8osstream& out, unsigned codepoint_value, const Options& options);



    cl7::logging::LogContext _log_context;

}; // class Escaper



} // namespace dl7::json::util

#endif // DL7_JSON_UTIL_ESCAPER_H
