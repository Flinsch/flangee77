#ifndef DL7_XML_DETAIL_SYMBOL_H
#define DL7_XML_DETAIL_SYMBOL_H

#include <DataLabs/syntax/SymbolID.h>



namespace dl7::xml::detail {



enum Symbol : syntax::SymbolID
{
    WHITESPACE = 1,

    EQUAL_SIGN,

    DOCTYPE,

    PI_START,
    PI_END,

    START_TAG_START,
    END_TAG_START,
    TAG_END,
    EMPTY_TAG_END,

    CDATA,

    COMMENT,

    NAME,

    STRING,

    CHAR_DATA,

    REF,
}; // enum Symbol



} // namespace dl7::xml::detail

#endif // DL7_XML_DETAIL_SYMBOL_H
