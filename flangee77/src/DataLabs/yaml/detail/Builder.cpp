#include "Builder.h"

#include "./Symbol.h"

#include "../util/Unescaper.h"

#include <DataLabs/json/detail/UnescaperErrorHandler.h>

#include <CoreLabs/text/codec.h>

#include <charconv>
#include <cstdint>
#include <limits>
#include <optional>



namespace dl7::yaml::detail {



namespace {

    /** Stands in for the line break a flow collection spans, which separates tokens like a space does. */
    constexpr cl7::u8string_view LINE_BREAK_SUBSTITUTE = u8" ";

    bool _is_digit(cl7::u8char_t ch) { return ch >= u8'0' && ch <= u8'9'; }
    bool _is_octal_digit(cl7::u8char_t ch) { return ch >= u8'0' && ch <= u8'7'; }
    bool _is_hex_digit(cl7::u8char_t ch) { return _is_digit(ch) || (ch >= u8'a' && ch <= u8'f') || (ch >= u8'A' && ch <= u8'F'); }

    /**
     * Tries to parse an integer scalar according to the core schema, i.e., a
     * decimal, `0o`-prefixed octal, or `0x`-prefixed hexadecimal literal. Returns
     * std::nullopt if the given text is not such a literal (or does not fit).
     */
    std::optional<integer_t> _try_parse_integer(cl7::u8string_view text)
    {
        if (text.empty())
            return std::nullopt;

        size_t i = 0;
        bool negative = false;
        if (text[i] == u8'+' || text[i] == u8'-')
        {
            negative = text[i] == u8'-';
            ++i;
        }

        int base = 10;
        if (text.size() - i > 2 && text[i] == u8'0')
        {
            if (text[i + 1] == u8'o') { base = 8; i += 2; }
            else if (text[i + 1] == u8'x') { base = 16; i += 2; }
        }

        const auto digits = text.substr(i);
        if (digits.empty())
            return std::nullopt;

        for (const auto ch : digits)
        {
            const bool ok = base == 8 ? _is_octal_digit(ch) : (base == 16 ? _is_hex_digit(ch) : _is_digit(ch));
            if (!ok)
                return std::nullopt;
        }

        const std::string_view chars{cl7::text::codec::reinterpret_utf8(digits)};

        std::uint64_t value = 0;
        const auto [ptr, ec] = std::from_chars(chars.data(), chars.data() + chars.size(), value, base);
        if (ec != std::errc{} || ptr != chars.data() + chars.size())
            return std::nullopt;

        constexpr auto limit = static_cast<std::uint64_t>(std::numeric_limits<integer_t>::max());
        if (value > (negative ? limit + 1 : limit))
            return std::nullopt; // Out of range: let it be a float (or a string).

        if (negative)
            return static_cast<integer_t>(~value + 1);
        return static_cast<integer_t>(value);
    }

    /**
     * Tries to parse a floating-point scalar according to the core schema,
     * including `.inf` and `.nan` in their accepted spellings. Returns std::nullopt
     * if the given text is not such a literal.
     */
    std::optional<float_t> _try_parse_float(cl7::u8string_view text)
    {
        if (text.empty())
            return std::nullopt;

        size_t i = 0;
        bool negative = false;
        if (text[i] == u8'+' || text[i] == u8'-')
        {
            negative = text[i] == u8'-';
            ++i;
        }

        const auto rest = text.substr(i);

        if (rest == u8".inf" || rest == u8".Inf" || rest == u8".INF")
            return negative ? -std::numeric_limits<float_t>::infinity() : std::numeric_limits<float_t>::infinity();
        if (rest == u8".nan" || rest == u8".NaN" || rest == u8".NAN")
            return std::numeric_limits<float_t>::quiet_NaN();

        // Reject anything `std::from_chars` would accept but the core schema does
        // not (`inf`, `nan`, hexadecimal significands), i.e., insist on a leading
        // digit or decimal point.
        if (rest.empty() || (!_is_digit(rest[0]) && rest[0] != u8'.'))
            return std::nullopt;

        const std::string_view chars{cl7::text::codec::reinterpret_utf8(text)};

        float_t value = 0.0;
        const auto [ptr, ec] = std::from_chars(chars.data(), chars.data() + chars.size(), value, std::chars_format::general);
        if (ec != std::errc{} || ptr != chars.data() + chars.size())
            return std::nullopt;

        return value;
    }

} // namespace



    Builder::Builder(syntax::Diagnostics* diagnostics)
        : DirectAstBuilder(diagnostics)
    {
    }



    std::unique_ptr<Yaml> Builder::build(syntax::TokenReader& token_reader)
    {
        _token_reader = &token_reader;
        _line = {};

        auto yaml = std::make_unique<Yaml>(_parse_document());

        _token_reader = nullptr;

        return yaml;
    }



    /**
     * Reads the next line into `_line` verbatim, i.e., including its comment and
     * any trailing whitespace. This is what block scalars are made of, everywhere
     * else `_read_line` is what you want.
     */
    void Builder::_read_raw_line()
    {
        auto& token_reader = *_token_reader;

        _line.tokens.clear();
        _line.indent = 0;
        _line.tab_indented = false;
        _line.offset = token_reader.peek_token().source_offset;
        _line.eof = token_reader.is_eof();

        if (_line.eof)
            return;

        while (token_reader.check_symbol_id(WHITESPACE))
        {
            const auto token = token_reader.consume_token();
            _line.tab_indented = _line.tab_indented || token.lexeme == u8"\t";
            _line.indent += token.lexeme.size();
        }

        while (!token_reader.is_eof() && !token_reader.check_symbol_id(NEWLINE))
            _line.tokens.push_back(token_reader.consume_token());

        token_reader.skip_symbol_id(NEWLINE);
    }

    /**
     * Cuts the comment off the buffered line, trims its trailing whitespace, and
     * reports whatever is wrong with what remains. Turns a raw line into an
     * ordinary one.
     */
    void Builder::_strip_comment_and_trim()
    {
        // A `#` only starts a comment where it follows whitespace or begins a line.
        for (size_t i = 0; i < _line.tokens.size(); ++i)
        {
            if (_line.tokens[i].symbol_id != COMMENT_DELIMITER)
                continue;
            if (i > 0 && _line.tokens[i - 1].symbol_id != WHITESPACE)
                continue;

            _line.tokens.resize(i);
            break;
        }

        while (!_line.tokens.empty() && _line.tokens.back().symbol_id == WHITESPACE)
            _line.tokens.pop_back();

        if (_line.tab_indented)
            _error(u8"Tabs must not be used for indentation.", _line.offset);

        // Only complain about what survived: a comment may well contain characters
        // that make no sense as tokens (an apostrophe, most notably).
        for (const auto& token : _line.tokens)
            if (token.symbol_id < 0)
                _error(u8"Unexpected character.", token.source_offset);
    }

    /**
     * Reads the next line into `_line`, blank or not.
     */
    void Builder::_read_line()
    {
        _read_raw_line();
        _strip_comment_and_trim();
    }

    /**
     * Reads lines until one with content is found. Returns false at the end of the
     * source text.
     */
    bool Builder::_advance_to_content_line()
    {
        for (;;)
        {
            _read_line();

            if (_line.eof)
                return false;
            if (!_line.is_blank())
                return true;
        }
    }

    /**
     * Appends the next line to the buffered one, which is how a flow collection
     * gets to span lines. The line break itself becomes a whitespace token, since
     * that is what it separates tokens like. Returns false at the end of the source
     * text, leaving the buffered line as it was.
     */
    bool Builder::_extend_line()
    {
        auto tokens = std::move(_line.tokens);
        const size_t offset = _line.offset;
        const size_t indent = _line.indent;

        _read_line();

        const bool extended = !_line.eof;

        if (extended)
        {
            tokens.push_back({.symbol_id = WHITESPACE, .lexeme = LINE_BREAK_SUBSTITUTE, .source_offset = _line.offset});
            tokens.insert(tokens.end(), _line.tokens.begin(), _line.tokens.end());
        }

        _line.tokens = std::move(tokens);
        _line.offset = offset;
        _line.indent = indent;

        return extended;
    }

    /**
     * Makes sure the buffered line has a token at the given index, appending
     * further lines if necessary. Returns false at the end of the source text.
     */
    bool Builder::_ensure_token(size_t index)
    {
        while (index >= _line.tokens.size())
            if (!_extend_line())
                return false;

        return true;
    }



    Yaml Builder::_parse_document()
    {
        if (!_advance_to_content_line())
            return {};

        size_t index = 0;

        if (_line.tokens[0].symbol_id == DOCUMENT_END)
            return {};

        if (_line.tokens[0].symbol_id == DOCUMENT_START)
        {
            index = _skip_whitespace(1);
            if (index >= _line.tokens.size())
            {
                if (!_advance_to_content_line())
                    return {};
                index = 0;
            }
        }

        auto root = _parse_block_node(index);

        if (!_line.eof)
        {
            if (_line.tokens[0].symbol_id == DOCUMENT_START)
                _warning(u8"Only the first document of a YAML stream is parsed.", _line.offset);
            else if (_line.tokens[0].symbol_id != DOCUMENT_END)
                _error(u8"Unexpected content after the end of the document.", _line.offset);
        }

        return root;
    }

    Yaml Builder::_parse_block_node(size_t index)
    {
        if (_is_sequence_entry(index))
            return Yaml{_parse_block_sequence(index)};

        if (!_starts_non_plain_value(index) && _find_key_separator(index) != NO_INDEX)
            return Yaml{_parse_block_mapping(index)};

        return _parse_value(index);
    }

    mapping_t Builder::_parse_block_mapping(size_t index)
    {
        const size_t indent = _column_of(index);

        mapping_t mapping;

        for (;;)
        {
            const size_t separator = _find_key_separator(index);
            assert(separator != NO_INDEX);

            const size_t key_offset = _line.tokens[index].source_offset;
            auto key = _parse_key(index, separator);

            const size_t value_index = _skip_whitespace(separator + 1);

            Yaml value;
            if (value_index < _line.tokens.size())
                value = _parse_value(value_index);
            else
                value = _parse_nested_value(indent, true);

            if (mapping.contains(key))
                _warning(u8"Duplicate key; the previous value is replaced.", key_offset);
            mapping[std::move(key)] = std::move(value);

            // A document marker ends the mapping without further ado; anything
            // else that is not a key-value pair is an error.
            if (_line.eof || _at_document_marker() || _line.indent < indent || _is_sequence_entry(0))
                break;

            if (_find_key_separator(0) == NO_INDEX)
            {
                _error(u8"Key-value pair expected.", _line.offset);
                break;
            }

            if (_line.indent > indent)
                _warning(u8"Unexpected indentation.", _line.offset);

            index = 0;
        }

        return mapping;
    }

    sequence_t Builder::_parse_block_sequence(size_t index)
    {
        const size_t indent = _column_of(index);

        sequence_t sequence;

        for (;;)
        {
            assert(_is_sequence_entry(index));

            const size_t entry_index = _skip_whitespace(index + 1);

            if (entry_index < _line.tokens.size())
                sequence.push_back(_parse_block_node(entry_index));
            else
                sequence.push_back(_parse_nested_value(indent, false));

            if (_line.eof || _line.indent < indent || !_is_sequence_entry(0))
                break;

            if (_line.indent > indent)
                _warning(u8"Unexpected indentation.", _line.offset);

            index = 0;
        }

        return sequence;
    }

    Yaml Builder::_parse_value(size_t index)
    {
        const auto symbol_id = _line.tokens[index].symbol_id;

        if (symbol_id == BLOCK_SCALAR_HEADER && index + 1 == _line.tokens.size())
            return Yaml{_parse_block_scalar(_line.tokens[index])};

        Yaml value;

        if (symbol_id == OPENING_BRACKET || symbol_id == LEFT_BRACE)
        {
            size_t cursor = index;
            value = _parse_flow_node(cursor);
        }
        else
        {
            value = _parse_scalar(index);
        }

        _advance_to_content_line();

        return value;
    }

    /**
     * Parses the value of a key-value pair or sequence entry whose line ended right
     * after the `:` or `-`, i.e., a value that is either more deeply indented or
     * simply absent (null). A block sequence may share its parent key's
     * indentation, which is what `allow_sequence_at_same_indent` is about.
     */
    Yaml Builder::_parse_nested_value(size_t indent, bool allow_sequence_at_same_indent)
    {
        if (!_advance_to_content_line())
            return {};

        if (_line.indent > indent)
            return _parse_block_node(0);

        if (allow_sequence_at_same_indent && _line.indent == indent && _is_sequence_entry(0))
            return Yaml{_parse_block_sequence(0)};

        return {};
    }

    /**
     * Parses a literal (`|`) or folded (`>`) block scalar, the header of which is
     * passed by value because reading the content lines rewrites the line buffer
     * the header token lives in.
     */
    string_t Builder::_parse_block_scalar(syntax::Token header)
    {
        enum struct Chomping { Clip, Strip, Keep };

        const bool folded = header.lexeme.front() == u8'>';

        size_t explicit_indent = 0;
        auto chomping = Chomping::Clip;

        for (size_t i = 1; i < header.lexeme.size(); ++i)
        {
            const auto ch = header.lexeme[i];
            if (ch >= u8'1' && ch <= u8'9') explicit_indent = static_cast<size_t>(ch - u8'0');
            else if (ch == u8'-') chomping = Chomping::Strip;
            else if (ch == u8'+') chomping = Chomping::Keep;
        }

        const size_t parent_indent = _line.indent;

        // Zero doubles as "not determined yet"; an explicit indicator is never 0.
        size_t content_indent = explicit_indent ? parent_indent + explicit_indent : 0;

        std::vector<string_t> lines;

        for (;;)
        {
            _read_raw_line();

            if (_line.eof)
                break;

            if (_line.is_blank())
            {
                lines.emplace_back();
                continue;
            }

            if (content_indent == 0)
            {
                if (_line.indent <= parent_indent)
                    break;
                content_indent = _line.indent;
            }
            else if (_line.indent < content_indent)
            {
                break;
            }

            lines.push_back(string_t(_line.indent - content_indent, u8' ') + string_t{_join(0, _line.tokens.size())});
        }

        // The line that ended the block scalar is an ordinary one again, and may
        // well have been nothing but a comment.
        _strip_comment_and_trim();
        if (!_line.eof && _line.is_blank())
            _advance_to_content_line();

        size_t trailing_line_breaks = 0;
        while (!lines.empty() && lines.back().empty())
        {
            lines.pop_back();
            ++trailing_line_breaks;
        }

        string_t content;

        for (size_t i = 0; i < lines.size(); ++i)
        {
            if (i > 0)
            {
                // Folding turns a single line break into a space, and n
                // consecutive ones into n-1 line breaks: an empty line does not
                // separate anything by itself, it *is* the separator that the
                // line after it then does not add to.
                if (!folded || lines[i - 1].empty())
                    content += u8'\n';
                else if (!lines[i].empty())
                    content += u8' ';
            }

            content += lines[i];
        }

        if (chomping != Chomping::Strip && !lines.empty())
            content += u8'\n';
        if (chomping == Chomping::Keep)
            content.append(trailing_line_breaks, u8'\n');

        return content;
    }



    Yaml Builder::_parse_flow_node(size_t& index)
    {
        switch (_line.tokens[index].symbol_id)
        {
        case OPENING_BRACKET:
            return Yaml{_parse_flow_sequence(index)};
        case LEFT_BRACE:
            return Yaml{_parse_flow_mapping(index)};
        default:
            return _parse_flow_scalar(index);
        }
    }

    sequence_t Builder::_parse_flow_sequence(size_t& index)
    {
        assert(_line.tokens[index].symbol_id == OPENING_BRACKET);
        ++index;

        sequence_t sequence;

        for (;;)
        {
            _skip_flow_whitespace(index);

            if (!_ensure_token(index))
            {
                _error(u8"Unterminated flow sequence.", _line.offset);
                break;
            }

            if (_line.tokens[index].symbol_id == CLOSING_BRACKET)
            {
                ++index;
                break;
            }

            sequence.push_back(_parse_flow_node(index));

            _skip_flow_whitespace(index);

            if (!_ensure_token(index))
            {
                _error(u8"Unterminated flow sequence.", _line.offset);
                break;
            }

            const auto symbol_id = _line.tokens[index].symbol_id;

            if (symbol_id == COMMA)
            {
                ++index;
                continue;
            }
            if (symbol_id == CLOSING_BRACKET)
            {
                ++index;
                break;
            }

            _error(u8"Comma or closing bracket expected.", _line.tokens[index].source_offset);
            break;
        }

        return sequence;
    }

    mapping_t Builder::_parse_flow_mapping(size_t& index)
    {
        assert(_line.tokens[index].symbol_id == LEFT_BRACE);
        ++index;

        mapping_t mapping;

        for (;;)
        {
            _skip_flow_whitespace(index);

            if (!_ensure_token(index))
            {
                _error(u8"Unterminated flow mapping.", _line.offset);
                break;
            }

            if (_line.tokens[index].symbol_id == RIGHT_BRACE)
            {
                ++index;
                break;
            }

            const size_t key_offset = _line.tokens[index].source_offset;
            auto key = _parse_flow_key(index);

            _skip_flow_whitespace(index);

            // A key may well have no value at all, as in `{a, b}`.
            Yaml value;
            if (_ensure_token(index) && _line.tokens[index].symbol_id == COLON)
            {
                ++index;
                _skip_flow_whitespace(index);

                if (_ensure_token(index))
                {
                    const auto symbol_id = _line.tokens[index].symbol_id;
                    if (symbol_id != COMMA && symbol_id != RIGHT_BRACE)
                        value = _parse_flow_node(index);
                }
            }

            if (mapping.contains(key))
                _warning(u8"Duplicate key; the previous value is replaced.", key_offset);
            mapping[std::move(key)] = std::move(value);

            _skip_flow_whitespace(index);

            if (!_ensure_token(index))
            {
                _error(u8"Unterminated flow mapping.", _line.offset);
                break;
            }

            const auto symbol_id = _line.tokens[index].symbol_id;

            if (symbol_id == COMMA)
            {
                ++index;
                continue;
            }
            if (symbol_id == RIGHT_BRACE)
            {
                ++index;
                break;
            }

            _error(u8"Comma or closing brace expected.", _line.tokens[index].source_offset);
            break;
        }

        return mapping;
    }

    Yaml Builder::_parse_flow_scalar(size_t& index)
    {
        if (_line.tokens[index].symbol_id == QUOTED_STRING_LITERAL)
            return Yaml{_unquote(_line.tokens[index++])};

        const size_t from = index;

        while (_ensure_token(index) && !_ends_flow_scalar(index))
            ++index;

        size_t to = index;
        while (to > from && _line.tokens[to - 1].symbol_id == WHITESPACE)
            --to;

        return _resolve_plain_scalar(_concat(from, to));
    }

    string_t Builder::_parse_flow_key(size_t& index)
    {
        const auto symbol_id = _line.tokens[index].symbol_id;

        if (symbol_id == QUOTED_STRING_LITERAL)
            return _unquote(_line.tokens[index++]);

        if (symbol_id == OPENING_BRACKET || symbol_id == LEFT_BRACE)
        {
            _error(u8"A flow collection is not supported as a key.", _line.tokens[index].source_offset);
            _parse_flow_node(index);
            return {};
        }

        const size_t from = index;

        while (_ensure_token(index) && !_ends_flow_scalar(index))
            ++index;

        size_t to = index;
        while (to > from && _line.tokens[to - 1].symbol_id == WHITESPACE)
            --to;

        return _concat(from, to);
    }

    /**
     * Advances the cursor past whitespace, extending the buffered line if
     * necessary.
     */
    void Builder::_skip_flow_whitespace(size_t& index)
    {
        while (_ensure_token(index) && _line.tokens[index].symbol_id == WHITESPACE)
            ++index;
    }

    /**
     * Returns true if the token at the given index ends a flow scalar, i.e., it is
     * a `,`, a `]`, a `}`, or a `:` acting as a key separator.
     */
    bool Builder::_ends_flow_scalar(size_t index)
    {
        switch (_line.tokens[index].symbol_id)
        {
        case COMMA:
        case CLOSING_BRACKET:
        case RIGHT_BRACE:
            return true;
        case COLON:
            break;
        default:
            return false;
        }

        // Inside a flow collection, a `:` separates a key from its value if what
        // follows is whitespace or the collection's own punctuation.
        if (!_ensure_token(index + 1))
            return true;

        switch (_line.tokens[index + 1].symbol_id)
        {
        case WHITESPACE:
        case COMMA:
        case CLOSING_BRACKET:
        case RIGHT_BRACE:
            return true;
        default:
            return false;
        }
    }



    string_t Builder::_parse_key(size_t index, size_t separator)
    {
        size_t end = separator;
        while (end > index && _line.tokens[end - 1].symbol_id == WHITESPACE)
            --end;

        if (end == index)
        {
            _error(u8"Key expected.", _line.tokens[separator].source_offset);
            return {};
        }

        if (end - index == 1 && _line.tokens[index].symbol_id == QUOTED_STRING_LITERAL)
            return _unquote(_line.tokens[index]);

        return string_t{_join(index, end)};
    }

    Yaml Builder::_parse_scalar(size_t index)
    {
        const size_t end = _line.tokens.size();
        assert(index < end);

        if (end - index == 1 && _line.tokens[index].symbol_id == QUOTED_STRING_LITERAL)
            return Yaml{_unquote(_line.tokens[index])};

        const auto& token = _line.tokens[index];
        const auto text = _join(index, end);

        // Report what is recognizably not a plain scalar but is not supported
        // either, rather than silently turning it into a string.
        if (token.symbol_id == QUESTION_MARK)
            _warning(u8"Explicit keys are not supported; treated as a plain scalar.", token.source_offset);
        else if (text.front() == u8'&' || text.front() == u8'*')
            _warning(u8"Anchors and aliases are not supported; treated as a plain scalar.", token.source_offset);
        else if (text.front() == u8'!')
            _warning(u8"Tags are not supported; treated as a plain scalar.", token.source_offset);

        return _resolve_plain_scalar(text);
    }

    Yaml Builder::_resolve_plain_scalar(cl7::u8string_view text)
    {
        if (text.empty() || text == u8"~" || text == u8"null" || text == u8"Null" || text == u8"NULL")
            return {};

        if (text == u8"true" || text == u8"True" || text == u8"TRUE")
            return Yaml{true};
        if (text == u8"false" || text == u8"False" || text == u8"FALSE")
            return Yaml{false};

        if (const auto integer = _try_parse_integer(text))
            return Yaml{*integer};
        if (const auto number = _try_parse_float(text))
            return Yaml{*number};

        return Yaml{string_t{text}};
    }

    string_t Builder::_unquote(const syntax::Token& token)
    {
        const auto lexeme = token.lexeme;

        assert(lexeme.size() >= 2);
        const auto quote_char = lexeme.front();
        assert(lexeme.starts_with(quote_char));
        assert(lexeme.ends_with(quote_char));

        const auto inner = lexeme.substr(1, lexeme.size() - 2);

        // Single-quoted scalars know exactly one escape: a doubled quote.
        if (quote_char == u8'\'')
        {
            string_t result;
            result.reserve(inner.size());
            for (size_t i = 0; i < inner.size(); ++i)
            {
                result += inner[i];
                if (inner[i] == u8'\'')
                    ++i;
            }
            return result;
        }

        json::detail::UnescaperErrorHandler unescaper_error_handler{get_diagnostics(), &token};
        const util::Unescaper unescaper{&unescaper_error_handler};

        return unescaper.unescape_string(inner);
    }



    bool Builder::_at_document_marker() const
    {
        if (_line.eof || _line.is_blank())
            return false;

        const auto symbol_id = _line.tokens[0].symbol_id;

        return symbol_id == DOCUMENT_START || symbol_id == DOCUMENT_END;
    }

    bool Builder::_is_sequence_entry(size_t index) const
    {
        if (index >= _line.tokens.size())
            return false;
        if (_line.tokens[index].symbol_id != DASH)
            return false;

        return index + 1 >= _line.tokens.size() || _line.tokens[index + 1].symbol_id == WHITESPACE;
    }

    /**
     * Returns true if the token at the given index starts a value that is not a
     * plain scalar, i.e., a flow collection or a block scalar. Only otherwise can a
     * `:` further along the line make the line a block mapping.
     */
    bool Builder::_starts_non_plain_value(size_t index) const
    {
        const auto symbol_id = _line.tokens[index].symbol_id;

        if (symbol_id == OPENING_BRACKET || symbol_id == LEFT_BRACE)
            return true;

        return symbol_id == BLOCK_SCALAR_HEADER && index + 1 == _line.tokens.size();
    }

    /**
     * Returns the index of the token that separates a key from its value, i.e., the
     * first `:` followed by whitespace or the end of the line, or `NO_INDEX`.
     */
    size_t Builder::_find_key_separator(size_t index) const
    {
        for (size_t i = index; i < _line.tokens.size(); ++i)
        {
            if (_line.tokens[i].symbol_id != COLON)
                continue;
            if (i + 1 >= _line.tokens.size() || _line.tokens[i + 1].symbol_id == WHITESPACE)
                return i;
        }

        return NO_INDEX;
    }

    /**
     * Returns the index of the first non-whitespace token at or after the given
     * index (which may be the end of the line).
     */
    size_t Builder::_skip_whitespace(size_t index) const
    {
        while (index < _line.tokens.size() && _line.tokens[index].symbol_id == WHITESPACE)
            ++index;
        return index;
    }

    /**
     * Returns the given token's 0-based column within the current line.
     */
    size_t Builder::_column_of(size_t index) const
    {
        assert(index < _line.tokens.size());
        return _line.tokens[index].source_offset - _line.offset;
    }

    /**
     * Returns the piece of source text the given range of tokens is made of. The
     * tokens of a single line are contiguous, so this is a view into the source
     * rather than a copy — which is also why it must not be used across a line
     * boundary introduced by `_extend_line`; that is what `_concat` is for.
     */
    cl7::u8string_view Builder::_join(size_t from, size_t to) const
    {
        if (from >= to)
            return {};

        const auto& first = _line.tokens[from];
        const auto& last = _line.tokens[to - 1];

        return {first.lexeme.data(), last.source_offset + last.lexeme.size() - first.source_offset};
    }

    /**
     * Returns the given range of tokens' lexemes, concatenated.
     */
    string_t Builder::_concat(size_t from, size_t to) const
    {
        string_t result;

        for (size_t i = from; i < to; ++i)
            result += _line.tokens[i].lexeme;

        return result;
    }



    void Builder::_error(cl7::u8string_view message, size_t source_offset)
    {
        get_diagnostics()->add(syntax::Diagnostic::Severity::Error, message, source_offset);
    }

    void Builder::_warning(cl7::u8string_view message, size_t source_offset)
    {
        get_diagnostics()->add(syntax::Diagnostic::Severity::Warning, message, source_offset);
    }



} // namespace dl7::yaml::detail
