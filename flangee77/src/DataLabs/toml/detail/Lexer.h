#ifndef DL7_TOML_DETAIL_LEXER_H
#define DL7_TOML_DETAIL_LEXER_H
#include <DataLabs/syntax/GenericLexer.h>



namespace dl7::toml::detail {



class Lexer
    : public syntax::GenericLexer
{

public:
    explicit Lexer(syntax::Diagnostics* diagnostics);



private:
    void _init_symbols();

    syntax::TerminalSymbolCollection _terminal_symbols;

}; // class Lexer



} // namespace dl7::toml::detail

#endif // DL7_TOML_DETAIL_LEXER_H
