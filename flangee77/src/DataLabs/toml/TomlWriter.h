#ifndef DL7_TOML_TOMLWRITER_H
#define DL7_TOML_TOMLWRITER_H

#include "./Toml.h"
#include "./Format.h"

#include <DataLabs/syntax/Diagnostics.h>


namespace dl7::toml {



/**
 * Serializes `Toml` objects into UTF-8 encoded string representations, keeping
 * whatever it had to complain about along the way.
 */
class TomlWriter
{

public:
    static constexpr Format DEFAULT_PRETTY_PRINT_FORMAT = {
        .line_ending = Format::LineEnding::LF,
        .space_around_equals_sign = Format::SpaceAroundEqualsSign::Both,
        .empty_line_at_start_of_file = true,
        .empty_line_before_table_header = true,
        .empty_line_after_table_header = false,
        .add_empty_line = true,
        .escape_unicode = false,
    };
    static constexpr Format DEFAULT_COMPACT_FORMAT = {
        .line_ending = Format::LineEnding::LF,
        .space_around_equals_sign = Format::SpaceAroundEqualsSign::Both,
        .empty_line_at_start_of_file = false,
        .empty_line_before_table_header = false,
        .empty_line_after_table_header = false,
        .add_empty_line = false,
        .escape_unicode = false,
    };
    static constexpr Format DEFAULT_FORMAT = DEFAULT_PRETTY_PRINT_FORMAT;



    explicit TomlWriter(const Format& format = DEFAULT_FORMAT);



    /**
     * Generates a string representing the given `Toml` object based on the
     * format this writer was given. Whatever there is to complain about ends
     * up in the diagnostics, which are cleared beforehand.
     */
    cl7::u8string to_string(const Toml& toml);

    /**
     * Returns the format the strings are generated according to.
     */
    const Format& get_format() const noexcept { return _format; }

    /**
     * Returns the diagnostics of the most recent `to_string` call.
     */
    const syntax::Diagnostics& get_diagnostics() const noexcept { return _diagnostics; }



private:
    Format _format;
    syntax::Diagnostics _diagnostics;

}; // class TomlWriter



} // namespace dl7::toml

#endif // DL7_TOML_TOMLWRITER_H
