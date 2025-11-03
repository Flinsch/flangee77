#ifndef DL7_XML_DOCUMENT_H
#define DL7_XML_DOCUMENT_H

#include "./Element.h"
#include "./Text.h"



namespace dl7::xml {



class Document
{

public:
    Document() noexcept = default;

    Document(const Document& other);
    Document& operator=(const Document& other);
    Document(Document&& other) noexcept = default;
    Document& operator=(Document&& other) noexcept = default;

    ~Document() noexcept = default;



    const Element& root_element() const { return _root_element; }
    Element& root_element() { return _root_element; }



    /**
     * Serializes this XML document into a compact single-line string representation.
     */
    cl7::u8string to_string() const;



    bool operator==(const Document& other) const noexcept = default;
    bool operator!=(const Document& other) const noexcept = default;



private:
    Element _root_element;

}; // class Document



} // namespace dl7::xml

#endif // DL7_XML_DOCUMENT_H
