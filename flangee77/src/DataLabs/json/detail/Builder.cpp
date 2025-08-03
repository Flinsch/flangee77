#include "Builder.h"

#include "./Symbol.h"
#include "./UnescaperErrorHandler.h"

#include "../util/Unescaper.h"

#include <CoreLabs/strings.h>

#include <sstream>



namespace dl7::json::detail {



    Builder::Builder(syntax::Diagnostics* diagnostics)
        : DirectAstBuilder(diagnostics)
    {
    }



    std::unique_ptr<Json> Builder::build(syntax::TokenReader& token_reader)
    {
        return _parse_value(token_reader);
    }



    std::unique_ptr<Json> Builder::_parse_value(syntax::TokenReader& token_reader)
    {
        while (token_reader.check_symbol_id(LINE_COMMENT) || token_reader.check_symbol_id(BLOCK_COMMENT))
            token_reader.next_token();

        switch (token_reader.peek_symbol_id())
        {
        case NULL_CONSTANT:
            token_reader.next_token();
            return std::make_unique<Json>(Json::Type::Null);
        case TRUE_CONSTANT:
            token_reader.next_token();
            return std::make_unique<Json>(true);
        case FALSE_CONSTANT:
            token_reader.next_token();
            return std::make_unique<Json>(false);

        case INTEGER_LITERAL:
            if (token_reader.check_first_char(u8'-') || token_reader.check_first_char(u8'+'))
                return std::make_unique<Json>(_parse_integer(token_reader));
            return std::make_unique<Json>(_parse_unsigned(token_reader));
        case DECIMAL_LITERAL:
            return std::make_unique<Json>(_parse_decimal(token_reader));

        case STRING_LITERAL:
            return std::make_unique<Json>(_parse_string(token_reader));

        case LEFT_CURLY_BRACKET:
            return std::make_unique<Json>(_parse_object(token_reader));
        case LEFT_SQUARE_BRACKET:
            return std::make_unique<Json>(_parse_array(token_reader));

        default:
            _error(u8"Value expected.", token_reader.peek_token());
            return {};
        }
    }



    object_t Builder::_parse_object(syntax::TokenReader& token_reader)
    {
        if (!token_reader.skip_symbol_id(LEFT_CURLY_BRACKET))
        {
            _error(u8"'{' expected.", token_reader.peek_token());
            return {};
        }

        object_t object;

        while (token_reader.is_valid() && !token_reader.check_symbol_id(RIGHT_CURLY_BRACKET))
        {
            auto member = _parse_object_member(token_reader);
            if (member.second)
                object[std::move(member.first)] = std::move(*member.second);

            if (token_reader.check_symbol_id(RIGHT_CURLY_BRACKET))
                break;

            if (token_reader.skip_symbol_id(COMMA))
                continue;

            _error(u8"',' or '}' expected.", member.second ? token_reader.peek_token() : token_reader.consume_token());
        }

        if (token_reader.is_eof())
            _error(u8"Unexpected end of file.", token_reader.peek_token());

        token_reader.next_token();

        return object;
    }

    array_t Builder::_parse_array(syntax::TokenReader& token_reader)
    {
        if (!token_reader.skip_symbol_id(LEFT_SQUARE_BRACKET))
        {
            _error(u8"'[' expected.", token_reader.peek_token());
            return {};
        }

        array_t array;

        while (token_reader.is_valid() && !token_reader.check_symbol_id(RIGHT_SQUARE_BRACKET))
        {
            auto value = _parse_value(token_reader);
            if (value)
                array.push_back(std::move(*value));

            if (token_reader.check_symbol_id(RIGHT_SQUARE_BRACKET))
                break;

            if (token_reader.skip_symbol_id(COMMA))
                continue;

            _error(u8"',' or ']' expected.", value ? token_reader.peek_token() : token_reader.consume_token());
        }

        if (token_reader.is_eof())
            _error(u8"Unexpected end of file.", token_reader.peek_token());

        token_reader.next_token();

        return array;
    }

    string_t Builder::_parse_string(syntax::TokenReader& token_reader)
    {
        if (!token_reader.check_symbol_id(STRING_LITERAL))
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

        UnescaperErrorHandler unescaper_error_handler{get_diagnostics(), token};
        util::Unescaper unescaper{&unescaper_error_handler};

        return unescaper.unescape_string(lexeme.substr(1, lexeme.size() - 2));
    }

    decimal_t Builder::_parse_decimal(syntax::TokenReader& token_reader)
    {
        if (!token_reader.check_symbol_id(DECIMAL_LITERAL))
        {
            _error(u8"Decimal number expected.", token_reader.peek_token());
            return {};
        }

        const auto token = token_reader.consume_token();
        const auto lexeme = token.lexeme;

        std::istringstream iss{std::string{cl7::strings::reinterpret_utf8(lexeme)}};
        decimal_t number = {};
        iss >> number;

        if (iss.bad())
            _error(u8"Bad decimal literal.", token);

        return number;
    }

    integer_t Builder::_parse_integer(syntax::TokenReader& token_reader)
    {
        if (!token_reader.check_symbol_id(INTEGER_LITERAL))
        {
            _error(u8"(Signed) integer number expected.", token_reader.peek_token());
            return {};
        }

        const auto token = token_reader.consume_token();
        const auto lexeme = token.lexeme;

        std::istringstream iss{std::string{cl7::strings::reinterpret_utf8(lexeme)}};
        integer_t number = {};
        iss >> number;

        if (iss.bad())
            _error(u8"Bad (signed) integer literal.", token);

        return number;
    }

    unsigned_t Builder::_parse_unsigned(syntax::TokenReader& token_reader)
    {
        if (!token_reader.check_symbol_id(INTEGER_LITERAL))
        {
            _error(u8"Unsigned integer number expected.", token_reader.peek_token());
            return {};
        }

        const auto token = token_reader.consume_token();
        const auto lexeme = token.lexeme;

        std::istringstream iss{std::string{cl7::strings::reinterpret_utf8(lexeme)}};
        unsigned_t number = {};
        iss >> number;

        if (iss.bad())
            _error(u8"Bad unsigned integer literal.", token);

        return number;
    }



    std::pair<string_t, std::unique_ptr<Json>> Builder::_parse_object_member(syntax::TokenReader& token_reader)
    {
        auto key = _parse_object_key(token_reader);

        if (!token_reader.skip_symbol_id(COLON))
            _error(u8"':' expected.", token_reader.peek_token());

        auto value = _parse_value(token_reader);

        return {std::move(key), std::move(value)};
    }

    string_t Builder::_parse_object_key(syntax::TokenReader& token_reader)
    {
        if (token_reader.check_symbol_id(STRING_LITERAL))
            return _parse_string(token_reader);
        if (token_reader.check_symbol_id(IDENTIFIER))
            return _parse_identifier(token_reader);

        _error(u8"Object key expected.", token_reader.peek_token());

        return {};
    }

    string_t Builder::_parse_identifier(syntax::TokenReader& token_reader)
    {
        if (!token_reader.check_symbol_id(IDENTIFIER))
        {
            _error(u8"Identifier expected.", token_reader.peek_token());
            return {};
        }

        const auto token = token_reader.consume_token();
        const auto lexeme = token.lexeme;

        return string_t{lexeme};
    }



    void Builder::_error(cl7::u8string_view message, const syntax::Token& token)
    {
        get_diagnostics()->add(syntax::Diagnostic::Severity::Error, message, token.source_offset);
    }



} // namespace dl7::json::detail
