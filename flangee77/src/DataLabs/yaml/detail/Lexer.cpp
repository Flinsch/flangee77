#include "Lexer.h"

#include "./Symbol.h"
#include "./AnyOtherMatcher.h"
#include "./BlockScalarHeaderMatcher.h"
#include "./SingleQuotedStringMatcher.h"

#include <DataLabs/syntax/matchers.h>



namespace dl7::yaml::detail {



    Lexer::Lexer(syntax::Diagnostics* diagnostics)
        : GenericLexer(diagnostics, &_terminal_symbols, {WhitespaceHandling::Preserve})
    {
        _init_symbols();
    }



    void Lexer::_init_symbols()
    {
        _terminal_symbols.add_literal(WHITESPACE, u8" ");
        _terminal_symbols.add_literal(WHITESPACE, u8"\t");
        _terminal_symbols.add_literal(NEWLINE, u8"\r\n");
        _terminal_symbols.add_literal(NEWLINE, u8"\n");
        _terminal_symbols.add_literal(NEWLINE, u8"\r");
        _terminal_symbols.add_literal(COMMENT_DELIMITER, u8"#");
        _terminal_symbols.add_literal(DOCUMENT_START, u8"---");
        _terminal_symbols.add_literal(DOCUMENT_END, u8"...");
        _terminal_symbols.add_literal(DASH, u8"-");
        _terminal_symbols.add_literal(COLON, u8":");
        _terminal_symbols.add_literal(QUESTION_MARK, u8"?");
        _terminal_symbols.add_literal(COMMA, u8",");
        _terminal_symbols.add_literal(OPENING_BRACKET, u8"[");
        _terminal_symbols.add_literal(CLOSING_BRACKET, u8"]");
        _terminal_symbols.add_literal(LEFT_BRACE, u8"{");
        _terminal_symbols.add_literal(RIGHT_BRACE, u8"}");

        _terminal_symbols.add_custom(QUOTED_STRING_LITERAL, SingleQuotedStringMatcher{});
        _terminal_symbols.add_custom(QUOTED_STRING_LITERAL, syntax::matchers::DoubleQuotedStringMatcher{});

        _terminal_symbols.add_custom(BLOCK_SCALAR_HEADER, BlockScalarHeaderMatcher{});

        _terminal_symbols.add_custom(ANY_OTHER, AnyOtherMatcher{});
    }




} // namespace dl7::yaml::detail
