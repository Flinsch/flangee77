#ifndef DL7_JSON_JSONWRITER_H
#define DL7_JSON_JSONWRITER_H

#include "./Json.h"
#include "./Format.h"



namespace dl7::json {



/**
 * A utility class for serializing `Json` objects into UTF-8 encoded string
 * representations.
 */
class JsonWriter
{

public:
    static constexpr Format::SingleLineOptions DEFAULT_SINGLE_LINE_OPTIONS = {.compact = false};
    static constexpr Format::MultiLineOptions DEFAULT_MULTI_LINE_OPTIONS = {.indentation = Format::MultiLineOptions::Indentation::Spaces4, .line_ending = Format::MultiLineOptions::LineEnding::LF, .add_trailing_commas = false, .add_empty_line = false};

    static constexpr Format DEFAULT_MULTI_LINE_FORMAT = {.style = Format::Style::MultiLine, .single_line_options = DEFAULT_SINGLE_LINE_OPTIONS, .multi_line_options = {.indentation = Format::MultiLineOptions::Indentation::Spaces4, .line_ending = Format::MultiLineOptions::LineEnding::LF, .add_trailing_commas = false, .add_empty_line = false}, .escape_unicode = false, .allow_single_quotes = false, .allow_unquoted_keys = false, .float_policy = Format::FloatPolicy::ReplaceWithNull};
    static constexpr Format DEFAULT_PRETTY_PRINT_FORMAT = {.style = Format::Style::MultiLine, .single_line_options = DEFAULT_SINGLE_LINE_OPTIONS, .multi_line_options = {.indentation = Format::MultiLineOptions::Indentation::Spaces4, .line_ending = Format::MultiLineOptions::LineEnding::LF, .add_trailing_commas = true, .add_empty_line = true}, .escape_unicode = false, .allow_single_quotes = false, .allow_unquoted_keys = false, .float_policy = Format::FloatPolicy::ReplaceWithNull};
    static constexpr Format DEFAULT_SINGLE_LINE_FORMAT = {.style = Format::Style::SingleLine, .single_line_options = {.compact = false}, .multi_line_options = DEFAULT_MULTI_LINE_OPTIONS, .escape_unicode = false, .allow_single_quotes = false, .allow_unquoted_keys = false, .float_policy = Format::FloatPolicy::ReplaceWithNull};
    static constexpr Format DEFAULT_COMPACT_FORMAT = {.style = Format::Style::SingleLine, .single_line_options = {.compact = true}, .multi_line_options = DEFAULT_MULTI_LINE_OPTIONS, .escape_unicode = false, .allow_single_quotes = false, .allow_unquoted_keys = false, .float_policy = Format::FloatPolicy::ReplaceWithNull};
    static constexpr Format DEFAULT_FORMAT = DEFAULT_PRETTY_PRINT_FORMAT;



    /**
     * Generates a string representing the given JSON object/value based on the
     * specified format.
     */
    static cl7::u8string to_string(const Json& json, const Format& format = DEFAULT_FORMAT);

}; // class JsonWriter



} // namespace dl7::json

#endif // DL7_JSON_JSONWRITER_H
