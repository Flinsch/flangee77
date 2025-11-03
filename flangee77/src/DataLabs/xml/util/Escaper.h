#ifndef DL7_XML_UTIL_ESCAPER_H
#define DL7_XML_UTIL_ESCAPER_H

#include <CoreLabs/logging/LogContext.h>
#include <CoreLabs/sstream.h>
#include <CoreLabs/string.h>



namespace dl7::xml::util {



/**
 * Escapes a raw string for inclusion in XML content or attribute values. Replaces
 * offending characters that could be mistakenly interpreted as markup with
 * corresponding XML entities, quotation marks (optionally, depending on any
 * specified quotation character), and (optionally) non-ASCII characters.
 */
class Escaper
{

public:
    enum struct EscapeType : cl7::u8char_t
    {
        ElementContent = 0,
        DoubleQuotedAttributeValue = u8'"',
        SingleQuotedAttributeValue = u8'\'',
    };

    struct Options
    {
        /** Whether to always escape all offending characters (i.e., `&`, `<`, `>`, `"`, `'`), even if not strictly necessary in a given context. */
        bool force_escape = false;
        /** Whether to escape non-ASCII characters as numeric character references (`&#nnnn;` or `&#xhhhh;` format). */
        bool escape_unicode = false;
    };

    static constexpr Options DEFAULT_OPTIONS = {.force_escape = false, .escape_unicode = false};



    explicit Escaper(cl7::logging::LogContext log_context = {}) : _log_context(log_context) {}



    /**
     * Escapes a raw string for inclusion in XML content or attribute values and
     * returns the result as a new string.
     */
    cl7::u8string escape_string(cl7::u8string_view raw_string, EscapeType escape_type, const Options& options = DEFAULT_OPTIONS) const;

    /**
     * Escapes a raw string for inclusion in XML content or attribute values and
     * writes the result to the provided output stream.
     */
    void escape_string(cl7::u8osstream& out, cl7::u8string_view raw_string, EscapeType escape_type, const Options& options = DEFAULT_OPTIONS) const;



private:
    cl7::logging::LogContext _log_context;

}; // class Escaper



} // namespace dl7::xml::util

#endif // DL7_XML_UTIL_ESCAPER_H
