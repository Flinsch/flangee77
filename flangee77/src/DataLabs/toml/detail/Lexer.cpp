#include "Lexer.h"

#include "./Symbol.h"



namespace dl7::toml::detail {



    Lexer::Lexer(syntax::Diagnostics* diagnostics)
        : GenericLexer(diagnostics, &_terminal_symbols, {WhitespaceHandling::Preserve})
    {
        _init_symbols();
    }



    void Lexer::_init_symbols()
    {

    }




} // namespace dl7::toml::detail
