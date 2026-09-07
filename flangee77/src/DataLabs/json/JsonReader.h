#ifndef DL7_JSON_JSONREADER_H
#define DL7_JSON_JSONREADER_H

#include "./Json.h"

#include <DataLabs/syntax/Diagnostics.h>



namespace dl7::json {



/**
 * Parses JSON data from UTF-8 encoded string representations, keeping whatever it
 * had to complain about along the way.
 */
class JsonReader
{

public:
    /**
     * Parses a UTF-8 encoded JSON string and returns a `Json` object. Whatever
     * there is to complain about ends up in the diagnostics, which are cleared
     * beforehand.
     */
    Json parse(cl7::u8string_view source);

    /**
     * Returns the diagnostics of the most recent `parse` call.
     */
    const syntax::Diagnostics& get_diagnostics() const noexcept { return _diagnostics; }



private:
    syntax::Diagnostics _diagnostics;

}; // class JsonReader



} // namespace dl7::json

#endif // DL7_JSON_JSONREADER_H
