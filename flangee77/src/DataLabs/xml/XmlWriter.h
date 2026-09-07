#ifndef DL7_XML_XMLWRITER_H
#define DL7_XML_XMLWRITER_H

#include "./Document.h"
#include "./Format.h"

#include <DataLabs/syntax/Diagnostics.h>



namespace dl7::xml {



/**
 * Serializes XML documents into UTF-8 encoded string representations, keeping
 * whatever it had to complain about along the way.
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



    explicit XmlWriter(const Format& format = DEFAULT_FORMAT);



    /**
     * Generates a string representing the given XML document based on the
     * format this writer was given. Whatever there is to complain about ends
     * up in the diagnostics, which are cleared beforehand.
     */
    cl7::u8string to_string(const Document& document);

    /**
     * Returns the format the strings are generated according to.
     */
    const Format& get_format() const noexcept { return _format; }

    /**
     * Returns the diagnostics of the most recent `to_string` call.
     */
    const syntax::Diagnostics& get_diagnostics() const noexcept { return _diagnostics; }



private:
    Format _format;
    syntax::Diagnostics _diagnostics;

}; // class XmlWriter



} // namespace dl7::xml

#endif // DL7_XML_XMLWRITER_H
