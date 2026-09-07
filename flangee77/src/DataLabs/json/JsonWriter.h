#ifndef DL7_JSON_JSONWRITER_H
#define DL7_JSON_JSONWRITER_H

#include "./Json.h"
#include "./Format.h"

#include <DataLabs/syntax/Diagnostics.h>



namespace dl7::json {



/**
 * Serializes `Json` objects into UTF-8 encoded string representations, keeping
 * whatever it had to complain about along the way.
 */
class JsonWriter
{

public:
    static constexpr Format::SingleLineOptions DEFAULT_SINGLE_LINE_OPTIONS = {
        .compact = false,
    };
    static constexpr Format::MultiLineOptions DEFAULT_MULTI_LINE_OPTIONS = {
        .indentation = Format::MultiLineOptions::Indentation::Spaces4,
        .line_ending = Format::MultiLineOptions::LineEnding::LF,
        .add_trailing_commas = false,
        .add_empty_line = false,
    };

    static constexpr Format DEFAULT_MULTI_LINE_FORMAT = {
        .style = Format::Style::MultiLine,
        .single_line_options = DEFAULT_SINGLE_LINE_OPTIONS,
        .multi_line_options = {
            .indentation = Format::MultiLineOptions::Indentation::Spaces4,
            .line_ending = Format::MultiLineOptions::LineEnding::LF,
            .add_trailing_commas = false,
            .add_empty_line = false,
        },
        .escape_unicode = false,
        .allow_single_quotes = false,
        .allow_unquoted_keys = false,
        .float_policy = Format::FloatPolicy::ReplaceWithNull,
    };
    static constexpr Format DEFAULT_PRETTY_PRINT_FORMAT = {
        .style = Format::Style::MultiLine,
        .single_line_options = DEFAULT_SINGLE_LINE_OPTIONS,
        .multi_line_options = {
            .indentation = Format::MultiLineOptions::Indentation::Spaces4,
            .line_ending = Format::MultiLineOptions::LineEnding::LF,
            .add_trailing_commas = true,
            .add_empty_line = true,
        },
        .escape_unicode = false,
        .allow_single_quotes = false,
        .allow_unquoted_keys = false,
        .float_policy = Format::FloatPolicy::ReplaceWithNull,
    };
    static constexpr Format DEFAULT_SINGLE_LINE_FORMAT = {
        .style = Format::Style::SingleLine,
        .single_line_options = {
            .compact = false,
        },
        .multi_line_options = DEFAULT_MULTI_LINE_OPTIONS,
        .escape_unicode = false,
        .allow_single_quotes = false,
        .allow_unquoted_keys = false,
        .float_policy = Format::FloatPolicy::ReplaceWithNull,
    };
    static constexpr Format DEFAULT_COMPACT_FORMAT = {
        .style = Format::Style::SingleLine,
        .single_line_options = {
            .compact = true,
        },
        .multi_line_options = DEFAULT_MULTI_LINE_OPTIONS,
        .escape_unicode = false,
        .allow_single_quotes = false,
        .allow_unquoted_keys = false,
        .float_policy = Format::FloatPolicy::ReplaceWithNull,
    };
    static constexpr Format DEFAULT_FORMAT = DEFAULT_PRETTY_PRINT_FORMAT;



    explicit JsonWriter(const Format& format = DEFAULT_FORMAT);



    /**
     * Generates a string representing the given JSON object/value based on the
     * format this writer was given. Whatever there is to complain about ends
     * up in the diagnostics, which are cleared beforehand.
     */
    cl7::u8string to_string(const Json& json);

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

}; // class JsonWriter



} // namespace dl7::json

#endif // DL7_JSON_JSONWRITER_H
