#ifndef DL7_XML_WHITESPACEHANDLING_H
#define DL7_XML_WHITESPACEHANDLING_H



namespace dl7::xml {



/**
 * Specifies how whitespace should be handled when parsing the textual content of
 * elements. Regardless of this setting, textual content that contains only
 * whitespace will always be completely discarded.
 */
enum struct WhitespaceHandling
{
    /** All whitespace is preserved. */
    Preserve,
    /** All occurrences of tab, line feed, and carriage return are replaced with spaces. */
    Replace,
    /** Contiguous sequences of whitespace are collapsed to a single space; leading and trailing whitespace is stripped. */
    Collapse,

    /** The default whitespace handling, tendentially identical to "preserve". */
    Default = Preserve,
};



} // namespace dl7::xml

#endif // DL7_XML_WHITESPACEHANDLING_H
