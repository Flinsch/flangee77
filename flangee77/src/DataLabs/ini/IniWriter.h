#ifndef DL7_INI_INIWRITER_H
#define DL7_INI_INIWRITER_H

#include "./Ini.h"
#include "./Format.h"
#include "Format.h"
#include "Format.h"



namespace dl7::ini {



/**
 * A utility class for serializing `Ini` objects into UTF-8 encoded string
 * representations.
 */
class IniWriter
{

public:
    static constexpr Format DEFAULT_PRETTY_PRINT_FORMAT = {
        .newline = Format::Newline::LF,
        .section_name_delimiter = Format::SectionNameDelimiter::Dot,
        .key_value_delimiter = Format::KeyValueDelimiter::Equals,
        .space_around_delimiter = Format::SpaceAroundKeyValueDelimiter::Both,
        .allow_single_quotes = false,
        .float_policy = Format::FloatPolicy::ReplaceWithUndefined,
        .empty_line_at_start_of_file = true,
        .empty_line_before_section_header = true,
        .empty_line_after_section_header = false,
    };
    static constexpr Format DEFAULT_COMPACT_FORMAT = {
        .newline = Format::Newline::LF,
        .section_name_delimiter = Format::SectionNameDelimiter::Dot,
        .key_value_delimiter = Format::KeyValueDelimiter::Equals,
        .space_around_delimiter = Format::SpaceAroundKeyValueDelimiter::None,
        .allow_single_quotes = false,
        .float_policy = Format::FloatPolicy::ReplaceWithUndefined,
        .empty_line_at_start_of_file = false,
        .empty_line_before_section_header = false,
        .empty_line_after_section_header = false,
    };
    static constexpr Format DEFAULT_FORMAT = DEFAULT_PRETTY_PRINT_FORMAT;



    /**
     * Generates a string representing the given `Ini` object based on the specified
     * format.
     */
    static cl7::u8string to_string(const Ini& ini, const Format& format = DEFAULT_FORMAT);

}; // class IniWriter



} // namespace dl7::ini

#endif // DL7_INI_INIWRITER_H
