#include "JsonWriter.h"

#include <CoreLabs/sstream.h>

#include <cmath>
#include <format>



namespace dl7::json {



    static void _put_line_ending(cl7::u8osstream& oss, const JsonWriter::Format& format)
    {
        assert(format.style == JsonWriter::Format::Style::MultiLine);

        switch (format.multi_line_options.line_ending)
        {
        case JsonWriter::Format::MultiLineOptions::LineEnding::CRLF:
            oss << "\r\n";
            break;
        case JsonWriter::Format::MultiLineOptions::LineEnding::LF:
            oss << '\n';
            break;
        case JsonWriter::Format::MultiLineOptions::LineEnding::CR:
            oss << '\r';
            break;
        }
    }

    static void _start_item(cl7::u8osstream& oss, size_t depth, const JsonWriter::Format& format)
    {
        if (format.style != JsonWriter::Format::Style::MultiLine)
            return;

        _put_line_ending(oss, format);

        if (format.multi_line_options.indentation == JsonWriter::Format::MultiLineOptions::Indentation::Tabs)
            for (size_t i = 0; i < depth; ++i)
                oss.put(u8'\t');
        else // => spaces
            for (size_t i = 0; i < depth * static_cast<unsigned>(format.multi_line_options.indentation); ++i)
                oss.put(u8' ');
    }

    static void _end_item(cl7::u8osstream& oss, size_t index, size_t count, const JsonWriter::Format& format)
    {
        assert(index < count);
        const bool is_last = index == count - 1;

        if (is_last)
        {
            if (format.style == JsonWriter::Format::Style::MultiLine && format.multi_line_options.add_trailing_commas)
                oss.put(u8',');
        }
        else // => !is_last
        {
            oss.put(u8',');
            if (format.style == JsonWriter::Format::Style::SingleLine && !format.single_line_options.compact)
                oss.put(u8' ');
        }
    }

    static cl7::u8osstream& _write_string(cl7::u8osstream& oss, cl7::u8string_view string, cl7::u8char_type quote_char, const JsonWriter::Format& format)
    {
        oss << quote_char;
        for (const auto& ch : string)
        {
            switch (ch)
            {
            case u8'\b': oss << u8"\\b"; break;
            case u8'\t': oss << u8"\\t"; break;
            case u8'\n': oss << u8"\\n"; break;
            case u8'\f': oss << u8"\\f"; break;
            case u8'\r': oss << u8"\\r"; break;

            case u8'"':
            case u8'\'':
                if (ch == quote_char)
                    oss << u8'\\';
                oss << quote_char;
                break;

            case u8'\\': oss << u8"\\\\"; break;

            default:
                if (ch < 0x20)
                    oss << std::format("\\u{:04X}", static_cast<unsigned>(ch));
                else
                    oss << ch;
            }
        }
        oss << quote_char;
        return oss;
    }

    static cl7::u8osstream& _write_string(cl7::u8osstream& oss, cl7::u8string_view string, const JsonWriter::Format& format)
    {
        if (!format.allow_single_quotes)
            return _write_string(oss, string, u8'"', format);

        size_t single_quotes = 0;
        size_t double_quotes = 0;
        for (const auto& ch : string)
        {
            if (ch == u8'\'') ++single_quotes;
            if (ch == u8'"') ++double_quotes;
        }

        cl7::u8char_type quote_char = single_quotes > double_quotes ? u8'\'' : u8'"';
        return _write_string(oss, string, quote_char, format);
    }

    static cl7::u8osstream& _write_decimal(cl7::u8osstream& oss, decimal_t decimal, const JsonWriter::Format& format)
    {
        decimal_t int_part;
        decimal_t frac_part = std::modf(decimal, &int_part);
        if (frac_part == decimal_t{0.0})
        {
            constexpr decimal_t min_int = std::numeric_limits<signed long>::min();
            constexpr decimal_t max_int = std::numeric_limits<signed long>::max();
            if (int_part >= min_int && int_part <= max_int)
                return oss << static_cast<signed long>(int_part) << u8".0";
            return oss << std::scientific << int_part;
        }

        return oss << std::defaultfloat << decimal;
    }

    static cl7::u8osstream& _write_json(cl7::u8osstream& oss, const Json& json, size_t depth, const JsonWriter::Format& format)
    {
        if (json.is_null()) return oss << u8"null";
        if (json.is_true()) return oss << u8"true";
        if (json.is_false()) return oss << u8"false";
        if (json.is_string()) return _write_string(oss, json.as_string(), format);
        if (json.is_integer()) return oss << json.as_integer();
        if (json.is_unsigned()) return oss << json.as_unsigned();
        if (json.is_decimal()) return _write_decimal(oss, json.as_decimal(), format);

        if (json.is_array())
        {
            const array_t& array = json.as_array();
            if (array.empty()) return oss << u8"[]";
            oss << u8"[";
            for (size_t i = 0; i < array.size(); ++i)
            {
                _start_item(oss, depth + 1, format);
                _write_json(oss, array[i], depth + 1, format);
                _end_item(oss, i, array.size(), format);
            }
            _start_item(oss, depth, format);
            oss << u8"]";
            return oss;
        }

        if (json.is_object())
        {
            const object_t& object = json.as_object();
            if (object.empty()) return oss << u8"{}";
            oss << u8"[";
            size_t i = 0;
            for (const auto& p : object)
            {
                _start_item(oss, depth + 1, format);
                _write_string(oss, p.first, format);
                oss.put(u8':');
                if (format.style == JsonWriter::Format::Style::SingleLine && !format.single_line_options.compact)
                    oss.put(u8' ');
                _write_json(oss, p.second, depth + 1, format);
                _end_item(oss, i, object.size(), format);
                ++i;
            }
            _start_item(oss, depth, format);
            oss << u8"]";
            return oss;
        }

        assert(false);
        return oss;
    }



    cl7::u8string JsonWriter::to_string(const Json& json, const Format& format)
    {
        cl7::u8osstream oss;

        _write_json(oss, json, 0, format);

        if (format.style == JsonWriter::Format::Style::MultiLine && format.multi_line_options.add_empty_line && json.is_structured())
            _put_line_ending(oss, format);

        return std::move(oss).str();
    }



} // namespace dl7::json
