#include "Builder.h"

#include "./Symbol.h"

#include <DataLabs/json/util/Unescaper.h>
#include <DataLabs/json/detail/UnescaperErrorHandler.h>

#include <CoreLabs/text/codec.h>

#include <charconv>
#include <cstdint>
#include <limits>
#include <optional>



namespace dl7::yaml::detail {



namespace {

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
        _line = {};

        return yaml;
    }



    /**
     * Reads the next line into `_line`, blank or not.
     */
    void Builder::_read_line()
    {
        auto& token_reader = *_token_reader;

        _line.tokens.clear();
        _line.indent = 0;
        _line.offset = token_reader.peek_token().source_offset;
        _line.eof = token_reader.is_eof();

        if (_line.eof)
            return;

        bool tab_indented = false;
        while (token_reader.check_symbol_id(WHITESPACE))
        {
            const auto token = token_reader.consume_token();
            tab_indented = tab_indented || token.lexeme == u8"\t";
            _line.indent += token.lexeme.size();
        }

        if (tab_indented)
            _error(u8"Tabs must not be used for indentation.", _line.offset);

        // A `#` only starts a comment where it follows whitespace or begins a line.
        bool after_whitespace = true;

        while (!token_reader.is_eof() && !token_reader.check_symbol_id(NEWLINE))
        {
            if (after_whitespace && token_reader.check_symbol_id(COMMENT_DELIMITER))
            {
                while (!token_reader.is_eof() && !token_reader.check_symbol_id(NEWLINE))
                    token_reader.next_token();
                break;
            }

            const auto token = token_reader.consume_token();

            if (token.symbol_id < 0)
                _error(u8"Unexpected character.", token.source_offset);

            after_whitespace = token.symbol_id == WHITESPACE;
            _line.tokens.push_back(token);
        }

        token_reader.skip_symbol_id(NEWLINE);

        while (!_line.tokens.empty() && _line.tokens.back().symbol_id == WHITESPACE)
            _line.tokens.pop_back();
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

        if (_find_key_separator(index) != NO_INDEX)
            return Yaml{_parse_block_mapping(index)};

        auto scalar = _parse_scalar(index);
        _advance_to_content_line();
        return scalar;
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
            {
                value = _parse_scalar(value_index);
                _advance_to_content_line();
            }
            else
            {
                value = _parse_nested_value(indent, true);
            }

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

        // Report what is recognizably not a plain scalar but is not supported (yet)
        // either, rather than silently turning it into a string.
        switch (token.symbol_id)
        {
        case OPENING_BRACKET:
        case LEFT_BRACE:
            _warning(u8"Flow collections are not supported; treated as a plain scalar.", token.source_offset);
            break;
        case QUESTION_MARK:
            _warning(u8"Explicit keys are not supported; treated as a plain scalar.", token.source_offset);
            break;
        default:
            if (text.front() == u8'|' || text.front() == u8'>')
                _warning(u8"Block scalars are not supported; treated as a plain scalar.", token.source_offset);
            else if (text.front() == u8'&' || text.front() == u8'*')
                _warning(u8"Anchors and aliases are not supported; treated as a plain scalar.", token.source_offset);
            else if (text.front() == u8'!')
                _warning(u8"Tags are not supported; treated as a plain scalar.", token.source_offset);
            break;
        }

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

        // Double-quoted scalars: reuse the JSON unescaper, whose escapes YAML's are
        // a superset of. (The YAML-only ones are still missing.)
        json::detail::UnescaperErrorHandler unescaper_error_handler{get_diagnostics(), &token};
        const json::util::Unescaper unescaper{&unescaper_error_handler};

        return unescaper.unescape_string(inner);
    }



    /**
     * Returns true if the current line begins with a `---` or `...` marker, i.e.,
     * the current document ends here.
     */
    bool Builder::_at_document_marker() const
    {
        if (_line.eof || _line.is_blank())
            return false;

        const auto symbol_id = _line.tokens[0].symbol_id;

        return symbol_id == DOCUMENT_START || symbol_id == DOCUMENT_END;
    }

    /**
     * Returns true if the token at the given index starts a block sequence entry,
     * i.e., it is a `-` followed by whitespace or the end of the line.
     */
    bool Builder::_is_sequence_entry(size_t index) const
    {
        if (index >= _line.tokens.size())
            return false;
        if (_line.tokens[index].symbol_id != DASH)
            return false;

        return index + 1 >= _line.tokens.size() || _line.tokens[index + 1].symbol_id == WHITESPACE;
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
     * tokens of a line are contiguous, so this is a view into the source rather
     * than a copy.
     */
    cl7::u8string_view Builder::_join(size_t from, size_t to) const
    {
        if (from >= to)
            return {};

        const auto& first = _line.tokens[from];
        const auto& last = _line.tokens[to - 1];

        return {first.lexeme.data(), last.source_offset + last.lexeme.size() - first.source_offset};
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
