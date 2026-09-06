#ifndef DL7_YAML_DETAIL_BUILDER_H
#define DL7_YAML_DETAIL_BUILDER_H
#include <DataLabs/syntax/DirectAstBuilder.h>

#include "../Yaml.h"

#include <vector>



namespace dl7::yaml::detail {



class Builder
    : public syntax::DirectAstBuilder<Yaml>
{

public:
    explicit Builder(syntax::Diagnostics* diagnostics);



    std::unique_ptr<Yaml> build(syntax::TokenReader& token_reader) override;



private:
    /**
     * One logical line of the source text: its indentation plus the tokens that
     * follow, with comments already stripped and trailing whitespace trimmed.
     *
     * YAML's block structure is line-oriented, and deciding what a line even is
     * (a key-value pair? a sequence entry? a plain scalar?) requires looking ahead
     * to the end of it, which a token reader's single token of lookahead does not
     * allow. The builder therefore buffers one line at a time and parses from that
     * buffer rather than straight from the reader.
     */
    struct Line
    {
        /** The source offset of the line's first character. */
        size_t offset = 0;
        /** The width of the line's indentation, in code units. */
        size_t indent = 0;
        /** Whether the line's indentation contains a tab, which is not allowed. */
        bool tab_indented = false;
        /** The line's content tokens (without indentation, comment, and trailing whitespace). */
        std::vector<syntax::Token> tokens;
        /** Whether the end of the source text has been reached, i.e., there is no line. */
        bool eof = true;

        bool is_blank() const noexcept { return tokens.empty(); }
    };

    static constexpr size_t NO_INDEX = static_cast<size_t>(-1);



    /**
     * Reads the next line into `_line` verbatim, i.e., including its comment and
     * any trailing whitespace. This is what block scalars are made of, everywhere
     * else `_read_line` is what you want.
     */
    void _read_raw_line();

    /**
     * Cuts the comment off the buffered line, trims its trailing whitespace, and
     * reports whatever is wrong with what remains. Turns a raw line into an
     * ordinary one.
     */
    void _strip_comment_and_trim();

    /**
     * Reads the next line into `_line`, blank or not.
     */
    void _read_line();

    /**
     * Reads lines until one with content is found. Returns false at the end of the
     * source text.
     */
    bool _advance_to_content_line();

    /**
     * Appends the next line to the buffered one, which is how a flow collection
     * gets to span lines. The line break itself becomes a whitespace token, since
     * that is what it separates tokens like. Returns false at the end of the source
     * text, leaving the buffered line as it was.
     */
    bool _extend_line();

    /**
     * Makes sure the buffered line has a token at the given index, appending
     * further lines if necessary. Returns false at the end of the source text.
     */
    bool _ensure_token(size_t index);



    /**
     * The parsing functions below share one postcondition: when they return, the
     * current line is the next one they did *not* consume (with the end of the
     * source text reached, if there is none).
     */

    Yaml _parse_document();
    Yaml _parse_block_node(size_t index);
    mapping_t _parse_block_mapping(size_t index);
    sequence_t _parse_block_sequence(size_t index);
    Yaml _parse_value(size_t index);

    /**
     * Parses the value of a key-value pair or sequence entry whose line ended right
     * after the `:` or `-`, i.e., a value that is either more deeply indented or
     * simply absent (null). A block sequence may share its parent key's
     * indentation, which is what `allow_sequence_at_same_indent` is about.
     */
    Yaml _parse_nested_value(size_t indent, bool allow_sequence_at_same_indent);

    /**
     * Parses a literal (`|`) or folded (`>`) block scalar, the header of which is
     * passed by value because reading the content lines rewrites the line buffer
     * the header token lives in.
     */
    string_t _parse_block_scalar(syntax::Token header);



    /**
     * The flow parsing functions below take their cursor by reference and leave it
     * on the first token they did not consume. They may extend the buffered line,
     * so a flow collection can span as many lines as it likes.
     */

    Yaml _parse_flow_node(size_t& index);
    sequence_t _parse_flow_sequence(size_t& index);
    mapping_t _parse_flow_mapping(size_t& index);
    Yaml _parse_flow_scalar(size_t& index);
    string_t _parse_flow_key(size_t& index);

    /**
     * Advances the cursor past whitespace, extending the buffered line if
     * necessary.
     */
    void _skip_flow_whitespace(size_t& index);

    /**
     * Returns true if the token at the given index ends a flow scalar, i.e., it is
     * a `,`, a `]`, a `}`, or a `:` acting as a key separator.
     */
    bool _ends_flow_scalar(size_t index);



    string_t _parse_key(size_t index, size_t separator);
    Yaml _parse_scalar(size_t index);
    string_t _unquote(const syntax::Token& token);



    /**
     * Returns true if the current line begins with a `---` or `...` marker, i.e.,
     * the current document ends here.
     */
    bool _at_document_marker() const;

    /**
     * Returns true if the token at the given index starts a block sequence entry,
     * i.e., it is a `-` followed by whitespace or the end of the line.
     */
    bool _is_sequence_entry(size_t index) const;

    /**
     * Returns true if the token at the given index starts a value that is not a
     * plain scalar, i.e., a flow collection or a block scalar. Only otherwise can a
     * `:` further along the line make the line a block mapping.
     */
    bool _starts_non_plain_value(size_t index) const;

    /**
     * Returns the index of the token that separates a key from its value, i.e., the
     * first `:` followed by whitespace or the end of the line, or `NO_INDEX`.
     */
    size_t _find_key_separator(size_t index) const;

    /**
     * Returns the index of the first non-whitespace token at or after the given
     * index (which may be the end of the line).
     */
    size_t _skip_whitespace(size_t index) const;

    /**
     * Returns the given token's 0-based column within the current line.
     */
    size_t _column_of(size_t index) const;

    /**
     * Returns the piece of source text the given range of tokens is made of. The
     * tokens of a single line are contiguous, so this is a view into the source
     * rather than a copy — which is also why it must not be used across a line
     * boundary introduced by `_extend_line`; that is what `_concat` is for.
     */
    cl7::u8string_view _join(size_t from, size_t to) const;

    /**
     * Returns the given range of tokens' lexemes, concatenated.
     */
    string_t _concat(size_t from, size_t to) const;

    void _error(cl7::u8string_view message, size_t source_offset);
    void _warning(cl7::u8string_view message, size_t source_offset);



    syntax::TokenReader* _token_reader = nullptr;
    Line _line;

}; // class Builder



} // namespace dl7::yaml::detail

#endif // DL7_YAML_DETAIL_BUILDER_H
