#ifndef F77_TESTS_DL7_XML_H
#define F77_TESTS_DL7_XML_H

#include <DataLabs/xml/Document.h>
#include <DataLabs/xml/XmlReader.h>
#include <DataLabs/xml/XmlWriter.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



namespace tl7::internals {
    inline
    cl7::u8string to_string(const dl7::xml::Document& document) { return dl7::xml::XmlWriter::to_string( document ); }
}





#endif // F77_TESTS_DL7_XML_H
