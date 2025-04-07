#ifndef DL7_JSON_DETAIL_LEXER_H
#define DL7_JSON_DETAIL_LEXER_H
#include <DataLabs/syntax/GenericLexer.h>



namespace dl7::json::detail {



class Lexer
    : public syntax::GenericLexer
{

public:
    explicit Lexer(syntax::Diagnostics* diagnostics);



private:
    syntax::TerminalSymbolCollection _terminal_symbols;

}; // class Lexer



} // namespace dl7::json::detail

#endif // DL7_JSON_DETAIL_LEXER_H
