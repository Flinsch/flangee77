#include "Builder.h"

#include "./Symbol.h"
#include "./UnescaperErrorHandler.h"

#include "../util/Unescaper.h"

#include <CoreLabs/text/transform.h>


namespace dl7::xml::detail {



    Builder::Builder(syntax::Diagnostics* diagnostics, WhitespaceHandling whitespace_handling)
        : DirectAstBuilder(diagnostics)
        , _whitespace_handling(whitespace_handling)
    {
    }



    std::unique_ptr<Document> Builder::build(syntax::TokenReader& token_reader)
    {
        return _parse_document(token_reader);
    }



    std::unique_ptr<Document> Builder::_parse_document(syntax::TokenReader& token_reader)
    {
        if (token_reader.check_symbol_id(PI_START))
        {
            if (!_parse_prolog(token_reader))
                return {};
        }

        auto document = std::make_unique<Document>();

        document->root_element() = _parse_element(token_reader, _whitespace_handling);

        _skip_misc(token_reader);

        return document;
    }



    bool Builder::_parse_prolog(syntax::TokenReader& token_reader)
    {
        if (!token_reader.skip_symbol_id(PI_START) || !token_reader.skip_lexeme(u8"xml"))
        {
            _error(u8"Prolog expected.", token_reader.peek_token());
            return false;
        }

        if (!token_reader.skip_past_symbol_id(WHITESPACE))
            _error(u8"Whitespace expected.", token_reader.peek_token());

        Element prolog;
        prolog.attributes() = _parse_attributes(token_reader);
        const Attribute* attribute = nullptr;
        bool all_okay = true;

        attribute = prolog.find_attribute(u8"version");
        if (attribute == nullptr)
        {
            _error(u8"XML version expected.", token_reader.peek_token());
            all_okay = false;
        }
        else if (attribute->get_value() != u8"1.0")
        {
            _error(u8"Bad XML version number: \"" + attribute->get_value() + u8"\" (should be \"1.0\").", token_reader.peek_token());
            all_okay = false;
        }

        attribute = prolog.find_attribute(u8"encoding");
        if (attribute != nullptr && cl7::text::transform::to_lower_ascii(attribute->get_value()) != u8"utf-8")
        {
            _error(u8"Bad character encoding: \"" + attribute->get_value() + u8"\". Only UTF-8 is supported.", token_reader.peek_token());
            all_okay = false;
        }

        attribute = prolog.find_attribute(u8"standalone");
        if (attribute != nullptr &&
            cl7::text::transform::to_lower_ascii(attribute->get_value()) != u8"yes" &&
            cl7::text::transform::to_lower_ascii(attribute->get_value()) != u8"no")
        {
            _error(u8"Bad 'standalone' value: \"" + attribute->get_value() + u8"\"", token_reader.peek_token());
            all_okay = false;
        }

        token_reader.skip_while_symbol_id(WHITESPACE);
        if (!token_reader.skip_symbol_id(PI_END))
        {
            _error(u8"'?>' expected.", token_reader.peek_token());
            return false;
        }

        _skip_misc(token_reader);

        if (token_reader.check_symbol_id(DOCTYPE))
        {
            all_okay &= _parse_doctype(token_reader);
            _skip_misc(token_reader);
        }

        return all_okay;
    }

    bool Builder::_parse_doctype(syntax::TokenReader& token_reader)
    {
        if (!token_reader.check_symbol_id(DOCTYPE))
        {
            _error(u8"DOCTYPE declaration expected.", token_reader.peek_token());
            return false;
        }

        token_reader.next_token();
        return true;
    }



    Element Builder::_parse_element(syntax::TokenReader& token_reader, WhitespaceHandling whitespace_handling)
    {
        if (!token_reader.skip_symbol_id(START_TAG_START))
        {
            _error(u8"'<' expected.", token_reader.peek_token());
            return {};
        }

        Element element;
        element.set_name(_parse_name(token_reader));
        if (element.get_name().empty())
            return {};

        element.attributes() = _parse_attributes(token_reader);

        if (element.has_attribute(u8"xml:space"))
        {
            const cl7::u8string& xml_space = element.find_attribute(u8"xml:space")->get_value();
            if (xml_space == u8"default")
                whitespace_handling = _whitespace_handling;
            else if (xml_space == u8"preserve")
                whitespace_handling = WhitespaceHandling::Preserve;
            else if (xml_space == u8"replace")
                whitespace_handling = WhitespaceHandling::Replace;
            else if (xml_space == u8"collapse")
                whitespace_handling = WhitespaceHandling::Collapse;
            else
                _warning(u8"Bad 'xml:space' value: " + xml_space, token_reader.peek_token());
        }

        token_reader.skip_while_symbol_id(WHITESPACE);
        if (token_reader.skip_symbol_id(EMPTY_TAG_END))
            return element;

        if (!token_reader.skip_symbol_id(TAG_END))
        {
            _error(u8"'>' expected.", token_reader.peek_token());
            return {};
        }

        auto nodes = _parse_content(token_reader, whitespace_handling);
        for (auto& p : nodes)
        {
            switch (p->get_type())
            {
            case Node::Type::Element:
                *element.append_element(u8"") = std::move(*reinterpret_cast<Element*>(p.get()));
                break;
            case Node::Type::Text:
                *element.append_text(u8"") = std::move(*reinterpret_cast<Text*>(p.get()));
                break;
            default:
                assert(false);
            }
        }

        if (!token_reader.skip_symbol_id(END_TAG_START))
        {
            _error(u8"'</' expected.", token_reader.peek_token());
            return {};
        }

        if (!token_reader.check_symbol_id(NAME) || !token_reader.skip_lexeme(element.get_name()))
        {
            _error(u8"Matching closing tag name expected: " + element.get_name(), token_reader.peek_token());
            return {};
        }

        token_reader.skip_while_symbol_id(WHITESPACE);

        if (!token_reader.skip_symbol_id(TAG_END))
            _error(u8"'>' expected.", token_reader.peek_token());

        return element;
    }



    Attribute Builder::_parse_attribute(syntax::TokenReader& token_reader)
    {
        Attribute attribute;
        attribute.set_name(_parse_name(token_reader));
        if (attribute.get_name().empty())
            return {};

        if (!_parse_equal_sign(token_reader))
            return attribute; // Still return attribute with empty value.

        attribute.set_value(_parse_attribute_value(token_reader));

        return attribute;
    }

    bool Builder::_parse_equal_sign(syntax::TokenReader& token_reader)
    {
        token_reader.skip_while_symbol_id(WHITESPACE);

        if (!token_reader.skip_symbol_id(EQUAL_SIGN))
        {
            _error(u8"'=' expected.", token_reader.peek_token());
            return false;
        }

        token_reader.skip_while_symbol_id(WHITESPACE);

        return true;
    }

    cl7::u8string Builder::_parse_attribute_value(syntax::TokenReader& token_reader)
    {
        if (!token_reader.check_symbol_id(STRING))
        {
            _error(u8"Attribute value expected.", token_reader.peek_token());
            return {};
        }

        const auto token = token_reader.consume_token();
        const auto lexeme = token.lexeme;

        assert(lexeme.size() >= 2);
        const auto quote_char = lexeme[0];
        assert(lexeme.starts_with(quote_char));
        assert(lexeme.ends_with(quote_char));

        UnescaperErrorHandler unescaper_error_handler{get_diagnostics(), &token};
        util::Unescaper unescaper{&unescaper_error_handler};

        return unescaper.unescape_string(lexeme.substr(1, lexeme.size() - 2), util::Unescaper::UnescapeType::AttributeValue);
    }



    std::vector<std::unique_ptr<Node>> Builder::_parse_content(syntax::TokenReader& token_reader, WhitespaceHandling whitespace_handling)
    {
        std::vector<std::unique_ptr<Node>> nodes;

        while (true)
        {
            cl7::u8string char_data = _parse_char_data(token_reader, whitespace_handling);
            if (!char_data.empty() && !cl7::text::transform::trimmed(cl7::u8string_view(char_data), u8" \t\n\r").empty())
            {
                nodes.push_back(std::make_unique<Text>(char_data));
                continue;
            }

            if (token_reader.check_symbol_id(START_TAG_START))
            {
                nodes.push_back(std::make_unique<Element>(_parse_element(token_reader, whitespace_handling)));
                continue;
            }

            if (token_reader.check_symbol_id(END_TAG_START))
                break;
        }

        return nodes;
    }

    cl7::u8string Builder::_parse_char_data(syntax::TokenReader& token_reader, WhitespaceHandling whitespace_handling)
    {
        cl7::u8osstream oss;
        cl7::u8osstream buffer;

        auto flush = [&oss, &buffer, whitespace_handling](bool final = false) -> cl7::u8osstream& {
            static constexpr cl7::u8string_view whitespace_chars = u8" \t\n\r";

            switch (whitespace_handling)
            {
            case WhitespaceHandling::Preserve:
                oss << buffer.view();
                break;
            case WhitespaceHandling::Replace:
            {
                cl7::u8string_view char_data = buffer.view();

                for (const auto ch : char_data)
                {
                    if (whitespace_chars.find(ch) == cl7::u8string_view::npos)
                        oss << ch;
                    else
                        oss << u8' ';
                }
                break;
            }
            case WhitespaceHandling::Collapse:
            {
                cl7::u8string_view char_data = buffer.view();

                for (size_t i = 0; i < char_data.size(); )
                {
                    const auto ch = char_data[i++];
                    if (whitespace_chars.find(ch) == cl7::u8string_view::npos)
                    {
                        oss << ch;
                        continue;
                    }

                    i = char_data.find_first_not_of(whitespace_chars, i);
                    if (i != cl7::u8string_view::npos || !final)
                        oss << u8' ';
                }
                break;
            }
            default:
                assert(false);
            }

            buffer = {};
            return oss;
        };

        _skip_misc(token_reader, whitespace_handling == WhitespaceHandling::Collapse);

        while (true)
        {
            _skip_misc(token_reader, false); // Keep whitespace

            if (token_reader.check_symbol_id(CDATA))
            {
                flush() << _parse_cdata_section(token_reader);
                continue;
            }

            if (token_reader.check_symbol_id(REF))
            {
                const auto token = token_reader.consume_token();
                const auto lexeme = token.lexeme;

                UnescaperErrorHandler unescaper_error_handler{get_diagnostics(), &token};
                util::Unescaper unescaper{&unescaper_error_handler};

                flush() << unescaper.unescape_string(lexeme, util::Unescaper::UnescapeType::ElementContent);
                continue;
            }

            if (token_reader.check_first_char(u8'<'))
                break;

            buffer << token_reader.consume_token().lexeme;
        }

        return flush(true).str();
    }

    cl7::u8string Builder::_parse_cdata_section(syntax::TokenReader& token_reader)
    {
        if (!token_reader.check_symbol_id(CDATA))
        {
            _error(u8"'<![CDATA[' expected.", token_reader.peek_token());
            return {};
        }

        const auto token = token_reader.consume_token();
        const auto lexeme = token.lexeme;

        assert(lexeme.starts_with(u8"<![CDATA["));
        assert(lexeme.ends_with(u8"]]>"));

        return cl7::u8string{lexeme.substr(9, lexeme.size() - 12)};
    }



    cl7::u8string_view Builder::_parse_name(syntax::TokenReader& token_reader)
    {
        if (!token_reader.check_symbol_id(NAME))
        {
            _error(u8"Name expected.", token_reader.peek_token());
            return {};
        }

        return token_reader.consume_token().lexeme;
    }

    std::vector<Attribute> Builder::_parse_attributes(syntax::TokenReader& token_reader)
    {
        std::vector<Attribute> attributes;

        while (true)
        {
            token_reader.skip_while_symbol_id(WHITESPACE);

            if (!token_reader.check_symbol_id(NAME))
                break;

            auto attribute = _parse_attribute(token_reader);

            assert(!attribute.get_name().empty());

            attributes.emplace_back(std::move(attribute));
        }

        return attributes;
    }



    bool Builder::_skip_misc(syntax::TokenReader& token_reader, bool skip_whitespace)
    {
        while (!token_reader.is_eof())
        {
            if (token_reader.skip_symbol_id(COMMENT) || (skip_whitespace && token_reader.skip_symbol_id(WHITESPACE)))
                continue;

            if (token_reader.check_symbol_id(PI_START))
            {
                token_reader.next_token();

                if (_parse_name(token_reader).empty())
                    return false;

                while (true)
                {
                    if (token_reader.is_eof())
                    {
                        _error(u8"Unexpected end of file.", token_reader.peek_token());
                        return false;
                    }

                    if (token_reader.skip_symbol_id(PI_END))
                        break;

                    token_reader.next_token();
                }

                continue;
            } // processing instructions

            break;
        }

        return true;
    }



    void Builder::_error(cl7::u8string_view message, const syntax::Token& token)
    {
        get_diagnostics()->add(syntax::Diagnostic::Severity::Error, message, token.source_offset);
    }

    void Builder::_warning(cl7::u8string_view message, const syntax::Token& token)
    {
        get_diagnostics()->add(syntax::Diagnostic::Severity::Warning, message, token.source_offset);
    }



} // namespace dl7::xml::detail
