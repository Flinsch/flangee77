#include "Document.h"

#include "./XmlWriter.h"



namespace dl7::xml {



    Document::Document(const Document& other)
        : Document()
    {
        *this = other;
    }

    Document& Document::operator=(const Document& other)
    {
        _root_element = Element(other._root_element, nullptr);
        return *this;
    }



    /**
     * Serializes this XML document into a compact single-line string representation.
     */
    cl7::u8string Document::to_string() const
    {
        return XmlWriter::to_string(*this, XmlWriter::DEFAULT_COMPACT_FORMAT);
    }



} // namespace dl7::xml
