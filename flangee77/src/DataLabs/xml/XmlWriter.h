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
    static constexpr Format::SingleLineOptions DEFAULT_SINGLE_LINE_OPTIONS = {
    };
    static constexpr Format::MultiLineOptions DEFAULT_MULTI_LINE_OPTIONS = {
        .indentation = 2,
        .line_ending = Format::MultiLineOptions::LineEnding::LF,
        .add_empty_line = false,
    };

    static constexpr Format DEFAULT_MULTI_LINE_FORMAT = {
        .style = Format::Style::MultiLine,
        .single_line_options = DEFAULT_SINGLE_LINE_OPTIONS,
        .multi_line_options = DEFAULT_MULTI_LINE_OPTIONS,
        .escape_unicode = false,
        .escape_html = false,
    };
    static constexpr Format DEFAULT_SINGLE_LINE_FORMAT = {
        .style = Format::Style::SingleLine,
        .single_line_options = DEFAULT_SINGLE_LINE_OPTIONS,
        .multi_line_options = DEFAULT_MULTI_LINE_OPTIONS,
        .escape_unicode = false,
        .escape_html = false,
    };
    static constexpr Format DEFAULT_FORMAT = DEFAULT_MULTI_LINE_FORMAT;



    /**
     * Generates a string representing the given XML document based on the specified
     * format.
     */
    static cl7::u8string to_string(const Document& document, const Format& format = DEFAULT_FORMAT);

}; // class XmlWriter



} // namespace dl7::xml

#endif // DL7_XML_XMLWRITER_H
