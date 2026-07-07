#include "Builder.h"

#include "./Symbol.h"

#include <DataLabs/syntax/matchers.h>
#include <DataLabs/json/util/Unescaper.h>
#include <DataLabs/json/detail/UnescaperErrorHandler.h>

#include <CoreLabs/text/codec.h>
#include <CoreLabs/sstream.h>

#include <charconv>
#include <chrono>
#include <cstdint>
#include <limits>
#include <optional>
#include <sstream>



namespace dl7::toml::detail {



    static cl7::u8string _strip_underscores(cl7::u8string_view string)
    {
        cl7::u8string result;
        result.reserve(string.size());
        for (const auto ch : string)
            if (ch != u8'_')
                result += ch;
        return result;
    }

    /**
     * Tries to parse an integer literal with a base prefix (`0x`, `0o`, or `0b`,
     * with optional underscores between digits). Returns std::nullopt if the given
     * string is not such a prefixed integer.
     */
    static std::optional<integer_t> _try_parse_based_integer(cl7::u8string_view raw)
    {
        if (raw.size() < 3 || raw[0] != u8'0')
            return std::nullopt;

        int base = 0;
        switch (raw[1])
        {
        case u8'x': case u8'X': base = 16; break;
        case u8'o': case u8'O': base = 8; break;
        case u8'b': case u8'B': base = 2; break;
        default: return std::nullopt;
        }

        const auto digits = _strip_underscores(raw.substr(2));
        if (digits.empty())
            return std::nullopt;

        const std::string chars{cl7::text::codec::reinterpret_utf8(digits)};

        std::uint64_t value = 0;
        const auto [ptr, ec] = std::from_chars(chars.data(), chars.data() + chars.size(), value, base);
        if (ec != std::errc{} || ptr != chars.data() + chars.size())
            return std::nullopt;

        return static_cast<integer_t>(value);
    }



    Builder::Builder(syntax::Diagnostics* diagnostics)
        : DirectAstBuilder(diagnostics)
    {
    }



    std::unique_ptr<Toml> Builder::build(syntax::TokenReader& token_reader)
    {
        auto toml = std::make_unique<Toml>();

        Context ctx;
        ctx.root = &toml->root();
        ctx.current = &toml->root();

        _parse_toml(ctx, token_reader);

        return toml;
    }



    void Builder::_parse_toml(Context& ctx, syntax::TokenReader& token_reader)
    {
        while (!token_reader.is_eof())
        {
            if (!token_reader.is_valid())
            {
                _error(u8"Key-value pair, table header, or comment expected.", token_reader.peek_token());
                break;
            }

            _parse_line(ctx, token_reader);
        }
    }



    void Builder::_parse_line(Context& ctx, syntax::TokenReader& token_reader)
    {
        while (token_reader.check_symbol_id(WHITESPACE) || token_reader.check_symbol_id(NEWLINE))
            token_reader.next_token();

        if (token_reader.is_eof())
            return;

        switch (token_reader.peek_symbol_id())
        {
        case COMMENT_DELIMITER:
            _parse_comment(token_reader);
            break;
        case OPENING_BRACKET:
            _parse_table_header(ctx, token_reader);
            break;
        default:
            _parse_assignment(*ctx.current, token_reader);
            break;
        }

        _skip_inline_whitespace(token_reader);

        if (token_reader.check_symbol_id(COMMENT_DELIMITER))
            _parse_comment(token_reader);

        if (!token_reader.is_eof() && !token_reader.skip_symbol_id(NEWLINE))
        {
            _error(u8"Line break expected.", token_reader.peek_token());
            token_reader.skip_past_symbol_id(NEWLINE);
        }
    }



    void Builder::_parse_comment(syntax::TokenReader& token_reader)
    {
        if (!token_reader.skip_symbol_id(COMMENT_DELIMITER))
        {
            _error(u8"'#' expected.", token_reader.peek_token());
            return;
        }

        token_reader.skip_to_symbol_id(NEWLINE);
    }

    void Builder::_parse_table_header(Context& ctx, syntax::TokenReader& token_reader)
    {
        if (!token_reader.skip_symbol_id(OPENING_BRACKET))
        {
            _error(u8"'[' expected.", token_reader.peek_token());
            return;
        }

        // A second opening bracket denotes an array-of-tables header (`[[name]]`).
        const bool array_of_tables = token_reader.skip_symbol_id(OPENING_BRACKET);

        _skip_inline_whitespace(token_reader);

        table_t* table = ctx.root;

        syntax::Token key_token = token_reader.peek_token();
        string_t key;

        for (;;)
        {
            key_token = token_reader.peek_token();
            key = _parse_key(token_reader);
            if (key.empty())
            {
                _error(u8"Table name expected.", token_reader.peek_token());
                token_reader.skip_to_symbol_id(NEWLINE);
                return;
            }

            _skip_inline_whitespace(token_reader);

            if (!token_reader.skip_symbol_id(DOT))
                break;

            _skip_inline_whitespace(token_reader);

            table = &_descend(*table, key, key_token);
        }

        ctx.current = array_of_tables
            ? &_descend_array(*table, key, key_token)
            : &_descend(*table, key, key_token);

        if (!token_reader.skip_symbol_id(CLOSING_BRACKET))
            _error(u8"']' expected.", token_reader.peek_token());

        if (array_of_tables && !token_reader.skip_symbol_id(CLOSING_BRACKET))
            _error(u8"']' expected.", token_reader.peek_token());
    }

    void Builder::_parse_assignment(table_t& base, syntax::TokenReader& token_reader)
    {
        table_t* table = &base;

        auto key_token = token_reader.peek_token();
        auto key = _parse_key(token_reader);
        if (key.empty())
        {
            _error(u8"Key expected.", token_reader.peek_token());
            token_reader.skip_to_symbol_id(NEWLINE);
            return;
        }

        _skip_inline_whitespace(token_reader);

        while (token_reader.skip_symbol_id(DOT))
        {
            _skip_inline_whitespace(token_reader);

            table = &_descend(*table, key, key_token);

            key_token = token_reader.peek_token();
            key = _parse_key(token_reader);
            if (key.empty())
            {
                _error(u8"Key expected.", token_reader.peek_token());
                token_reader.skip_to_symbol_id(NEWLINE);
                return;
            }

            _skip_inline_whitespace(token_reader);
        }

        if (!token_reader.skip_symbol_id(EQUALS))
        {
            _error(u8"'=' expected.", token_reader.peek_token());
            token_reader.skip_to_symbol_id(NEWLINE);
            return;
        }

        _skip_inline_whitespace(token_reader);

        auto value = _parse_value(token_reader);

        auto it = table->emplace(key, Value{}).first;
        it->second = std::move(value);
    }



    string_t Builder::_parse_key(syntax::TokenReader& token_reader)
    {
        if (token_reader.check_symbol_id(QUOTED_STRING_LITERAL))
            return _parse_quoted_string(token_reader);
        if (token_reader.check_symbol_id(ANY_OTHER))
            return string_t{token_reader.consume_token().lexeme};

        return {};
    }

    Value Builder::_parse_value(syntax::TokenReader& token_reader)
    {
        _skip_inline_whitespace(token_reader);

        switch (token_reader.peek_symbol_id())
        {
        case QUOTED_STRING_LITERAL:
            return Value{_parse_quoted_string(token_reader)};
        case MULTILINE_STRING_LITERAL:
            return Value{_parse_multiline_string(token_reader)};
        case DATE_TIME_LITERAL:
            return _parse_date_time(token_reader);
        case OPENING_BRACKET:
            return Value{_parse_array(token_reader)};
        case LEFT_BRACE:
            return Value{_parse_inline_table(token_reader)};
        default:
            return _parse_bare_value(token_reader);
        }
    }

    Value Builder::_parse_bare_value(syntax::TokenReader& token_reader)
    {
        cl7::u8osstream oss;
        while (token_reader.check_symbol_id(ANY_OTHER) || token_reader.check_symbol_id(DOT))
            oss << token_reader.consume_token().lexeme;

        const auto raw = std::move(oss).str();
        if (raw.empty())
        {
            _error(u8"Value expected.", token_reader.peek_token());
            return {};
        }

        if (raw == u8"true") return Value{true};
        if (raw == u8"false") return Value{false};

        if (raw == u8"inf" || raw == u8"+inf") return Value{std::numeric_limits<float_t>::infinity()};
        if (raw == u8"-inf") return Value{-std::numeric_limits<float_t>::infinity()};
        if (raw == u8"nan" || raw == u8"+nan" || raw == u8"-nan") return Value{std::numeric_limits<float_t>::quiet_NaN()};

        // Integers with a base prefix (0x/0o/0b) are parsed before the decimal forms.
        if (const auto integer = _try_parse_based_integer(raw))
            return Value{*integer};

        // Underscores are allowed as digit separators in numbers; strip them before
        // classifying/parsing (but keep the original text for the string fallback).
        const auto digits = _strip_underscores(raw);

        if (!digits.empty() && syntax::matchers::IntegerLiteralMatcher{}(digits) == digits.length())
        {
            std::istringstream iss{std::string{cl7::text::codec::reinterpret_utf8(digits)}};
            integer_t number = {};
            iss >> number;
            assert(!iss.bad());
            return Value{number};
        }

        if (!digits.empty() && syntax::matchers::FloatingPointLiteralMatcher{}(digits) == digits.length())
        {
            std::istringstream iss{std::string{cl7::text::codec::reinterpret_utf8(digits)}};
            float_t number = {};
            iss >> number;
            assert(!iss.bad());
            return Value{number};
        }

        // Any other bare token is kept as a string so that it at least round-trips
        // losslessly through re-serialization.
        return Value{raw};
    }

    array_t Builder::_parse_array(syntax::TokenReader& token_reader)
    {
        if (!token_reader.skip_symbol_id(OPENING_BRACKET))
        {
            _error(u8"'[' expected.", token_reader.peek_token());
            return {};
        }

        array_t array;

        _skip_whitespace_and_newlines(token_reader);

        while (token_reader.is_valid() && !token_reader.check_symbol_id(CLOSING_BRACKET))
        {
            array.push_back(_parse_value(token_reader));

            _skip_whitespace_and_newlines(token_reader);

            if (token_reader.check_symbol_id(CLOSING_BRACKET))
                break;

            if (token_reader.skip_symbol_id(COMMA))
            {
                _skip_whitespace_and_newlines(token_reader);
                continue;
            }

            _error(u8"',' or ']' expected.", token_reader.peek_token());
            break;
        }

        if (token_reader.is_eof())
            _error(u8"Unexpected end of file.", token_reader.peek_token());

        token_reader.skip_symbol_id(CLOSING_BRACKET);

        return array;
    }

    table_t Builder::_parse_inline_table(syntax::TokenReader& token_reader)
    {
        if (!token_reader.skip_symbol_id(LEFT_BRACE))
        {
            _error(u8"'{' expected.", token_reader.peek_token());
            return {};
        }

        table_t table;

        _skip_inline_whitespace(token_reader);

        if (token_reader.skip_symbol_id(RIGHT_BRACE))
            return table;

        for (;;)
        {
            _parse_assignment(table, token_reader);

            _skip_inline_whitespace(token_reader);

            if (token_reader.skip_symbol_id(RIGHT_BRACE))
                break;

            if (token_reader.skip_symbol_id(COMMA))
            {
                _skip_inline_whitespace(token_reader);
                continue;
            }

            _error(u8"',' or '}' expected.", token_reader.peek_token());
            break;
        }

        return table;
    }



    string_t Builder::_parse_quoted_string(syntax::TokenReader& token_reader)
    {
        if (!token_reader.check_symbol_id(QUOTED_STRING_LITERAL))
        {
            _error(u8"String expected.", token_reader.peek_token());
            return {};
        }

        const auto token = token_reader.consume_token();
        const auto lexeme = token.lexeme;

        assert(lexeme.size() >= 2);
        const auto quote_char = lexeme[0];
        assert(lexeme.starts_with(quote_char));
        assert(lexeme.ends_with(quote_char));

        const auto inner = lexeme.substr(1, lexeme.size() - 2);

        // Literal strings (single-quoted) are taken verbatim: no escape processing.
        if (quote_char == u8'\'')
            return string_t{inner};

        // Basic strings (double-quoted): reuse the JSON unescaper for convenience.
        json::detail::UnescaperErrorHandler unescaper_error_handler{get_diagnostics(), &token};
        json::util::Unescaper unescaper{&unescaper_error_handler};

        return unescaper.unescape_string(inner);
    }

    string_t Builder::_parse_multiline_string(syntax::TokenReader& token_reader)
    {
        if (!token_reader.check_symbol_id(MULTILINE_STRING_LITERAL))
        {
            _error(u8"String expected.", token_reader.peek_token());
            return {};
        }

        const auto token = token_reader.consume_token();
        const auto lexeme = token.lexeme;

        assert(lexeme.size() >= 6);
        const auto quote_char = lexeme[0];

        auto inner = lexeme.substr(3, lexeme.size() - 6);

        // A newline immediately following the opening delimiter is trimmed.
        if (inner.starts_with(u8"\r\n"))
            inner = inner.substr(2);
        else if (inner.starts_with(u8"\n"))
            inner = inner.substr(1);

        // Multi-line literal strings (single-quoted) are taken verbatim.
        if (quote_char == u8'\'')
            return string_t{inner};

        // Multi-line basic strings (double-quoted): first resolve line-ending
        // backslashes (a backslash as the last non-whitespace character of a line
        // trims itself and all following whitespace up to the next non-whitespace
        // character), then reuse the JSON unescaper for the remaining escapes.
        cl7::u8osstream oss;
        for (size_t i = 0; i < inner.size(); )
        {
            if (inner[i] == u8'\\')
            {
                size_t j = i + 1;
                while (j < inner.size() && (inner[j] == u8' ' || inner[j] == u8'\t'))
                    ++j;

                if (j < inner.size() && (inner[j] == u8'\n' || inner[j] == u8'\r'))
                {
                    // Line-ending backslash: skip it and all following whitespace.
                    i = j;
                    while (i < inner.size() && (inner[i] == u8' ' || inner[i] == u8'\t' || inner[i] == u8'\n' || inner[i] == u8'\r'))
                        ++i;
                    continue;
                }

                // Regular escape sequence: keep the backslash and the escaped character
                // together so the unescaper can process them.
                oss << inner[i];
                if (i + 1 < inner.size())
                    oss << inner[i + 1];
                i += 2;
                continue;
            }

            oss << inner[i];
            ++i;
        }

        json::detail::UnescaperErrorHandler unescaper_error_handler{get_diagnostics(), &token};
        json::util::Unescaper unescaper{&unescaper_error_handler};

        return unescaper.unescape_string(std::move(oss).str());
    }

    Value Builder::_parse_date_time(syntax::TokenReader& token_reader)
    {
        const auto token = token_reader.consume_token();
        const cl7::u8string_view s = token.lexeme;

        const auto num = [&s](size_t pos, size_t count) {
            int value = 0;
            for (size_t i = 0; i < count; ++i)
                value = value * 10 + (s[pos + i] - u8'0');
            return value;
        };

        // A standalone local time has no date part (`HH:MM:SS`).
        const bool has_date = s.size() >= 10 && s[4] == u8'-';
        if (!has_date)
        {
            const auto time_of_day = std::chrono::hours{num(0, 2)} + std::chrono::minutes{num(3, 2)} + std::chrono::seconds{num(6, 2)};
            return Value{local_time_t{time_of_day}};
        }

        const std::chrono::year_month_day ymd{
            std::chrono::year{num(0, 4)},
            std::chrono::month{static_cast<unsigned>(num(5, 2))},
            std::chrono::day{static_cast<unsigned>(num(8, 2))},
        };

        // Just a local date?
        if (s.size() <= 10 || (s[10] != u8'T' && s[10] != u8't' && s[10] != u8' '))
            return Value{local_date_t{ymd}};

        const size_t t = 11;
        const auto seconds_of_day = std::chrono::hours{num(t, 2)} + std::chrono::minutes{num(t + 3, 2)} + std::chrono::seconds{num(t + 6, 2)};

        // Skip past the (possibly fractional) seconds to reach the optional offset.
        size_t after = t + 8;
        if (after < s.size() && s[after] == u8'.')
        {
            ++after;
            while (after < s.size() && s[after] >= u8'0' && s[after] <= u8'9')
                ++after;
        }

        // Local date-time (no offset).
        if (after >= s.size())
        {
            const std::chrono::local_days ld{std::chrono::sys_days{ymd}.time_since_epoch()};
            return Value{local_date_time_t{ld + seconds_of_day}};
        }

        // Offset date-time: store the instant as UTC and keep the offset for display.
        int offset_minutes = 0;
        if (s[after] != u8'Z' && s[after] != u8'z')
        {
            const int sign = s[after] == u8'-' ? -1 : 1;
            offset_minutes = sign * (num(after + 1, 2) * 60 + num(after + 4, 2));
        }

        const auto utc = std::chrono::sys_days{ymd} + seconds_of_day - std::chrono::minutes{offset_minutes};
        return Value{offset_date_time_t{utc, static_cast<int16_t>(offset_minutes)}};
    }



    table_t& Builder::_descend(table_t& table, const string_t& key, const syntax::Token& token)
    {
        // Note: operator[] gets-or-creates without clobbering an existing value.
        Value& value = table[key];

        // Descending into an array-of-tables refers to its most recently added table.
        if (value.is_array())
        {
            auto& array = value.as_array();
            if (array.empty() || !array.back().is_table())
            {
                _error(u8"Key is not an array of tables.", token);
                array.push_back(Value{Value::Type::Table});
            }
            return array.back().as_table();
        }

        if (!value.is_table())
        {
            if (!value.is_none())
                _error(u8"Key is already defined as a non-table value.", token);
            value.set_table({});
        }

        return value.as_table();
    }

    table_t& Builder::_descend_array(table_t& table, const string_t& key, const syntax::Token& token)
    {
        // Note: operator[] gets-or-creates without clobbering an existing value.
        Value& value = table[key];

        if (!value.is_array())
        {
            if (!value.is_none())
                _error(u8"Key is already defined as a non-array value.", token);
            value.set_array({});
        }

        // Each `[[name]]` header appends a new table to the array.
        value.as_array().push_back(Value{Value::Type::Table});
        return value.as_array().back().as_table();
    }



    void Builder::_skip_inline_whitespace(syntax::TokenReader& token_reader)
    {
        while (token_reader.check_symbol_id(WHITESPACE))
            token_reader.next_token();
    }

    void Builder::_skip_whitespace_and_newlines(syntax::TokenReader& token_reader)
    {
        while (true)
        {
            if (token_reader.check_symbol_id(WHITESPACE) || token_reader.check_symbol_id(NEWLINE))
            {
                token_reader.next_token();
                continue;
            }
            if (token_reader.check_symbol_id(COMMENT_DELIMITER))
            {
                _parse_comment(token_reader);
                continue;
            }
            break;
        }
    }



    void Builder::_error(cl7::u8string_view message, const syntax::Token& token)
    {
        get_diagnostics()->add(syntax::Diagnostic::Severity::Error, message, token.source_offset);
    }



} // namespace dl7::toml::detail
