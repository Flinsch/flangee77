#include "Lexer.h"

#include "./Symbol.h"

#include <DataLabs/syntax/matchers.h>



namespace dl7::xml::detail {



    Lexer::Lexer(syntax::Diagnostics* diagnostics)
        : GenericLexer(diagnostics, &_terminal_symbols, {WhitespaceHandling::Discard})
    {
        _init_symbols();
    }



    void Lexer::_init_symbols()
    {

    }




} // namespace dl7::xml::detail
