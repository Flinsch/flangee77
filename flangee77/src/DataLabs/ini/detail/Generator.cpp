#include "Generator.h"

#include "./BooleanMatcher.h"

#include <DataLabs/syntax/matchers.h>
#include <DataLabs/json/util/Escaper.h>

#include <CoreLabs/strings/codec/Encoder.h>
#include <CoreLabs/strings/codec/Decoder.h>

#include <cmath>
#include <algorithm>



namespace dl7::ini::detail {



    Generator::Generator(syntax::Diagnostics* diagnostics, const Format& format)
        : AstProcessor(diagnostics)
        , _format(format)
    {
    }



    cl7::u8string Generator::process(const Ini& root)
    {
        cl7::u8osstream oss;

        // Only write an empty line at start of file (if configured at all) if there
        // are global properties or if no separate empty line is to be inserted
        // before the otherwise first section (if such first subsection exists).
        if (_format.empty_line_at_start_of_file && (!root.properties().empty() || (!_format.empty_line_before_section_header && !root.sections().empty())))
            _write_newline(oss);

        _write_unnamed_section(oss, root, u8"");

        return std::move(oss).str();
    }



    void Generator::_write_unnamed_section(cl7::u8osstream& oss, const Section& section, cl7::u8string_view section_name_prefix) const
    {
        _write_properties(oss, section.properties());
        _write_subsections(oss, section.sections(), section_name_prefix);
    }



    void Generator::_write_subsections(cl7::u8osstream& oss, const Section::sections_t& sections, cl7::u8string_view section_name_prefix) const
    {
        for (const auto& p : sections)
        {
            const auto& section_name = p.first;
            const auto& section = p.second;

            cl7::u8osstream tmp;
            tmp << section_name_prefix;
            _write_section_name(tmp, section_name);
            const auto full_section_name = tmp.str();

            // Only write the name if the section contains any properties. Unless
            // the section is otherwise empty, meaning it has no subsections and is
            // therefore a leaf element, in which case the name is still written.
            if (!section.properties().empty() || section.sections().empty())
            {
                if (_format.empty_line_before_section_header)
                    _write_newline(oss);

                oss << u8"[" << full_section_name << u8"]";
                _write_newline(oss);

                if (_format.empty_line_after_section_header)
                    _write_newline(oss);
            }

            _write_unnamed_section(oss, section, full_section_name + static_cast<cl7::u8char_t>(_format.section_name_delimiter));
        }
    }

    void Generator::_write_properties(cl7::u8osstream& oss, const Section::properties_t& properties) const
    {
        for (const auto& p : properties)
        {
            const auto& key = p.first;
            const auto& value = p.second;

            _write_key(oss, key);

            if (_format.space_around_delimiter == Format::SpaceAroundKeyValueDelimiter::Before || _format.space_around_delimiter == Format::SpaceAroundKeyValueDelimiter::Both)
                oss << u8" ";

            oss << static_cast<cl7::u8char_t>(_format.key_value_delimiter);

            if (_format.space_around_delimiter == Format::SpaceAroundKeyValueDelimiter::After || _format.space_around_delimiter == Format::SpaceAroundKeyValueDelimiter::Both)
                oss << u8" ";

            _write_value(oss, value);
            _write_newline(oss);
        }
    }



    cl7::u8osstream& Generator::_write_section_name(cl7::u8osstream& oss, cl7::u8string_view string) const
    {
        return _write_string(oss, string, u8"[]\n\r./#;");
    }

    cl7::u8osstream& Generator::_write_key(cl7::u8osstream& oss, cl7::u8string_view string) const
    {
        return _write_string(oss, string, u8"=:\n\r#;[]");
    }

    cl7::u8osstream& Generator::_write_value(cl7::u8osstream& oss, const Value& value) const
    {
        if (value.is_true()) return oss << u8"true";
        if (value.is_false()) return oss << u8"false";
        if (value.is_string()) return _write_string_value(oss, value.as_string());
        if (value.is_integer()) return oss << value.as_integer();
        if (value.is_unsigned()) return oss << value.as_unsigned();
        if (value.is_decimal()) return _write_decimal_value(oss, value.as_decimal());

        return oss;
    }



    cl7::u8osstream& Generator::_write_string_value(cl7::u8osstream& oss, cl7::u8string_view string) const
    {
        return _write_string(oss, string, u8"\n\r\"'#;");
    }

    cl7::u8osstream& Generator::_write_decimal_value(cl7::u8osstream& oss, decimal_t decimal) const
    {
        if (std::isnan(decimal) || std::isinf(decimal))
        {
            switch (_format.float_policy)
            {
            case Format::FloatPolicy::ReplaceWithUndefined:
                return oss; // Nothing to write here.
            case Format::FloatPolicy::ReplaceWithZero:
                return oss << u8"0.0";
            case Format::FloatPolicy::EncodeAsString:
                return oss << (
                    std::isnan(decimal) ? u8"\"NaN\"" :
                    std::signbit(decimal) ? u8"\"-Infinity\"" : u8"\"Infinity\""
                );
            }
        }

        return oss << cl7::to_string(decimal);
    }



    cl7::u8osstream& Generator::_write_string(cl7::u8osstream& oss, cl7::u8string_view string, cl7::u8string_view invalid_chars) const
    {
        bool quotes_required = string.empty();

        for (const auto ch : string)
        {
            if (ch < 0x20 || invalid_chars.find(ch) != cl7::u8string_view::npos)
            {
                quotes_required = true;
                break;
            }
        }

        quotes_required = quotes_required ||
            syntax::matchers::IntegerLiteralMatcher{}(string) == string.length() ||
            syntax::matchers::FloatingPointLiteralMatcher{}(string) == string.length() ||
            BooleanMatcher{}(string) == string.length();

        if (quotes_required)
            return _write_quoted_string(oss, string);

        return _write_unquoted_string(oss, string);
    }

    cl7::u8osstream& Generator::_write_quoted_string(cl7::u8osstream& oss, cl7::u8string_view string) const
    {
        if (!_format.allow_single_quotes)
            return _write_quoted_string(oss, string, u8'"');

        size_t single_quotes = 0;
        size_t double_quotes = 0;
        for (const auto& ch : string)
        {
            if (ch == u8'\'') ++single_quotes;
            if (ch == u8'"') ++double_quotes;
        }

        cl7::u8char_t quote_char = single_quotes < double_quotes ? u8'\'' : u8'"';
        return _write_quoted_string(oss, string, quote_char);
    }

    cl7::u8osstream& Generator::_write_quoted_string(cl7::u8osstream& oss, cl7::u8string_view string, cl7::u8char_t quote_char) const
    {
        assert(quote_char == u8'"' || quote_char == u8'\'');

        oss << quote_char;

        // Just use the JSON escaper for convenience.
        json::util::Escaper escaper{get_diagnostics()->get_log_context()};
        escaper.escape_string(oss, string, {.quote_char = static_cast<json::util::Escaper::QuoteChar>(quote_char), .escape_unicode = false});

        return oss << quote_char;
    }

    cl7::u8osstream& Generator::_write_unquoted_string(cl7::u8osstream& oss, cl7::u8string_view string) const
    {
        cl7::strings::codec::DefaultErrorHandler encoding_error_handler{get_diagnostics()->get_log_context()};

        cl7::strings::codec::Encoder<cl7::u8char_t> encoder{&encoding_error_handler};
        cl7::u8char_t buffer[4];

        cl7::strings::codec::Decoder<cl7::u8char_t>::iterator it{string, &encoding_error_handler};
        cl7::strings::codec::Decoder<cl7::u8char_t>::sentinel end;

        for (; it != end; ++it)
        {
            const auto codepoint = *it;

            switch (codepoint.value)
            {
            case u8'\t': oss << u8"\\t"; break;
            case u8'\n': oss << u8"\\n"; break;
            case u8'\r': oss << u8"\\r"; break;
            case u8'\"': oss << u8"\\\""; break;
            case u8'\\': oss << u8"\\\\"; break;
            default:
                assert(codepoint.value >= 0x20);

                const auto size = encoder.encode_one_into(codepoint, cl7::u8string_span(buffer, 4));
                oss << cl7::u8string_view(buffer, size);
            }
        }

        return oss;
    }



    cl7::u8osstream& Generator::_write_newline(cl7::u8osstream& oss) const
    {
        switch (_format.newline)
        {
        case Format::Newline::CRLF:
            oss << "\r\n";
            break;
        case Format::Newline::LF:
            oss << '\n';
            break;
        case Format::Newline::CR:
            oss << '\r';
            break;
        }

        return oss;
    }



} // namespace dl7::ini::detail
