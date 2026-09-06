#ifndef DL7_YAML_FORMAT_H
#define DL7_YAML_FORMAT_H

#include <CoreLabs/root.h>



namespace dl7::yaml {



struct Format
{
    enum struct Style : unsigned
    {
        /** Use the indentation-based block style for mappings and sequences (scalars are single-line anyway). */
        Block = 0,
        /** Use the JSON-like flow style for mappings and sequences, resulting in single-line output. */
        Flow = 1,
    };

    enum struct Indentation : unsigned
    {
        /** Indent using 2 spaces. */
        Spaces2 = 2,
        /** Indent using 4 spaces. */
        Spaces4 = 4,
        // (No tabs: according to the specification, tabs must not be used for indentation.)
    };

    enum struct LineEnding
    {
        /** CRLF (`\r\n`): Windows, DOS */
        CRLF,
        /** LF (`\n`): Unix, Linux, macOS, and "modern" line-ending styles in general */
        LF,
        /** CR (`\r`): Legacy Mac (According to the specification, CR is actually not allowed, but it's still supported for the sake of robustness.) */
        CR,
    };

    enum struct SequenceIndentation
    {
        /** Put block sequence entries at the same indentation level as their parent mapping key. */
        None,
        /** Indent block sequence entries one level deeper than their parent mapping key. */
        OneLevel,
    };

    enum struct ScalarQuoting
    {
        /** Only quote string scalars that would otherwise be ambiguous (i.e., use the plain style wherever possible). */
        WhereNecessary,
        /** Always single-quote string scalars (unless they need to be double-quoted). */
        SingleQuoted,
        /** Always double-quote string scalars. */
        DoubleQuoted,
    };

    enum struct NullRepresentation
    {
        /** Write null values as `null`. */
        Null,
        /** Write null values as `~`. */
        Tilde,
        /** Write null values as empty scalars. */
        Empty,
    };

    enum struct FloatPolicy
    {
        /** Serialize NaN and infinite values as `.nan`, `.inf`, and `-.inf`. */
        Native,
        /** Replace NaN and infinite values with the YAML null value. */
        ReplaceWithNull,
        /** Replace NaN and infinite values with the numeric value `0.0`. */
        ReplaceWithZero,
    };

    /** Block style or flow style? */
    Style style = Style::Block;
    /** The indentation width to use (2 or 4 spaces; according to the specification, tabs must not be used for indentation). */
    Indentation indentation = Indentation::Spaces2;
    /** The line-ending format to use (CRLF, LF, or CR; according to the specification, CR is actually not allowed, but it's still supported for the sake of robustness). */
    LineEnding line_ending = LineEnding::LF;
    /** Whether to indent block sequence entries relative to their parent mapping key. */
    SequenceIndentation sequence_indentation = SequenceIndentation::None;
    /** Whether/how to quote string scalars. */
    ScalarQuoting scalar_quoting = ScalarQuoting::WhereNecessary;
    /** How to represent null values. */
    NullRepresentation null_representation = NullRepresentation::Null;
    /** Specifies how special floating-point values (NaN, +Infinity, -Infinity) should be handled. */
    FloatPolicy float_policy = FloatPolicy::Native;
    /** Whether to write multi-line string scalars as literal block scalars (`|`) instead of double-quoted single-line scalars. */
    bool use_block_scalars = true;
    /** Whether to introduce the document with an explicit `---` marker. */
    bool explicit_document_start = false;
    /** Whether to terminate the document with an explicit `...` marker. */
    bool explicit_document_end = false;
    /** Whether to escape non-ASCII characters as hexadecimal code points (`\xXX`, `\uXXXX`, or `\UXXXXXXXX` format; implies double-quoted scalars). */
    bool escape_unicode = false;
    /** Whether to add an empty line at the end (unless it's a single scalar value anyhow). */
    bool add_empty_line = false;
}; // struct Format



} // namespace dl7::yaml

#endif // DL7_YAML_FORMAT_H
