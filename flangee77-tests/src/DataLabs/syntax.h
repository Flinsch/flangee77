#pragma once
#ifndef F77_TESTS_DL7_SYNTAX_H
#define F77_TESTS_DL7_SYNTAX_H

#include <DataLabs/syntax/GenericLexer.h>

#include <CoreLabs/strings.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



TESTLABS_CASE( TEXT("DataLabs:  syntax:  GenericLexer:  tokenize") )
{
    cl7::u8string_view source = u8"#include <iostream>\n\nint main() {\n    std::cout << \"\\\"Hello, World!\\\"\" << '\\n';\n    return 0;\n}\n";

    dl7::syntax::TerminalSymbolCollection terminal_symbols;
    terminal_symbols.add_pattern( "directive", 1, R"(#[a-z]+)", u8"#" );
    terminal_symbols.add_literal( "left round bracket", 2, u8"(" );
    terminal_symbols.add_literal( "right round bracket", 3, u8")" );
    terminal_symbols.add_literal( "left square bracket", 4, u8"[" );
    terminal_symbols.add_literal( "right square bracket", 5, u8"]" );
    terminal_symbols.add_literal( "left curly bracket", 6, u8"{" );
    terminal_symbols.add_literal( "right curly bracket", 7, u8"}" );
    terminal_symbols.add_literal( "left angle bracket", 8, u8"<" );
    terminal_symbols.add_literal( "right angle bracket", 9, u8">" );
    terminal_symbols.add_literal( "comma", 10, u8"," );
    terminal_symbols.add_literal( "dot", 11, u8"." );
    terminal_symbols.add_literal( "colon", 12, u8":" );
    terminal_symbols.add_literal( "semicolon", 13, u8";" );
    terminal_symbols.add_pattern( "flow", 14, R"(return)" );
    terminal_symbols.add_pattern( "type", 15, R"(int|bool|float)" );
    terminal_symbols.add_pattern( "identifier", 16, R"([_a-zA-Z][_a-zA-Z0-9]*)" );
    terminal_symbols.add_pattern( "operator", 17, R"([=!~^&|*/%+-])" );
    terminal_symbols.add_pattern( "integer number", 18, R"(0|[1-9][0-9]*)" );
    terminal_symbols.add_pattern( "floating point number", 19, R"((?:0?|[1-9][0-9]*)\.[0-9]*f?)" );
    terminal_symbols.add_pattern( "character", 20, R"('(?:[^'\\]|\\.)')", u8"'" );
    terminal_symbols.add_pattern( "string", 21, R"("[^"\\]*(?:\\.[^"\\]*)*")", u8"\"" );

    dl7::syntax::GenericLexer lexer( terminal_symbols, {
        .whitespace_handling = dl7::syntax::Lexer::WhitespaceHandling::Discard,
    } );

    const auto tokens = lexer.tokenize( source );

    TESTLABS_CHECK_EQ( tokens.size(), 25 );
    TESTLABS_CHECK_EQ( tokens[0].symbol_id, 1 );
    TESTLABS_CHECK_EQ( tokens[1].symbol_id, 8 );
    TESTLABS_CHECK_EQ( tokens[2].symbol_id, 16 );
    TESTLABS_CHECK_EQ( tokens[3].symbol_id, 9 );
    TESTLABS_CHECK_EQ( tokens[4].symbol_id, 15 );
    TESTLABS_CHECK_EQ( tokens[5].symbol_id, 16 );
    TESTLABS_CHECK_EQ( tokens[6].symbol_id, 2 );
    TESTLABS_CHECK_EQ( tokens[7].symbol_id, 3 );
    TESTLABS_CHECK_EQ( tokens[8].symbol_id, 6 );
    TESTLABS_CHECK_EQ( tokens[9].symbol_id, 16 );
    TESTLABS_CHECK_EQ( tokens[10].symbol_id, 12 );
    TESTLABS_CHECK_EQ( tokens[11].symbol_id, 12 );
    TESTLABS_CHECK_EQ( tokens[12].symbol_id, 16 );
    TESTLABS_CHECK_EQ( tokens[13].symbol_id, 8 );
    TESTLABS_CHECK_EQ( tokens[14].symbol_id, 8 );
    TESTLABS_CHECK_EQ( tokens[15].symbol_id, 21 );
    TESTLABS_CHECK_EQ( tokens[16].symbol_id, 8 );
    TESTLABS_CHECK_EQ( tokens[17].symbol_id, 8 );
    TESTLABS_CHECK_EQ( tokens[18].symbol_id, 20 );
    TESTLABS_CHECK_EQ( tokens[19].symbol_id, 13 );
    TESTLABS_CHECK_EQ( tokens[20].symbol_id, 14 );
    TESTLABS_CHECK_EQ( tokens[21].symbol_id, 18 );
    TESTLABS_CHECK_EQ( tokens[22].symbol_id, 13 );
    TESTLABS_CHECK_EQ( tokens[23].symbol_id, 7 );
    TESTLABS_CHECK_EQ( tokens[24].symbol_id, 0 );
}



#endif // F77_TESTS_DL7_SYNTAX_H
