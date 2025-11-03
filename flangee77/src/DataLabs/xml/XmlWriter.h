#ifndef DL7_XML_XMLWRITER_H
#define DL7_XML_XMLWRITER_H

#include "./Document.h"
#include "./Format.h"



namespace dl7::xml {



/**
 * A utility class for serializing XML documents into UTF-8 encoded string
 * representations.
 */
class XmlWriter
{

public:
    static constexpr Format::CompactOptions DEFAULT_COMPACT_OPTIONS = {
    };
    static constexpr Format::PrettyOptions DEFAULT_PRETTY_OPTIONS = {
        .indentation = 2,
        .line_ending = Format::PrettyOptions::LineEnding::LF,
        .add_empty_line = true,
    };

    static constexpr Format DEFAULT_PRETTY_FORMAT = {
        .style = Format::Style::Pretty,
        .compact_options = DEFAULT_COMPACT_OPTIONS,
        .pretty_options = DEFAULT_PRETTY_OPTIONS,
        .force_escape = false,
        .escape_unicode = false,
        .insert_doctype = false,
    };
    static constexpr Format DEFAULT_COMPACT_FORMAT = {
        .style = Format::Style::Compact,
        .compact_options = DEFAULT_COMPACT_OPTIONS,
        .pretty_options = DEFAULT_PRETTY_OPTIONS,
        .force_escape = false,
        .escape_unicode = false,
        .insert_doctype = false,
    };
    static constexpr Format DEFAULT_FORMAT = DEFAULT_PRETTY_FORMAT;



    /**
     * Generates a string representing the given XML document based on the specified
     * format.
     */
    static cl7::u8string to_string(const Document& document, const Format& format = DEFAULT_FORMAT);

}; // class XmlWriter



} // namespace dl7::xml

#endif // DL7_XML_XMLWRITER_H
