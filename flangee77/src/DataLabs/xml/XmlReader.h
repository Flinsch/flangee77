#ifndef DL7_XML_XMLREADER_H
#define DL7_XML_XMLREADER_H

#include "./Document.h"
#include "./WhitespaceHandling.h"



namespace dl7::xml {



/**
 * A utility class for parsing XML from UTF-8 encoded string representations.
 */
class XmlReader
{

public:
    /**
     * Parses a UTF-8 encoded XML string and returns a `Document` object.
     */
    static Document parse(cl7::u8string_view source, WhitespaceHandling whitespace_handling = WhitespaceHandling::Default);

}; // class XmlReader



} // namespace dl7::xml

#endif // DL7_XML_XMLREADER_H
