#include "Generator.h"

#include "../util/Escaper.h"



namespace dl7::xml::detail {



    Generator::Generator(syntax::Diagnostics* diagnostics, const Format& format)
        : AstProcessor(diagnostics)
        , _format(format)
    {
    }



    cl7::u8string Generator::process(const Document& document)
    {
        cl7::u8osstream oss;

        oss << u8R"(<?xml version="1.0" encoding="utf-8"?>)";
        _try_write_newline(oss);

        if (_format.insert_doctype)
        {
            oss << u8"<!DOCTYPE " << document.root_element().get_name() << u8">";
            _try_write_newline(oss);
        }

        _write_element(oss, document.root_element(), 0);

        if (_format.style == Format::Style::Pretty && _format.pretty_options.add_empty_line)
            _try_write_newline(oss);

        return std::move(oss).str();
    }



    void Generator::_write_element(cl7::u8osstream& oss, const Element& element, unsigned indent_level) const
    {
        if (element.get_name().empty())
            return;

        oss << u8"<";
        oss << element.get_name();

        for (const auto& attribute : element.attributes())
        {
            oss << u8' ';
            oss << attribute.get_name();
            oss << u8'=';
            _write_attribute_value(oss, attribute.get_value());
        }

        if (element.is_empty())
        {
            oss << u8"/>";
            return;
        }

        oss << u8'>';

        if (element.find_child_elements().empty())
        {
            _write_text(oss, element.concatenate_text_content(), indent_level + 1);
        }
        else
        {
            const auto child_nodes = element.find_child_nodes();
            _write_element_content(oss, element, indent_level + 1);
            if (child_nodes.back()->is_element())
            {
                _try_write_newline(oss);
                _try_write_indentation(oss, indent_level);
            }
        }

        oss << u8"</";
        oss << element.get_name();
        oss << u8'>';
    }

    void Generator::_write_element_content(cl7::u8osstream& oss, const Element& element, unsigned indent_level) const
    {
        bool last_was_element = true;

        for (const Node* node : element.find_child_nodes())
        {
            switch (node->get_type())
            {
            case Node::Type::Element:
                if (last_was_element)
                {
                    _try_write_newline(oss);
                    _try_write_indentation(oss, indent_level);
                }
                _write_element(oss, *reinterpret_cast<const Element*>(node), indent_level);
                break;
            case Node::Type::Text:
                _write_text(oss, *reinterpret_cast<const Text*>(node), indent_level);
                break;
            default:
                assert(false);
            }

            last_was_element = node->get_type() == Node::Type::Element;
        } // for each child node
    }

    void Generator::_write_text(cl7::u8osstream& oss, const Text& text, unsigned indent_level) const
    {
        if (text.get_content().empty())
            return;

        util::Escaper escaper{get_diagnostics()->get_log_context()};
        escaper.escape_string(oss, text.get_content(), util::Escaper::EscapeType::ElementContent, {.force_escape = _format.force_escape, .escape_unicode = _format.escape_unicode});
    }



    cl7::u8osstream& Generator::_write_attribute_value(cl7::u8osstream& oss, cl7::u8string_view value) const
    {
        size_t single_quotes = 0;
        size_t double_quotes = 0;
        for (const auto& ch : value)
        {
            if (ch == u8'\'') ++single_quotes;
            if (ch == u8'"') ++double_quotes;
        }

        cl7::u8char_t quote_char = single_quotes < double_quotes ? u8'\'' : u8'"';

        oss << quote_char;

        util::Escaper escaper{get_diagnostics()->get_log_context()};
        escaper.escape_string(oss, value, static_cast<util::Escaper::EscapeType>(quote_char), {.force_escape = _format.force_escape, .escape_unicode = _format.escape_unicode});

        return oss << quote_char;
    }



    cl7::u8osstream& Generator::_try_write_indentation(cl7::u8osstream& oss, unsigned indent_level) const
    {
        if (_format.style != Format::Style::Pretty)
            return oss;

        if (_format.pretty_options.indentation == 0)
            for (size_t i = 0; i < indent_level; ++i)
                oss.put(u8'\t');
        else // => spaces
            for (size_t i = 0; i < static_cast<size_t>(indent_level) * static_cast<unsigned>(_format.pretty_options.indentation); ++i)
                oss.put(u8' ');

        return oss;
    }

    cl7::u8osstream& Generator::_try_write_newline(cl7::u8osstream& oss) const
    {
        if (_format.style == Format::Style::Pretty)
            _write_newline(oss);

        return oss;
    }

    cl7::u8osstream& Generator::_write_newline(cl7::u8osstream& oss) const
    {
        switch (_format.pretty_options.line_ending)
        {
        case Format::PrettyOptions::LineEnding::CRLF:
            oss << "\r\n";
            break;
        case Format::PrettyOptions::LineEnding::LF:
            oss << '\n';
            break;
        case Format::PrettyOptions::LineEnding::CR:
            oss << '\r';
            break;
        }

        return oss;
    }



} // namespace dl7::xml::detail
