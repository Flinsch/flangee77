#ifndef DL7_YAML_DETAIL_SYMBOL_H
#define DL7_YAML_DETAIL_SYMBOL_H

#include <DataLabs/syntax/SymbolId.h>



namespace dl7::yaml::detail {



/**
 * The terminal symbols of the YAML lexer.
 *
 * YAML is both indentation-sensitive and context-sensitive, whereas the underlying
 * lexer is neither. The symbols below are therefore deliberately "dumb": they are
 * recognized wherever they occur, and it is up to the builder to decide whether a
 * `COLON` actually separates a key from its value (only if followed by whitespace
 * or a line break), whether a `COMMENT_DELIMITER` really starts a comment (only if
 * preceded by whitespace or the beginning of a line), etc. Plain scalars are
 * reassembled by the builder from the token run up to the end of the line (or up to
 * the next flow indicator inside flow collections).
 */
enum Symbol : syntax::SymbolId
{
    WHITESPACE = 1,
    NEWLINE,
    COMMENT_DELIMITER,
    DOCUMENT_START,
    DOCUMENT_END,
    DASH,
    COLON,
    QUESTION_MARK,
    COMMA,
    OPENING_BRACKET,
    CLOSING_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,

    QUOTED_STRING_LITERAL,
    BLOCK_SCALAR_HEADER,

    ANY_OTHER,
}; // enum Symbol



} // namespace dl7::yaml::detail

#endif // DL7_YAML_DETAIL_SYMBOL_H
