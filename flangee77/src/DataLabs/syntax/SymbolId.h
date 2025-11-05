#ifndef DL7_SYNTAX_SYMBOLID_H
#define DL7_SYNTAX_SYMBOLID_H

#include <CoreLabs/root.h>



namespace dl7::syntax {



/**
 * 0 is reserved for the "EOF" symbol. Any negative value indicates an unrecognized
 * character or an invalid token. Regular custom IDs can have any positive value.
 */
using SymbolId = signed;



static constexpr SymbolId EOF_SYMBOL_ID = static_cast<SymbolId>(0);
//static constexpr SymbolId INVALID_SYMBOL_ID = static_cast<SymbolId>(-1);



} // namespace dl7::syntax

#endif // DL7_SYNTAX_SYMBOLID_H
