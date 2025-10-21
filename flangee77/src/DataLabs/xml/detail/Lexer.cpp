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
        _terminal_symbols.add_literal(WHITESPACE, u8" ");
        _terminal_symbols.add_literal(WHITESPACE, u8"\t");
        _terminal_symbols.add_literal(WHITESPACE, u8"\r\n");
        _terminal_symbols.add_literal(WHITESPACE, u8"\n");
        _terminal_symbols.add_literal(WHITESPACE, u8"\r");

        _terminal_symbols.add_literal(EQUAL_SIGN, u8"=");

        _terminal_symbols.add_literal(PI_START, u8"<?");
        _terminal_symbols.add_literal(PI_END, u8"?>");

        _terminal_symbols.add_literal(START_TAG_START, u8"<");
        _terminal_symbols.add_literal(END_TAG_START, u8"</");
        _terminal_symbols.add_literal(TAG_END, u8">");
        _terminal_symbols.add_literal(EMPTY_TAG_END, u8"/>");

        _terminal_symbols.add_custom(CDATA, syntax::matchers::BlockCommentMatcher{.open = u8"<![CDATA[", .close = u8"]]>"});

        _terminal_symbols.add_custom(COMMENT, syntax::matchers::BlockCommentMatcher{.open = u8"<!--", .close = u8"-->"});

        _terminal_symbols.add_pattern(NAME, R"([A-Za-z_:][A-Za-z0-9_:\-\.])");

        _terminal_symbols.add_custom(STRING, syntax::matchers::DoubleQuotedStringMatcher{});
        _terminal_symbols.add_custom(STRING, syntax::matchers::SingleQuotedStringMatcher{});

        _terminal_symbols.add_pattern(CHAR_DATA, R"([^<&]+)");

        _terminal_symbols.add_pattern(REF, R"(&[^;]+;)");
    }




} // namespace dl7::xml::detail
