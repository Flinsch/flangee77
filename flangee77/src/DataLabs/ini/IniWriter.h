#ifndef DL7_INI_INIWRITER_H
#define DL7_INI_INIWRITER_H

#include "./Ini.h"
#include "./Format.h"

#include <DataLabs/syntax/Diagnostics.h>



namespace dl7::ini {



/**
 * Serializes `Ini` objects into UTF-8 encoded string representations, keeping
 * whatever it had to complain about along the way.
 */
class IniWriter
{

public:
    static constexpr Format DEFAULT_PRETTY_PRINT_FORMAT = {
        .line_ending = Format::LineEnding::LF,
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
        .line_ending = Format::LineEnding::LF,
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



    explicit IniWriter(const Format& format = DEFAULT_FORMAT);



    /**
     * Generates a string representing the given `Ini` object based on the
     * format this writer was given. Whatever there is to complain about ends
     * up in the diagnostics, which are cleared beforehand.
     */
    cl7::u8string to_string(const Ini& ini);

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

}; // class IniWriter



} // namespace dl7::ini

#endif // DL7_INI_INIWRITER_H
