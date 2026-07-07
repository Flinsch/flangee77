#include "Generator.h"

#include <DataLabs/json/util/Escaper.h>

#include <algorithm>
#include <ranges>



namespace dl7::toml::detail {



    Generator::Generator(syntax::Diagnostics* diagnostics, const Format& format)
        : AstProcessor(diagnostics)
        , _format(format)
    {
    }



    cl7::u8string Generator::process(const Toml& toml)
    {
        cl7::u8osstream oss;

        const table_t& root = toml.root();

        bool has_scalars = false;
        bool has_headers = false;
        for (const auto& value : root | std::views::values)
            ((value.is_table() || _is_array_of_tables(value)) ? has_headers : has_scalars) = true;

        // Only write an empty line at start of file (if configured at all) if there
        // are root-level key-value pairs or if no separate empty line is to be
        // inserted before the otherwise first table header (if such header exists).
        if (_format.empty_line_at_start_of_file && (has_scalars || (!_format.empty_line_before_table_header && has_headers)))
            _write_newline(oss);

        _write_table(oss, root, u8"");

        if (_format.add_empty_line && (has_scalars || has_headers))
            _write_newline(oss);

        return std::move(oss).str();
    }



    void Generator::_write_table(cl7::u8osstream& oss, const table_t& table, cl7::u8string_view name_prefix) const
    {
        // First pass: all direct key-value pairs (everything that is not written as
        // its own header block).
        for (const auto& p : table)
        {
            if (p.second.is_table() || _is_array_of_tables(p.second))
                continue;
            _write_key_value(oss, p.first, p.second);
        }

        // Second pass: sub-tables and arrays-of-tables as their own headers.
        for (const auto& p : table)
        {
            if (p.second.is_table())
                _write_subtable(oss, p.first, p.second.as_table(), name_prefix);
            else if (_is_array_of_tables(p.second))
                _write_array_of_tables(oss, p.first, p.second.as_array(), name_prefix);
        }
    }

    void Generator::_write_subtable(cl7::u8osstream& oss, cl7::u8string_view key, const table_t& sub_table, cl7::u8string_view name_prefix) const
    {
        cl7::u8osstream tmp;
        tmp << name_prefix;
        _write_key(tmp, key);
        const auto full_name = std::move(tmp).str();

        bool sub_has_scalars = false;
        bool sub_has_headers = false;
        for (const auto& value : sub_table | std::views::values)
            ((value.is_table() || _is_array_of_tables(value)) ? sub_has_headers : sub_has_scalars) = true;

        // Only write the header if the table has direct key-value pairs. Unless the
        // table is otherwise empty (a leaf without sub-tables), in which case the
        // header is still written; a table that only contains sub-tables is a
        // super-table and is implied by its children.
        if (sub_has_scalars || !sub_has_headers)
        {
            if (_format.empty_line_before_table_header)
                _write_newline(oss);

            oss << u8'[' << full_name << u8']';
            _write_newline(oss);

            if (_format.empty_line_after_table_header)
                _write_newline(oss);
        }

        _write_table(oss, sub_table, full_name + u8".");
    }

    void Generator::_write_array_of_tables(cl7::u8osstream& oss, cl7::u8string_view key, const array_t& array, cl7::u8string_view name_prefix) const
    {
        cl7::u8osstream tmp;
        tmp << name_prefix;
        _write_key(tmp, key);
        const auto full_name = std::move(tmp).str();

        // Each element gets its own `[[name]]` header (an array-of-tables cannot be a
        // mere super-table, so a header is always written, even for empty elements).
        for (const auto& element : array)
        {
            if (_format.empty_line_before_table_header)
                _write_newline(oss);

            oss << u8"[[" << full_name << u8"]]";
            _write_newline(oss);

            if (_format.empty_line_after_table_header)
                _write_newline(oss);

            _write_table(oss, element.as_table(), full_name + u8".");
        }
    }



    void Generator::_write_key_value(cl7::u8osstream& oss, cl7::u8string_view key, const Value& value) const
    {
        _write_key(oss, key);

        if (_format.space_around_equals_sign == Format::SpaceAroundEqualsSign::Before || _format.space_around_equals_sign == Format::SpaceAroundEqualsSign::Both)
            oss << u8' ';

        oss << u8'=';

        if (_format.space_around_equals_sign == Format::SpaceAroundEqualsSign::After || _format.space_around_equals_sign == Format::SpaceAroundEqualsSign::Both)
            oss << u8' ';

        _write_value(oss, value);
        _write_newline(oss);
    }



    cl7::u8osstream& Generator::_write_key(cl7::u8osstream& oss, cl7::u8string_view key) const
    {
        if (_is_bare_key(key))
            return oss << key;

        return _write_string(oss, key);
    }

    cl7::u8osstream& Generator::_write_value(cl7::u8osstream& oss, const Value& value) const
    {
        if (value.is_string())
            return _write_string(oss, value.as_string());

        // Numbers, booleans, dates/times, and (inline) arrays/tables share the
        // inline representation of the value itself.
        return oss << value.to_string();
    }

    cl7::u8osstream& Generator::_write_string(cl7::u8osstream& oss, cl7::u8string_view string) const
    {
        oss << u8'"';
        json::util::Escaper{get_diagnostics()->get_log_context()}.escape_string(oss, string, {.escape_unicode = _format.escape_unicode});
        return oss << u8'"';
    }



    cl7::u8osstream& Generator::_write_newline(cl7::u8osstream& oss) const
    {
        switch (_format.line_ending)
        {
        case Format::LineEnding::CRLF:
            oss << "\r\n";
            break;
        case Format::LineEnding::LF:
            oss << '\n';
            break;
        case Format::LineEnding::CR:
            oss << '\r';
            break;
        }

        return oss;
    }



    bool Generator::_is_bare_key(cl7::u8string_view key)
    {
        if (key.empty())
            return false;

        return std::ranges::all_of(key, [](cl7::u8char_t ch) {
            return
                (ch >= u8'A' && ch <= u8'Z') ||
                (ch >= u8'a' && ch <= u8'z') ||
                (ch >= u8'0' && ch <= u8'9') ||
                ch == u8'-' || ch == u8'_';
        });
    }

    bool Generator::_is_array_of_tables(const Value& value)
    {
        // An empty array cannot be expressed as `[[name]]` headers, so it is written
        // inline (as `name = []`) instead.
        if (!value.is_array() || value.as_array().empty())
            return false;

        return std::ranges::all_of(value.as_array(), [](const Value& element) { return element.is_table(); });
    }



} // namespace dl7::toml::detail
