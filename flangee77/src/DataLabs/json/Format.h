#ifndef DL7_JSON_FORMAT_H
#define DL7_JSON_FORMAT_H



namespace dl7::json {



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



} // namespace dl7::json

#endif // DL7_JSON_FORMAT_H
