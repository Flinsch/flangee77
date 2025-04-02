#ifndef DL7_JSON_JSONWRITER_H
#define DL7_JSON_JSONWRITER_H

#include "./Json.h"



namespace dl7::json {



class JsonWriter
{

public:
    struct Format
    {
        enum struct Style : unsigned
        {
            /** Force single-line output, even for structured objects and arrays (primitive values are single-line anyway). */
            SingleLine = 0,
            /** Allow multi-line output for structured objects and arrays (primitive values are single-line anyway). */
            MultiLine = 1,
        };

        struct SingleLineOptions
        {
            /** A compact single-line style omits spaces after colons and commas. */
            bool compact = false;
        };

        struct MultiLineOptions
        {
            enum struct Indentation : unsigned
            {
                /** Indent using tabs. */
                Tabs = 0,
                /** Indent using 2 spaces. */
                Spaces2 = 2,
                /** Indent using 4 spaces. */
                Spaces4 = 4,
            };

            enum struct LineEnding
            {
                /** CRLF (`\r\n`): Windows, DOS */
                CRLF,
                /** LF (`\n`): Unix, Linux, macOS, and "modern" line-ending styles in general */
                LF,
                /** CR (`\r`): Legacy Mac */
                CR,
            };

            /** The indentation style to use (tabs, 2 spaces, or 4 spaces). */
            Indentation indentation = Indentation::Spaces4;
            /** The line-ending format to use (CRLF, LF, or CR). */
            LineEnding line_ending = LineEnding::LF;
            /** Whether to add a trailing comma to the last array/object entry. */
            bool add_trailing_commas = false;
            /** Whether to add an empty line at the end (unless it's a primitive single value anyhow). */
            bool add_empty_line = false;
        };

        /** Single-line or multi-line style? */
        Style style = Style::MultiLine;
        /** The specific options for the single-line style. */
        SingleLineOptions single_line_options;
        /** The specific options for the multi-line style. */
        MultiLineOptions multi_line_options;
        /** Whether to escape non-ASCII characters as hexadecimal UTF-16 code units and/or surrogate pairs (`\uXXXX` format). */
        bool escape_unicode = false;
        /** Whether to allow single-quoted strings. */
        bool allow_single_quotes = false;
        /** Whether to allow unquoted object keys. */
        bool allow_unquoted_keys = false;
    }; // struct Format

    static constexpr Format::SingleLineOptions DEFAULT_SINGLE_LINE_OPTIONS = {.compact=false};
    static constexpr Format::MultiLineOptions DEFAULT_MULTI_LINE_OPTIONS = {.indentation=Format::MultiLineOptions::Indentation::Spaces4, .line_ending=Format::MultiLineOptions::LineEnding::LF, .add_trailing_commas=false, .add_empty_line=false};

    static constexpr Format DEFAULT_MULTI_LINE_FORMAT = {.style=Format::Style::MultiLine, .single_line_options=DEFAULT_SINGLE_LINE_OPTIONS, .multi_line_options={.indentation=Format::MultiLineOptions::Indentation::Spaces4, .line_ending=Format::MultiLineOptions::LineEnding::LF, .add_trailing_commas=false, .add_empty_line=false}, .escape_unicode=false, .allow_single_quotes=false, .allow_unquoted_keys=false};
    static constexpr Format DEFAULT_PRETTY_PRINT_FORMAT = {.style=Format::Style::MultiLine, .single_line_options=DEFAULT_SINGLE_LINE_OPTIONS, .multi_line_options={.indentation=Format::MultiLineOptions::Indentation::Spaces4, .line_ending=Format::MultiLineOptions::LineEnding::LF, .add_trailing_commas=true, .add_empty_line=true}, .escape_unicode=false, .allow_single_quotes=false, .allow_unquoted_keys=false};
    static constexpr Format DEFAULT_SINGLE_LINE_FORMAT = {.style=Format::Style::SingleLine, .single_line_options={.compact=false}, .multi_line_options=DEFAULT_MULTI_LINE_OPTIONS, .escape_unicode=false, .allow_single_quotes=false, .allow_unquoted_keys=false};
    static constexpr Format DEFAULT_COMPACT_FORMAT = {.style=Format::Style::SingleLine, .single_line_options={.compact=true}, .multi_line_options=DEFAULT_MULTI_LINE_OPTIONS, .escape_unicode=false, .allow_single_quotes=false, .allow_unquoted_keys=false};
    static constexpr Format DEFAULT_FORMAT = DEFAULT_PRETTY_PRINT_FORMAT;



    /**
     * Generates a string representing the given JSON object/value based on the
     * specified format.
     */
    static cl7::u8string to_string(const Json& json, const Format& format = DEFAULT_FORMAT);



    /**
     * Checks whether the specified string is a valid identifier for an unquoted
     * object key. Not all conditions of JavaScript's identifier naming rules are
     * considered, as with JSON5, for example. We allow identifiers that begin with
     * a letter (A-Z or a-z), an underscore, or a dollar sign; consist of letters,
     * digits, underscores, and/or dollar signs; and do not correspond to any
     * reserved keyword (as per the ECMAScript specification).
     */
    static bool is_valid_unquoted_key(cl7::u8string_view key);

}; // class JsonWriter



} // namespace dl7::json

#endif // DL7_JSON_JSONWRITER_H
