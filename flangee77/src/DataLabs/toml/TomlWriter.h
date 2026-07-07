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



    /**
     * Generates a string representing the given `Toml` object based on the specified
     * format.
     */
    static cl7::u8string to_string(const Toml& toml, const Format& format = DEFAULT_FORMAT);

}; // class TomlWriter



} // namespace dl7::toml

#endif // DL7_TOML_TOMLWRITER_H
