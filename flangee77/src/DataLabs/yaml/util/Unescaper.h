#ifndef DL7_YAML_UTIL_UNESCAPER_H
#define DL7_YAML_UTIL_UNESCAPER_H

#include <DataLabs/json/util/Unescaper.h>

#include <CoreLabs/string.h>



namespace dl7::yaml::util {



/**
 * Unescapes a double-quoted YAML scalar (excluding the surrounding quotes).
 *
 * YAML's escape sequences are a superset of JSON's, so instead of duplicating the
 * decoding machinery, this translates the escapes JSON does not know into ones it
 * does and hands the result to `json::util::Unescaper`: the YAML-only escapes all
 * stand for one fixed character (`\0`, `\a`, `\e`, `\N`, `\_`, `\L`, `\P`, and an
 * escaped space), and `\UXXXXXXXX` becomes the `\uXXXX` spelling (a surrogate pair,
 * where necessary) that JSON uses for the same code point.
 *
 * Note that a single-quoted YAML scalar has no escapes at all beyond the doubled
 * quote, and is therefore none of this class's business.
 */
class Unescaper
{

public:
    using ErrorHandler = json::util::Unescaper::ErrorHandler;



    explicit Unescaper(const ErrorHandler* error_handler = nullptr) : _error_handler(error_handler) {}



    /**
     * Unescapes a double-quoted YAML scalar (excluding the surrounding quotes) and
     * returns the result as a new string.
     */
    cl7::u8string unescape_string(cl7::u8string_view yaml_string) const;



private:
    /**
     * Rewrites the escape sequences YAML has beyond JSON's into their JSON
     * equivalents, leaving everything else (including `\\`) untouched.
     */
    static cl7::u8string _translate_escapes(cl7::u8string_view yaml_string);

    const ErrorHandler* _error_handler;

}; // class Unescaper



} // namespace dl7::yaml::util

#endif // DL7_YAML_UTIL_UNESCAPER_H
