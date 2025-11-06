#ifndef DL7_TOML_TOMLREADER_H
#define DL7_TOML_TOMLREADER_H

#include "./Toml.h"



namespace dl7::toml {



/**
 * A utility class for parsing TOML data from UTF-8 encoded string representations.
 */
class TomlReader
{

public:
    /**
     * Parses a UTF-8 encoded TOML string and returns a `Toml` object.
     */
    static Toml parse(cl7::u8string_view source);

}; // class TomlReader



} // namespace dl7::toml

#endif // DL7_TOML_TOMLREADER_H
