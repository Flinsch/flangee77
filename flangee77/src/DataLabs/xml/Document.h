#ifndef DL7_XML_DOCUMENT_H
#define DL7_XML_DOCUMENT_H

#include "./Element.h"



namespace dl7::xml {



class Document
{

public:
    const Element& root_element() const { return _root_element; }
    Element& root_element() { return _root_element; }



private:
    Element _root_element;

}; // class Document



} // namespace dl7::xml

#endif // DL7_XML_DOCUMENT_H
