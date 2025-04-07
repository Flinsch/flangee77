#ifndef DL7_JSON_JSONREADER_H
#define DL7_JSON_JSONREADER_H

#include "./Json.h"



namespace dl7::json {



/**
 * A utility class for parsing JSON data from UTF-8 encoded string representations.
 */
class JsonReader
{

public:
    /**
     * Parses a UTF-8 encoded JSON string and returns a `Json` object.
     */
    static Json parse(cl7::u8string_view source);

}; // class JsonReader



} // namespace dl7::json

#endif // DL7_JSON_JSONREADER_H
