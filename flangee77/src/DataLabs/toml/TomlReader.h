#ifndef DL7_TOML_TOMLREADER_H
#define DL7_TOML_TOMLREADER_H

#include "./Toml.h"

#include <DataLabs/syntax/Diagnostics.h>



namespace dl7::toml {



/**
 * Parses TOML data from UTF-8 encoded string representations, keeping whatever it
 * had to complain about along the way.
 */
class TomlReader
{

public:
    /**
     * Parses a UTF-8 encoded TOML string and returns a `Toml` object. Whatever
     * there is to complain about ends up in the diagnostics, which are cleared
     * beforehand.
     */
    Toml parse(cl7::u8string_view source);

    /**
     * Returns the diagnostics of the most recent `parse` call.
     */
    const syntax::Diagnostics& get_diagnostics() const noexcept { return _diagnostics; }



private:
    syntax::Diagnostics _diagnostics;

}; // class TomlReader



} // namespace dl7::toml

#endif // DL7_TOML_TOMLREADER_H
