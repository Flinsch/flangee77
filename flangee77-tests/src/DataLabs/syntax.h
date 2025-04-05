#ifndef F77_TESTS_DL7_SYNTAX_H
#define F77_TESTS_DL7_SYNTAX_H

#include <DataLabs/syntax/matchers.h>
#include <DataLabs/syntax/GenericLexer.h>
#include <DataLabs/syntax/BufferedTokenReader.h>
#include <DataLabs/syntax/LexingTokenReader.h>
#include <DataLabs/syntax/Vocabulary.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



TESTLABS_CASE( u8"DataLabs:  syntax:  SingleQuotedStringMatcher" )
{
    struct Entry
    {
        cl7::u8string_view source;
        size_t expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", 0 },
        { u8"'", 0 },
        { u8"''", 2 },
        { u8"'Hello World'", 13 },
        { u8"'Hello World' ", 13 },
        { u8" 'Hello World'", 0 },
        { u8"'Hello World", 0 },
        { u8"Hello World'", 0 },
        { u8"'Don't Call Me White'", 5 },
        { u8"'Don\'t Call Me White'", 5 },
        { u8"'Don\\'t Call Me White'", 22 },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.source )
    {
        TESTLABS_CHECK_EQ( dl7::syntax::matchers::SingleQuotedStringMatcher()( entry.source ), entry.expected );
    }
}

TESTLABS_CASE( u8"DataLabs:  syntax:  DoubleQuotedStringMatcher" )
{
    struct Entry
    {
        cl7::u8string_view source;
        size_t expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", 0 },
        { u8"\"", 0 },
        { u8"\"\"", 2 },
        { u8"\"Hello World\"", 13 },
        { u8"\"Hello World\" ", 13 },
        { u8" \"Hello World\"", 0 },
        { u8"\"Hello World", 0 },
        { u8"Hello World\"", 0 },
        { u8"\"\"Hello World\"\"", 2 },
        { u8"\"\\\"Hello World\\\"\"", 17 },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.source )
    {
        TESTLABS_CHECK_EQ( dl7::syntax::matchers::DoubleQuotedStringMatcher()( entry.source ), entry.expected );
    }
}

TESTLABS_CASE( u8"DataLabs:  syntax:  IntegerLiteralMatcher" )
{
    struct Entry
    {
        dl7::syntax::matchers::SignPolicy sign_policy;
        cl7::u8string_view source;
        size_t expected;
    } entry;

    const std::vector<Entry> container {
        { dl7::syntax::matchers::SignPolicy::Unsigned, u8"", 0 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, u8"0", 1 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, u8"1", 1 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, u8"1 ", 1 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, u8" 1", 0 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, u8"01", 0 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, u8"10", 2 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, u8"42", 2 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, u8"7", 1 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, u8"-7", 0 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, u8"+7", 0 },
        { dl7::syntax::matchers::SignPolicy::AllowMinus, u8"7", 1 },
        { dl7::syntax::matchers::SignPolicy::AllowMinus, u8"-7", 2 },
        { dl7::syntax::matchers::SignPolicy::AllowMinus, u8"+7", 0 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, u8"7", 1 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, u8"-7", 2 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, u8"+7", 2 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, u8"-07", 0 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, u8"+07", 0 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, u8"1.0e2", 1 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, u8"1e2", 1 },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.source )
    {
        TESTLABS_CHECK_EQ( dl7::syntax::matchers::IntegerLiteralMatcher( entry.sign_policy )( entry.source ), entry.expected );
    }
}

TESTLABS_CASE( u8"DataLabs:  syntax:  FloatingPointLiteralMatcher" )
{
    struct Entry
    {
        dl7::syntax::matchers::SignPolicy sign_policy;
        dl7::syntax::matchers::FloatNotation notation;
        cl7::u8string_view source;
        size_t expected;
    } entry;

    const std::vector<Entry> container {
        { dl7::syntax::matchers::SignPolicy::Unsigned, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"", 0 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"0", 1 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"1", 1 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"1 ", 1 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8" 1", 0 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"01", 0 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"10", 2 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"42", 2 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"7", 1 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"-7", 0 },
        { dl7::syntax::matchers::SignPolicy::Unsigned, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"+7", 0 },
        { dl7::syntax::matchers::SignPolicy::AllowMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"7", 1 },
        { dl7::syntax::matchers::SignPolicy::AllowMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"-7", 2 },
        { dl7::syntax::matchers::SignPolicy::AllowMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"+7", 0 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"7", 1 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"-7", 2 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"+7", 2 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"-07", 0 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"+07", 0 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"1.0", 3 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"0.1", 3 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"1.", 2 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8".1", 2 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"+1.", 3 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"+.1", 3 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"10.10", 5 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"01.10", 0 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"1.e2", 2 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"-1.e+2", 3 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"+1.e-2", 3 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8".1e2", 2 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"-.1e+2", 3 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8"+.1e-2", 3 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::AllowScientific, u8"1.e2", 4 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::AllowScientific, u8"-1.e+2", 6 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::AllowScientific, u8"+1.e-2", 6 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::AllowScientific, u8".1e2", 4 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::AllowScientific, u8"-.1e+2", 6 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::AllowScientific, u8"+.1e-2", 6 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::AllowScientific, u8".1E2", 4 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::AllowScientific, u8".1E ", 0 },
        { dl7::syntax::matchers::SignPolicy::AllowPlusMinus, dl7::syntax::matchers::FloatNotation::DecimalOnly, u8".1E ", 2 },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.source )
    {
        TESTLABS_CHECK_EQ( dl7::syntax::matchers::FloatingPointLiteralMatcher( entry.sign_policy, entry.notation )( entry.source ), entry.expected );
    }
}

TESTLABS_CASE( u8"DataLabs:  syntax:  IdentifierMatcher" )
{
    struct Entry
    {
        cl7::u8string_view source;
        size_t expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", 0 },
        { u8" ", 0 },
        { u8"_", 1 },
        { u8"Hello World", 5 },
        { u8" Hello World", 0 },
        { u8"0zero", 0 },
        { u8"zero0", 5 },
        { u8"one-", 3 },
        { u8"$one", 0 },
        { u8"abcxyzABCXYZ0123456789_", 23 },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.source )
    {
        TESTLABS_CHECK_EQ( dl7::syntax::matchers::IdentifierMatcher()( entry.source ), entry.expected );
    }
}

TESTLABS_CASE( u8"DataLabs:  syntax:  LineCommentMatcher" )
{
    struct Entry
    {
        cl7::u8string_view source;
        size_t expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", 0 },
        { u8" ", 0 },
        { u8"// ...", 6 },
        { u8"// ...\n...", 6 },
        { u8" // ...", 0 },
        { u8"// /* ...\n... */", 9 },
        { u8"// // ...\n...", 9 },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.source )
    {
        TESTLABS_CHECK_EQ( dl7::syntax::matchers::LineCommentMatcher( u8"//" )( entry.source ), entry.expected );
    }
}

TESTLABS_CASE( u8"DataLabs:  syntax:  BlockCommentMatcher" )
{
    struct Entry
    {
        bool allow_nesting;
        cl7::u8string_view source;
        size_t expected;
    } entry;

    const std::vector<Entry> container {
        { false, u8"", 0 },
        { false, u8" ", 0 },
        { false, u8"/* ... */", 9 },
        { false, u8"/* ...\n... */", 13 },
        { false, u8" /* ... */", 0 },
        { false, u8"/* ...\n", 7 },
        { false, u8"/* comment */ code", 13 },
        { false, u8"code /* comment */ code", 0 },
        { false, u8"/* code /* comment */ code */ code", 21 },
        { false, u8"/* code // comment\ncode */ code", 26 },
        { false, u8"/* end of file", 14 },
        { false, u8"/* end of /* comment */ file", 23 },
        { true, u8"/* code /* comment */ code */ code", 29 },
        { true, u8"/* code // comment\ncode */ code", 26 },
        { true, u8"/* end of file", 14 },
        { true, u8"/* end of /* comment */ file", 0 },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.source )
    {
        TESTLABS_CHECK_EQ( dl7::syntax::matchers::BlockCommentMatcher( u8"/*", u8"*/", entry.allow_nesting )( entry.source ), entry.expected );
    }
}



TESTLABS_CASE( u8"DataLabs:  syntax:  GenericLexer / TokenReader" )
{
    constexpr cl7::u8string_view source = u8"#include <iostream>\n\nint main() {\n    std::cout << \"\\\"Hello, World!\\\"\" << '\\n';\n    return 0;\n}\n";

    dl7::syntax::TerminalSymbolCollection terminal_symbols;
    terminal_symbols.add_pattern( /*u8"directive",*/ 1, u8"#", R"(#[a-z]+)" );
    terminal_symbols.add_literal( /*u8"left round bracket",*/ 2, u8"(" );
    terminal_symbols.add_literal( /*u8"right round bracket",*/ 3, u8")" );
    terminal_symbols.add_literal( /*u8"left square bracket",*/ 4, u8"[" );
    terminal_symbols.add_literal( /*u8"right square bracket",*/ 5, u8"]" );
    terminal_symbols.add_literal( /*u8"left curly bracket",*/ 6, u8"{" );
    terminal_symbols.add_literal( /*u8"right curly bracket",*/ 7, u8"}" );
    terminal_symbols.add_literal( /*u8"left angle bracket",*/ 8, u8"<" );
    terminal_symbols.add_literal( /*u8"right angle bracket",*/ 9, u8">" );
    terminal_symbols.add_literal( /*u8"comma",*/ 10, u8"," );
    terminal_symbols.add_literal( /*u8"dot",*/ 11, u8"." );
    terminal_symbols.add_literal( /*u8"colon",*/ 12, u8":" );
    terminal_symbols.add_literal( /*u8"semicolon",*/ 13, u8";" );
    terminal_symbols.add_pattern( /*u8"flow",*/ 14, R"(return)" );
    terminal_symbols.add_pattern( /*u8"type",*/ 15, R"(int|bool|float)" );
    terminal_symbols.add_pattern( /*u8"identifier",*/ 16, R"([_a-zA-Z][_a-zA-Z0-9]*)" );
    terminal_symbols.add_pattern( /*u8"operator",*/ 17, R"([=!~^&|*/%+-])" );
    terminal_symbols.add_pattern( /*u8"integer number",*/ 18, R"(0|[1-9][0-9]*)" );
    terminal_symbols.add_pattern( /*u8"floating point number",*/ 19, R"((?:0?|[1-9][0-9]*)\.[0-9]*f?)" );
    terminal_symbols.add_pattern( /*u8"character",*/ 20, u8"'", R"('(?:[^'\\]|\\.)')" );
    terminal_symbols.add_pattern( /*u8"string",*/ 21, u8"\"", R"("[^"\\]*(?:\\.[^"\\]*)*")" );

    dl7::syntax::Diagnostics diagnostics;
    dl7::syntax::GenericLexer lexer( &diagnostics, &terminal_symbols, {
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

    lexer.init( source ); // reset lexer

    dl7::syntax::BufferedTokenReader<> buffered_reader( tokens );
    dl7::syntax::LexingTokenReader lexing_reader( &lexer );
    for ( const auto& token : tokens )
    {
        TESTLABS_CHECK_EQ( buffered_reader.peek_symbol_id(), token.symbol_id );
        TESTLABS_CHECK_EQ( lexing_reader.peek_symbol_id(), token.symbol_id );
        buffered_reader.next_token();
        lexing_reader.next_token();
    }
    TESTLABS_CHECK_EQ( buffered_reader.peek_symbol_id(), dl7::syntax::EOF_SYMBOL_ID );
    TESTLABS_CHECK_EQ( lexing_reader.peek_symbol_id(), dl7::syntax::EOF_SYMBOL_ID );
    TESTLABS_CHECK( buffered_reader.is_eof() );
    TESTLABS_CHECK( lexing_reader.is_eof() );
}



TESTLABS_CASE( u8"DataLabs:  syntax:  Vocabulary" )
{
    dl7::syntax::Vocabulary vocabulary;
    vocabulary.add_symbol( 1, u8"directive" );
    vocabulary.add_symbol( 2, u8"left round bracket" );
    vocabulary.add_symbol( 3, u8"right round bracket" );
    vocabulary.add_symbol( 4, u8"left square bracket" );
    vocabulary.add_symbol( 5, u8"right square bracket" );
    vocabulary.add_symbol( 6, u8"left curly bracket" );
    vocabulary.add_symbol( 7, u8"right curly bracket" );
    vocabulary.add_symbol( 8, u8"left angle bracket" );
    vocabulary.add_symbol( 9, u8"right angle bracket" );
    vocabulary.add_symbol( 10, u8"comma" );
    vocabulary.add_symbol( 11, u8"dot" );
    vocabulary.add_symbol( 12, u8"colon" );
    vocabulary.add_symbol( 13, u8"semicolon" );
    vocabulary.add_symbol( 14, u8"flow" );
    vocabulary.add_symbol( 15, u8"type" );
    vocabulary.add_symbol( 16, u8"identifier" );
    vocabulary.add_symbol( 17, u8"operator" );
    vocabulary.add_symbol( 18, u8"integer number" );
    vocabulary.add_symbol( 19, u8"floating point number" );
    vocabulary.add_symbol( 20, u8"character" );
    vocabulary.add_symbol( 21, u8"string" );

    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 1 ), u8"directive" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 2 ), u8"left round bracket" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 3 ), u8"right round bracket" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 4 ), u8"left square bracket" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 5 ), u8"right square bracket" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 6 ), u8"left curly bracket" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 7 ), u8"right curly bracket" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 8 ), u8"left angle bracket" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 9 ), u8"right angle bracket" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 10 ), u8"comma" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 11 ), u8"dot" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 12 ), u8"colon" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 13 ), u8"semicolon" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 14 ), u8"flow" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 15 ), u8"type" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 16 ), u8"identifier" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 17 ), u8"operator" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 18 ), u8"integer number" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 19 ), u8"floating point number" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 20 ), u8"character" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 21 ), u8"string" );

    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 0 ), u8"" );
    TESTLABS_CHECK_EQ( vocabulary.get_symbol_name( 22 ), u8"" );
}



#endif // F77_TESTS_DL7_SYNTAX_H
