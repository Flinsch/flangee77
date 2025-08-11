#include "Generator.h"

#include "../util/Validator.h"
#include "../util/Escaper.h"

#include <cmath>



namespace dl7::json::detail {



    Generator::Generator(syntax::Diagnostics* diagnostics, const Format& format)
        : AstProcessor(diagnostics)
        , _format(format)
    {
    }



    cl7::u8string Generator::process(const Json& root)
    {
        cl7::u8osstream oss;

        _write_json(oss, root, 0);

        if (_format.style == Format::Style::MultiLine && _format.multi_line_options.add_empty_line && root.is_structured() && !root.is_empty())
            _put_line_ending(oss);

        return std::move(oss).str();
    }



    cl7::u8osstream& Generator::_write_json(cl7::u8osstream& oss, const Json& json, size_t depth) const
    {
        if (json.is_null()) return oss << u8"null";
        if (json.is_true()) return oss << u8"true";
        if (json.is_false()) return oss << u8"false";
        if (json.is_string()) return _write_string(oss, json.as_string());
        if (json.is_integer()) return oss << json.as_integer();
        if (json.is_unsigned()) return oss << json.as_unsigned();
        if (json.is_decimal()) return _write_decimal(oss, json.as_decimal());

        if (json.is_array())
        {
            const array_t& array = json.as_array();
            if (array.empty()) return oss << u8"[]";
            oss << u8"[";
            for (size_t i = 0; i < array.size(); ++i)
            {
                _start_item(oss, depth + 1);
                _write_json(oss, array[i], depth + 1);
                _end_item(oss, i, array.size());
            }
            _start_item(oss, depth);
            oss << u8"]";
            return oss;
        }

        if (json.is_object())
        {
            const object_t& object = json.as_object();
            if (object.empty()) return oss << u8"{}";
            oss << u8"{";
            size_t i = 0;
            for (const auto& p : object)
            {
                _start_item(oss, depth + 1);
                if (_format.allow_unquoted_keys && util::Validator::is_valid_unquoted_key(p.first))
                    oss << p.first;
                else
                    _write_string(oss, p.first);
                oss.put(u8':');
                if (_format.style != Format::Style::SingleLine || !_format.single_line_options.compact)
                    oss.put(u8' ');
                _write_json(oss, p.second, depth + 1);
                _end_item(oss, i, object.size());
                ++i;
            }
            _start_item(oss, depth);
            oss << u8"}";
            return oss;
        }

        assert(false);
        return oss;
    }

    cl7::u8osstream& Generator::_write_string(cl7::u8osstream& oss, cl7::u8string_view string) const
    {
        if (!_format.allow_single_quotes)
            return _write_string(oss, string, u8'"');

        size_t single_quotes = 0;
        size_t double_quotes = 0;
        for (const auto& ch : string)
        {
            if (ch == u8'\'') ++single_quotes;
            if (ch == u8'"') ++double_quotes;
        }

        cl7::u8char_t quote_char = single_quotes < double_quotes ? u8'\'' : u8'"';
        return _write_string(oss, string, quote_char);
    }

    cl7::u8osstream& Generator::_write_string(cl7::u8osstream& oss, cl7::u8string_view string, cl7::u8char_t quote_char) const
    {
        assert(quote_char == u8'"' || quote_char == u8'\'');

        oss << quote_char;

        util::Escaper escaper{get_diagnostics()->get_log_context()};
        escaper.escape_string(oss, string, {.quote_char = static_cast<util::Escaper::QuoteChar>(quote_char), .escape_unicode = _format.escape_unicode});

        return oss << quote_char;
    }

    cl7::u8osstream& Generator::_write_decimal(cl7::u8osstream& oss, decimal_t decimal) const
    {
        if (std::isnan(decimal) || std::isinf(decimal))
        {
            switch (_format.float_policy)
            {
            case Format::FloatPolicy::ReplaceWithNull:
                return oss << u8"null";
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

    void Generator::_start_item(cl7::u8osstream& oss, size_t depth) const
    {
        if (_format.style != Format::Style::MultiLine)
            return;

        _put_line_ending(oss);

        if (_format.multi_line_options.indentation == Format::MultiLineOptions::Indentation::Tabs)
            for (size_t i = 0; i < depth; ++i)
                oss.put(u8'\t');
        else // => spaces
            for (size_t i = 0; i < depth * static_cast<unsigned>(_format.multi_line_options.indentation); ++i)
                oss.put(u8' ');
    }

    void Generator::_end_item(cl7::u8osstream& oss, size_t index, size_t count) const
    {
        assert(index < count);
        const bool is_last = index == count - 1;

        if (is_last)
        {
            if (_format.style == Format::Style::MultiLine && _format.multi_line_options.add_trailing_commas)
                oss.put(u8',');
        }
        else // => !is_last
        {
            oss.put(u8',');
            if (_format.style == Format::Style::SingleLine && !_format.single_line_options.compact)
                oss.put(u8' ');
        }
    }

    void Generator::_put_line_ending(cl7::u8osstream& oss) const
    {
        assert(_format.style == Format::Style::MultiLine);

        switch (_format.multi_line_options.line_ending)
        {
        case Format::MultiLineOptions::LineEnding::CRLF:
            oss << "\r\n";
            break;
        case Format::MultiLineOptions::LineEnding::LF:
            oss << '\n';
            break;
        case Format::MultiLineOptions::LineEnding::CR:
            oss << '\r';
            break;
        }
    }



} // namespace dl7::json::detail
