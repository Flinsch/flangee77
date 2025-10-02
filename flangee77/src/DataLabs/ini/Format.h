#ifndef DL7_INI_FORMAT_H
#define DL7_INI_FORMAT_H

#include <CoreLabs/string.h>



namespace dl7::ini {



struct Format
{
    enum struct Newline
    {
        /** CRLF (`\r\n`): Windows, DOS */
        CRLF,
        /** LF (`\n`): Unix, Linux, macOS, and "modern" line-ending styles in general */
        LF,
        /** CR (`\r`): Legacy Mac */
        CR,
    };

    enum struct SectionNameDelimiter : cl7::u8char_t
    {
        /** Dot: '.' */
        Dot = u8'.',
        /** Slash: '/' */
        Slash = u8'/',
    };

    enum struct KeyValueDelimiter : cl7::u8char_t
    {
        /** Equals sign: '=' */
        Equals = u8'=',
        /** Colon: ':' */
        Colon = u8':',
    };

    enum struct SpaceAroundKeyValueDelimiter
    {
        /** Don't put spaces around key-value delimiters. */
        None,
        /** Put a space before key-value delimiters. */
        Before,
        /** Put a space after key-value delimiters. */
        After,
        /** Put a space both before and after key-value delimiters. */
        Both,
    };

    enum struct FloatPolicy
    {
        /** Replace NaN and infinite values with an undefined/empty value. */
        ReplaceWithUndefined,
        /** Replace NaN and infinite values with the numeric value `0.0`. */
        ReplaceWithZero,
        /** Serializes NaN and infinite values as strings, e.g., "NaN", "Infinity", "-Infinity". */
        EncodeAsString,
    };

    /** The line-ending format to use (CRLF, LF, or CR). */
    Newline newline = Newline::LF;
    /** The section name delimiter to use (a dot or a slash). */
    SectionNameDelimiter section_name_delimiter = SectionNameDelimiter::Dot;
    /** The key-value delimiter to use (an equals sign or a colon). */
    KeyValueDelimiter key_value_delimiter = KeyValueDelimiter::Equals;
    /** Whether to put spaces before and/or after key-value delimiters. */
    SpaceAroundKeyValueDelimiter space_around_delimiter = SpaceAroundKeyValueDelimiter::Both;
    /** Whether to allow single-quoted strings. */
    bool allow_single_quotes = false;
    /** Specifies how special floating-point values (NaN, +Infinity, -Infinity) should be handled. */
    FloatPolicy float_policy = FloatPolicy::ReplaceWithUndefined;
    /** Whether to put an empty line at start of file. */
    bool empty_line_at_start_of_file = true;
    /** Whether to put an empty line before section headers. */
    bool empty_line_before_section_header = true;
    /** Whether to put an empty line after section headers (before properties). */
    bool empty_line_after_section_header = false;
}; // struct Format



} // namespace dl7::ini

#endif // DL7_INI_FORMAT_H
