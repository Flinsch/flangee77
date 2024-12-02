#pragma once
#ifndef F77_TESTS_DL7_SYNTAX_H
#define F77_TESTS_DL7_SYNTAX_H

#include <DataLabs/syntax/GenericLexer.h>

#include <CoreLabs/strings.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



TESTLABS_CASE( TEXT("DataLabs:  syntax:  GenericLexer:  tokenize") )
{
    cl7::u8string_view source = u8"#include <iostream>\n\nint main() {\n    std::cout << \"\\\"Hello World\\\"\" << std::endl;\n    return 0;\n}\n";

    dl7::syntax::TerminalSymbolCollection terminal_symbols;
    dl7::syntax::TerminalSymbol::ID symbol_id = 0;
    terminal_symbols.add_pattern( "directive", ++symbol_id, R"(#[a-z]+)" );
    terminal_symbols.add_literal( "left round bracket", ++symbol_id, u8"(" );
    terminal_symbols.add_literal( "left square bracket", ++symbol_id, u8"[" );
    terminal_symbols.add_literal( "left curly bracket", ++symbol_id, u8"{" );
    terminal_symbols.add_literal( "left angle bracket", ++symbol_id, u8"<" );
    terminal_symbols.add_literal( "right round bracket", ++symbol_id, u8")" );
    terminal_symbols.add_literal( "right square bracket", ++symbol_id, u8"]" );
    terminal_symbols.add_literal( "right curly bracket", ++symbol_id, u8"}" );
    terminal_symbols.add_literal( "right angle bracket", ++symbol_id, u8">" );
    terminal_symbols.add_literal( "colon", ++symbol_id, u8":" );
    terminal_symbols.add_literal( "semicolon", ++symbol_id, u8";" );
    terminal_symbols.add_pattern( "flow", ++symbol_id, R"(return)" );
    terminal_symbols.add_pattern( "type", ++symbol_id, R"(int|bool|float)" );
    terminal_symbols.add_pattern( "identifier", ++symbol_id, R"([_a-zA-Z][_a-zA-Z0-9]*)" );
    terminal_symbols.add_pattern( "operator", ++symbol_id, R"([=!~^&|*/%+-])" );
    terminal_symbols.add_pattern( "integer number", ++symbol_id, R"(0|[1-9][0-9]*)" );
    terminal_symbols.add_pattern( "floating point number", ++symbol_id, R"((?:0?|[1-9][0-9]*)\.[0-9]*f?)" );
    terminal_symbols.add_pattern( "string", ++symbol_id, R"(".*")" );

    dl7::syntax::GenericLexer lexer( terminal_symbols, {
        .whitespace_handling = dl7::syntax::Lexer::WhitespaceHandling::Discard,
    } );

    const auto tokens = lexer.tokenize( source );

    TESTLABS_CHECK_EQ( tokens.size(), 28 );
    
}



#endif // F77_TESTS_DL7_SYNTAX_H
