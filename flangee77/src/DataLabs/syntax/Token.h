#pragma once
#ifndef DL7_SYNTAX_TOKEN_H
#define DL7_SYNTAX_TOKEN_H

#include "./TerminalSymbol.h"
#include "./SourceLocation.h"

#include <CoreLabs/string.h>



namespace dl7 {
namespace syntax {



struct Token
{

    /** The terminal symbol ID of the token. */
    TerminalSymbol::ID symbol_id;

    /** The piece of the source text that matches the terminal symbol definition. */
    cl7::u8string_view lexeme;

    /** The location of the token in the source text. */
    SourceLocation source_location;

}; // struct Token



} // namespace syntax
} // namespace dl7

#endif // DL7_SYNTAX_TOKEN_H
