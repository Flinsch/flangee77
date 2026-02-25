#ifndef DL7_JSON_UTIL_UNESCAPER_H
#define DL7_JSON_UTIL_UNESCAPER_H

#include <CoreLabs/logging/LogContext.h>
#include <CoreLabs/sstream.h>
#include <CoreLabs/string.h>



namespace dl7::json::util {



/**
 * Unescapes a JSON-escaped string literal (excluding surrounding quotes).
 * Processes escape sequences such as `\n`, `\t`, `\uXXXX`, etc., converting
 * them to their literal character equivalents. Even if it's not valid JSON,
 * things like `\xXX` or `\v` are still taken into account. Otherwise assumes
 * valid UTF-8 encoding and JSON string syntax.
 */
class Unescaper
{

public:
    struct ErrorHandler
    {
        virtual ~ErrorHandler() = default;

        cl7::logging::LogContext log_context = {};

        ErrorHandler() = default;
        explicit ErrorHandler(cl7::logging::LogContext log_context) : log_context(log_context) {}

        /**
         * Handles an invalid escape sequence by returning a string to be used
         * instead of the invalid escape sequence.
         * Default: Returns the escape sequence unchanged.
         */
        virtual cl7::u8string handle_invalid_escape_sequence(cl7::u8string_view sequence) const { return cl7::u8string{sequence}; }

        /**
         * Will be called when an invalid UTF-8 encoding occurs.
         */
        virtual void on_invalid_utf8_encoding(cl7::u8string_view sequence) const {}

        /**
         * Will be called when an invalid UTF-16 encoding occurs (via an otherwise
         * syntactically correct `\uXXXX` escape sequence).
         */
        virtual void on_invalid_utf16_encoding(cl7::u16string_view sequence) const {}
    };



    explicit Unescaper(const ErrorHandler* error_handler = nullptr) : _error_handler(error_handler) {}



    /**
     * Unescapes a JSON string (excluding surrounding quotes) and returns the result
     * as a new string. Returns an empty string on invalid escape sequences or
     * invalid UTF-8.
     */
    cl7::u8string unescape_string(cl7::u8string_view json_string) const;

    /**
     * Unescapes a JSON string (excluding surrounding quotes) and writes the result
     * to the provided output stream. Returns false on invalid escape sequences or
     * invalid UTF-8 (returns true on success).
     */
    bool unescape_string(cl7::u8osstream& out, cl7::u8string_view json_string) const;



private:
    const ErrorHandler* _error_handler;

}; // class Unescaper



} // namespace dl7::json::util

#endif // DL7_JSON_UTIL_UNESCAPER_H
