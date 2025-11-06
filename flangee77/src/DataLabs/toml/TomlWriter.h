#ifndef DL7_TOML_TOMLWRITER_H
#define DL7_TOML_TOMLWRITER_H

#include "./Toml.h"
#include "./Format.h"


namespace dl7::toml {



/**
 * A utility class for serializing `Toml` objects into UTF-8 encoded string
 * representations.
 */
class TomlWriter
{

public:
    static constexpr Format DEFAULT_FORMAT = {};



    /**
     * Generates a string representing the given `Toml` object based on the specified
     * format.
     */
    static cl7::u8string to_string(const Toml& toml, const Format& format = DEFAULT_FORMAT);

}; // class TomlWriter



} // namespace dl7::toml

#endif // DL7_TOML_TOMLWRITER_H
