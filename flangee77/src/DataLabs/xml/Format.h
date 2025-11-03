#ifndef DL7_XML_FORMAT_H
#define DL7_XML_FORMAT_H



namespace dl7::xml {



struct Format
{
    enum struct Style : unsigned
    {
        /** Force compact (ideally single-line) output. */
        Compact = 0,
        /** Whether to pretty-print the output (multi-line, indented). */
        Pretty = 1,
    };

    struct CompactOptions
    {
    };

    struct PrettyOptions
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

    /** Compact (ideally single-line) or "pretty" multi-line style? */
    Style style = Style::Pretty;
    /** The specific options for the single-line style. */
    CompactOptions compact_options;
    /** The specific options for the multi-line style. */
    PrettyOptions pretty_options;
    /** Whether to always escape all offending characters (i.e., `&`, `<`, `>`, `"`, `'`), even if not strictly necessary in a given context. */
    bool force_escape = false;
    /** Whether to escape non-ASCII characters as character references (`&#[0-9]+;` or `&#x[0-9a-fA-F]+;` format). */
    bool escape_unicode = false;
    /** Whether to insert a DOCTYPE declaration. */
    bool insert_doctype = false;
}; // struct Format



} // namespace dl7::xml

#endif // DL7_XML_FORMAT_H
