#ifndef DL7_XML_XMLREADER_H
#define DL7_XML_XMLREADER_H

#include "./Document.h"
#include "./WhitespaceHandling.h"

#include <DataLabs/syntax/Diagnostics.h>



namespace dl7::xml {



/**
 * Parses XML from UTF-8 encoded string representations, keeping whatever it had to
 * complain about along the way.
 */
class XmlReader
{

public:
    explicit XmlReader(WhitespaceHandling whitespace_handling = WhitespaceHandling::Default);



    /**
     * Parses a UTF-8 encoded XML string and returns a `Document` object. Whatever
     * there is to complain about ends up in the diagnostics, which are cleared
     * beforehand.
     */
    Document parse(cl7::u8string_view source);

    /**
     * Returns how whitespace is handled while parsing.
     */
    WhitespaceHandling get_whitespace_handling() const noexcept { return _whitespace_handling; }

    /**
     * Returns the diagnostics of the most recent `parse` call.
     */
    const syntax::Diagnostics& get_diagnostics() const noexcept { return _diagnostics; }



private:
    WhitespaceHandling _whitespace_handling;
    syntax::Diagnostics _diagnostics;

}; // class XmlReader



} // namespace dl7::xml

#endif // DL7_XML_XMLREADER_H
