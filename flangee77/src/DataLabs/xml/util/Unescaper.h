#ifndef DL7_XML_UTIL_UNESCAPER_H
#define DL7_XML_UTIL_UNESCAPER_H

#include <CoreLabs/logging/LogContext.h>
#include <CoreLabs/sstream.h>
#include <CoreLabs/string.h>



namespace dl7::xml::util {



/**
 * Unescapes an XML-escaped string literal (excluding any surrounding quotes).
 * Processes XML entities such as `&amp;`, `&lt;`, etc., converting them to their
 * corresponding character equivalents.
 */
class Unescaper
{

public:
    enum struct UnescapeType
    {
        ElementContent,
        AttributeValue,
    };



    struct ErrorHandler
    {
        virtual ~ErrorHandler() = default;

        cl7::logging::LogContext log_context = {};

        ErrorHandler() = default;
        ErrorHandler(cl7::logging::LogContext log_context) : log_context(log_context) {}

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
    };



    explicit Unescaper(const ErrorHandler* error_handler = nullptr) : _error_handler(error_handler) {}



    /**
     * Unescapes an XML-escaped string literal (excluding any surrounding quotes)
     * and returns the result as a new string.
     */
    cl7::u8string unescape_string(cl7::u8string_view xml_string, UnescapeType unescape_type) const;

    /**
     * Unescapes an XML-escaped string literal (excluding any surrounding quotes)
     * and writes the result to the provided output stream. Returns false on invalid
     * escape sequences or invalid UTF-8 (returns true on success).
     */
    bool unescape_string(cl7::u8osstream& out, cl7::u8string_view xml_string, UnescapeType unescape_type) const;



private:
    const ErrorHandler* _error_handler;

}; // class Unescaper



} // namespace dl7::xml::util

#endif // DL7_XML_UTIL_UNESCAPER_H
