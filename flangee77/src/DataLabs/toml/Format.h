#ifndef DL7_TOML_FORMAT_H
#define DL7_TOML_FORMAT_H

#include <CoreLabs/string.h>



namespace dl7::toml {



struct Format
{
    enum struct LineEnding
    {
        /** CRLF (`\r\n`): Windows, DOS */
        CRLF,
        /** LF (`\n`): Unix, Linux, macOS, and "modern" line-ending styles in general */
        LF,
        /** CR (`\r`): Legacy Mac (According to the specification, CR is actually not allowed, but it's still supported for the sake of robustness.) */
        CR,
    };

    enum struct SpaceAroundEqualsSign
    {
        /** Don't put spaces around the equals sign of key-value pairs. */
        None,
        /** Put a space before the equals sign of key-value pairs. */
        Before,
        /** Put a space after the equals sign of key-value pairs. */
        After,
        /** Put a space both before and after the equals sign of key-value pairs. */
        Both,
    };

    /** The line-ending format to use (CRLF, LF, or CR; according to the specification, CR is actually not allowed, but it's still supported for the sake of robustness). */
    LineEnding line_ending = LineEnding::LF;
    /** Whether to put spaces before and/or after the equals sign of key-value pairs. */
    SpaceAroundEqualsSign space_around_equals_sign = SpaceAroundEqualsSign::Both;
    /** Whether to put an empty line at start of file. */
    bool empty_line_at_start_of_file = true;
    /** Whether to put an empty line before table headers. */
    bool empty_line_before_table_header = true;
    /** Whether to put an empty line after table headers (before key-value pairs). */
    bool empty_line_after_table_header = false;
    /** Whether to add an empty line at the end. */
    bool add_empty_line = true;
    /** Whether to escape non-ASCII characters with the `\uXXXX` or `\UXXXXXXXX` forms. */
    bool escape_unicode = false;
}; // struct Format



} // namespace dl7::toml

#endif // DL7_TOML_FORMAT_H
