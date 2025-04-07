#include "Lexer.h"

#include "./Symbol.h"
#include "./IdentifierMatcher.h"

#include <DataLabs/syntax/matchers.h>



namespace dl7::json::detail {



    Lexer::Lexer(syntax::Diagnostics* diagnostics)
        : GenericLexer(diagnostics, &_terminal_symbols, {WhitespaceHandling::Discard})
    {
        _terminal_symbols.add_literal( NULL_CONSTANT, u8"null" );
        _terminal_symbols.add_literal( TRUE_CONSTANT, u8"true" );
        _terminal_symbols.add_literal( FALSE_CONSTANT, u8"false" );

        _terminal_symbols.add_custom( INTEGER_LITERAL, syntax::matchers::IntegerLiteralMatcher{} );
        _terminal_symbols.add_custom( DECIMAL_LITERAL, syntax::matchers::FloatingPointLiteralMatcher{} );
        _terminal_symbols.add_custom( STRING_LITERAL, syntax::matchers::DoubleQuotedStringMatcher{} );
        _terminal_symbols.add_custom( STRING_LITERAL, syntax::matchers::SingleQuotedStringMatcher{} );

        _terminal_symbols.add_custom( IDENTIFIER, IdentifierMatcher{} );

        _terminal_symbols.add_literal( LEFT_CURLY_BRACKET, u8"{" );
        _terminal_symbols.add_literal( RIGHT_CURLY_BRACKET, u8"}" );
        _terminal_symbols.add_literal( LEFT_SQUARE_BRACKET, u8"[" );
        _terminal_symbols.add_literal( RIGHT_SQUARE_BRACKET, u8"]" );

        _terminal_symbols.add_literal( COMMA, u8"," );
        _terminal_symbols.add_literal( COLON, u8":" );

        _terminal_symbols.add_custom( LINE_COMMENT, syntax::matchers::LineCommentMatcher{} );
        _terminal_symbols.add_custom( BLOCK_COMMENT, syntax::matchers::BlockCommentMatcher{} );
    }



} // namespace dl7::json::detail
