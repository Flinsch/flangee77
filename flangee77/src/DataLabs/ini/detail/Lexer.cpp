#include "Lexer.h"

#include "./Symbol.h"
#include "./AnyOtherMatcher.h"

#include <DataLabs/syntax/matchers.h>



namespace dl7::ini::detail {



    Lexer::Lexer(syntax::Diagnostics* diagnostics)
        : GenericLexer(diagnostics, &_terminal_symbols, {WhitespaceHandling::Preserve})
    {
        _init_symbols();
    }



    void Lexer::_init_symbols()
    {
        _terminal_symbols.add_literal( WHITESPACE, u8" " );
        _terminal_symbols.add_literal( WHITESPACE, u8"\t" );
        _terminal_symbols.add_literal( NEWLINE, u8"\r\n" );
        _terminal_symbols.add_literal( NEWLINE, u8"\n" );
        _terminal_symbols.add_literal( NEWLINE, u8"\r" );
        _terminal_symbols.add_literal( COMMENT_DELIMITER, u8";" );
        _terminal_symbols.add_literal( COMMENT_DELIMITER, u8"#" );
        _terminal_symbols.add_literal( SECTION_NAME_DELIMITER, u8"." );
        _terminal_symbols.add_literal( SECTION_NAME_DELIMITER, u8"/" );
        _terminal_symbols.add_literal( KEY_VALUE_DELIMITER, u8"=" );
        _terminal_symbols.add_literal( KEY_VALUE_DELIMITER, u8":" );
        _terminal_symbols.add_literal( OPENING_BRACKET, u8"[" );
        _terminal_symbols.add_literal( CLOSING_BRACKET, u8"]" );
        _terminal_symbols.add_literal( ESCAPED_CHARACTER, u8"\\t" );
        _terminal_symbols.add_literal( ESCAPED_CHARACTER, u8"\\n" );
        _terminal_symbols.add_literal( ESCAPED_CHARACTER, u8"\\r" );
        _terminal_symbols.add_literal( ESCAPED_CHARACTER, u8"\\\"" );
        _terminal_symbols.add_literal( ESCAPED_CHARACTER, u8"\\\\" );
        _terminal_symbols.add_literal( ESCAPED_NEWLINE, u8"\\\r\n" );
        _terminal_symbols.add_literal( ESCAPED_NEWLINE, u8"\\\n" );
        _terminal_symbols.add_literal( ESCAPED_NEWLINE, u8"\\\r" );

        _terminal_symbols.add_custom( INTEGER_LITERAL, syntax::matchers::IntegerLiteralMatcher{} );
        _terminal_symbols.add_custom( DECIMAL_LITERAL, syntax::matchers::FloatingPointLiteralMatcher{} );
        _terminal_symbols.add_custom( QUOTED_STRING_LITERAL, syntax::matchers::DoubleQuotedStringMatcher{} );
        _terminal_symbols.add_custom( QUOTED_STRING_LITERAL, syntax::matchers::SingleQuotedStringMatcher{} );

        _terminal_symbols.add_custom( ANY_OTHER, AnyOtherMatcher{} );
    }




} // namespace dl7::ini::detail
