#ifndef DL7_XML_FORMAT_H
#define DL7_XML_FORMAT_H



namespace dl7::xml {



struct Format
{
    enum struct Style : unsigned
    {
        /** Force single-line output. */
        SingleLine = 0,
        /** Whether to pretty-print the output (multi-line, indented). */
        MultiLine = 1,
    };

    struct SingleLineOptions
    {
    };

    struct MultiLineOptions
    {
        enum struct LineEnding
        {
            /** CRLF (`\r\n`): Windows, DOS */
            CRLF,
            /** LF (`\n`): Unix, Linux, macOS, and "modern" line-ending styles in general */
            LF,
            /** CR (`\r`): Legacy Mac */
            CR,
        };

        /** The number of spaces to use for indentation. If set to 0, tabs are used instead. */
        unsigned indentation = 2;
        /** The line-ending format to use (CRLF, LF, or CR). */
        LineEnding line_ending = LineEnding::LF;
        /** Whether to add an empty line at the end. */
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
    /** Whether to additionally escape HTML-specific special characters (e.g., ``, ``, ``, ``, etc.) in text and attributes beyond standard XML escaping. */
    bool escape_html = false;
}; // struct Format



} // namespace dl7::xml

#endif // DL7_XML_FORMAT_H
