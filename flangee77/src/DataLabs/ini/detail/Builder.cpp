#include "Builder.h"

#include "./Symbol.h"
#include "./BooleanMatcher.h"

#include <DataLabs/syntax/matchers.h>
#include <DataLabs/json/util/Unescaper.h>
#include <DataLabs/json/detail/UnescaperErrorHandler.h>

#include <CoreLabs/text/codec/Encoder.h>
#include <CoreLabs/text/codec/Decoder.h>

#include <CoreLabs/text/codec.h>
#include <CoreLabs/text/transform.h>

#include <sstream>



namespace dl7::ini::detail {



    Builder::Builder(syntax::Diagnostics* diagnostics)
        : DirectAstBuilder(diagnostics)
    {
    }



    std::unique_ptr<Ini> Builder::build(syntax::TokenReader& token_reader)
    {
        auto ini = std::make_unique<Ini>();

        Context ctx;
        ctx.section = &*ini;

        _parse_ini(ctx, token_reader);

        return ini;
    }



    void Builder::_parse_ini(Context& ctx, syntax::TokenReader& token_reader)
    {
        while (!token_reader.is_eof())
        {
            if (!token_reader.is_valid())
            {
                _error(u8"Key-value pair expected.", token_reader.peek_token());
                break;
            }

            _parse_line(ctx, token_reader);
        }
    }



    void Builder::_parse_line(Context& ctx, syntax::TokenReader& token_reader)
    {
        while (token_reader.check_symbol_id(WHITESPACE) || token_reader.check_symbol_id(NEWLINE))
            token_reader.next_token();

        switch (token_reader.peek_symbol_id())
        {
        case COMMENT_DELIMITER:
            _parse_comment(token_reader);
            break;
        case OPENING_BRACKET:
            _parse_section(ctx, token_reader);
            break;
        default:
            _parse_key_value(ctx, token_reader);
            break;
        }

        while (token_reader.check_symbol_id(WHITESPACE))
            token_reader.next_token();

        if (token_reader.check_symbol_id(COMMENT_DELIMITER))
            _parse_comment(token_reader);

        if (!token_reader.skip_symbol_id(NEWLINE))
        {
            _error(u8"Line break expected.", token_reader.peek_token());
            token_reader.skip_past_symbol_id(NEWLINE);
        }
    }



    void Builder::_parse_comment(syntax::TokenReader& token_reader)
    {
        if (!token_reader.skip_symbol_id(COMMENT_DELIMITER))
        {
            _error(u8"';' or '#' expected.", token_reader.peek_token());
            return;
        }

        token_reader.skip_to_symbol_id(NEWLINE);
    }

    void Builder::_parse_section(Context& ctx, syntax::TokenReader& token_reader)
    {
        if (!token_reader.skip_symbol_id(OPENING_BRACKET))
        {
            _error(u8"'[' expected.", token_reader.peek_token());
            return;
        }

        while (_parse_section_name(ctx, token_reader))
        {
            if (!token_reader.skip_symbol_id(SECTION_NAME_DELIMITER))
                break;
        }

        if (!token_reader.skip_symbol_id(CLOSING_BRACKET))
            _error(u8"']' expected.", token_reader.peek_token());
    }

    void Builder::_parse_key_value(Context& ctx, syntax::TokenReader& token_reader)
    {
        auto key = _parse_key(token_reader);
        if (key.empty())
        {
            _error(u8"Key expected.", token_reader.peek_token());
            return;
        }

        auto it = ctx.section->properties().emplace(std::move(key), Value{}).first;

        if (!token_reader.skip_symbol_id(KEY_VALUE_DELIMITER))
        {
            _error(u8"'=' or ':' expected.", token_reader.peek_token());
            return;
        }

        it->second = _parse_value(token_reader);
    }



    bool Builder::_parse_section_name(Context& ctx, syntax::TokenReader& token_reader)
    {
        auto section_name = _parse_section_name(token_reader);
        if (section_name.empty())
        {
            _error(u8"Section name expected.", token_reader.peek_token());
            return false;
        }

        auto it = ctx.section->sections().emplace(std::move(section_name), Section{}).first;
        ctx.section = &it->second;
        return true;
    }

    string_t Builder::_parse_section_name(syntax::TokenReader& token_reader)
    {
        if (token_reader.check_symbol_id(QUOTED_STRING_LITERAL))
            return _parse_quoted_string(token_reader);

        cl7::u8osstream oss;

        while (token_reader.peek_lexeme().find_first_of(u8"[]\n\r./#;") == cl7::u8string_view::npos)
            oss << token_reader.consume_token().lexeme;

        return cl7::text::transform::trimmed(oss.str());
    }



    string_t Builder::_parse_key(syntax::TokenReader& token_reader)
    {
        if (token_reader.check_symbol_id(QUOTED_STRING_LITERAL))
            return _parse_quoted_string(token_reader);

        cl7::u8osstream oss;

        while (token_reader.peek_lexeme().find_first_of(u8"=:\n\r#;[]") == cl7::u8string_view::npos)
            oss << token_reader.consume_token().lexeme;

        return cl7::text::transform::trimmed(oss.str());
    }

    Value Builder::_parse_value(syntax::TokenReader& token_reader)
    {
        if (token_reader.peek_symbol_id() == QUOTED_STRING_LITERAL)
            return Value{_parse_quoted_string(token_reader)};

        auto string = _parse_unquoted_string(token_reader);
        if (string.empty())
            return {};

        if (syntax::matchers::IntegerLiteralMatcher{}(string) == string.length())
        {
            std::istringstream iss{std::string{cl7::text::codec::reinterpret_utf8(string)}};

            integer_t number = {};
            iss >> number;
            assert(!iss.bad());
            return Value{number};
        }

        if (syntax::matchers::FloatingPointLiteralMatcher{}(string) == string.length())
        {
            std::istringstream iss{std::string{cl7::text::codec::reinterpret_utf8(string)}};

            float_t number = {};
            iss >> number;
            assert(!iss.bad());
            return Value{number};
        }

        if (BooleanMatcher{BooleanMatcher::Mode::TruthyOnly}(string) == string.length())
            return Value{true};
        if (BooleanMatcher{BooleanMatcher::Mode::FalsyOnly}(string) == string.length())
            return Value{false};

        return Value{string};
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

        // Just use the JSON unescaper for convenience.
        json::detail::UnescaperErrorHandler unescaper_error_handler{get_diagnostics(), &token};
        json::util::Unescaper unescaper{&unescaper_error_handler};

        return unescaper.unescape_string(lexeme.substr(1, lexeme.size() - 2));
    }

    string_t Builder::_parse_unquoted_string(syntax::TokenReader& token_reader)
    {
        cl7::u8osstream oss;

        cl7::text::codec::DefaultErrorHandler encoding_error_handler{get_diagnostics()->get_log_context()};

        cl7::text::codec::Encoder<cl7::u8char_t> encoder{&encoding_error_handler};
        cl7::u8char_t buffer[4];

        bool is_eol = false;
        bool last_was_whitespace = false;

        while (token_reader.is_valid() && !is_eol)
        {
            bool is_whitespace = false;

            switch (token_reader.peek_symbol_id())
            {
            case WHITESPACE:
                oss << token_reader.consume_token().lexeme;
                is_whitespace = true;
                break;

            case NEWLINE:
                is_eol = true;
                break;

            case COMMENT_DELIMITER:
                if (last_was_whitespace)
                {
                    is_eol = true;
                }
                else
                {
                    oss << token_reader.consume_token().lexeme;
                }
                break;

            case ESCAPED_CHARACTER:
                assert(token_reader.peek_lexeme().length() == 2);
                switch (token_reader.peek_lexeme()[1])
                {
                case u8't': oss << u8'\t'; break;
                case u8'n': oss << u8'\n'; break;
                case u8'r': oss << u8'\r'; break;
                default:
                    // Just pass the escaped character.
                    oss << token_reader.peek_lexeme()[1];
                    break;
                }
                token_reader.next_token();
                break;

            case ESCAPED_NEWLINE:
            {
                auto tmp = cl7::text::transform::trimmed(oss.str());
                oss = cl7::u8osstream();
                oss << tmp;
                oss << u8" ";
                token_reader.next_token();
                while (token_reader.skip_symbol_id(WHITESPACE)) { /* Ignore leading whitespace. */ }
                break;
            }

            case QUOTED_STRING_LITERAL:
                oss << _parse_quoted_string(token_reader);
                break;

            default:
                for (cl7::text::codec::Decoder<cl7::u8char_t>::iterator it{token_reader.consume_token().lexeme, &encoding_error_handler};
                    it != cl7::text::codec::Decoder<cl7::u8char_t>::iterator{};
                    ++it)
                {
                    const auto codepoint = *it;

                    const auto size = encoder.encode_one_into(codepoint, cl7::u8string_span(buffer, 4));
                    oss << cl7::u8string_view(buffer, size);
                }
                break;
            } // switch

            last_was_whitespace = is_whitespace;
        } // while

        return cl7::text::transform::trimmed(oss.str());
    }



    void Builder::_error(cl7::u8string_view message, const syntax::Token& token)
    {
        get_diagnostics()->add(syntax::Diagnostic::Severity::Error, message, token.source_offset);
    }



} // namespace dl7::ini::detail
