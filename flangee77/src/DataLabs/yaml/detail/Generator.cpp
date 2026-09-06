#include "Generator.h"

#include "../util/Schema.h"

#include <DataLabs/json/util/Escaper.h>

#include <CoreLabs/text/format.h>

#include <algorithm>
#include <cmath>
#include <ranges>



namespace dl7::yaml::detail {



namespace {

    bool _is_space(cl7::u8char_t ch) { return ch == u8' ' || ch == u8'\t'; }

    bool _is_control(cl7::u8char_t ch) { return ch < 0x20 || ch == 0x7f; }

    /**
     * Returns true if the given character starts a node of its own, and can
     * therefore not start a plain scalar.
     */
    bool _is_leading_indicator(cl7::u8char_t ch)
    {
        switch (ch)
        {
        case u8'-': case u8'?': case u8':': case u8',':
        case u8'[': case u8']': case u8'{': case u8'}':
        case u8'#': case u8'&': case u8'*': case u8'!':
        case u8'|': case u8'>': case u8'\'': case u8'"':
        case u8'%': case u8'@': case u8'`':
            return true;
        default:
            return false;
        }
    }

} // namespace



    Generator::Generator(syntax::Diagnostics* diagnostics, const Format& format)
        : AstProcessor(diagnostics)
        , _format(format)
    {
    }



    cl7::u8string Generator::process(const Yaml& yaml)
    {
        cl7::u8osstream oss;

        // Flow style is a single line by definition, document markers and all the
        // rest of the block-style layout being beside the point there.
        if (_format.style == Format::Style::Flow)
        {
            _write_flow_node(oss, yaml, false);
            return std::move(oss).str();
        }

        if (_format.explicit_document_start)
        {
            oss << u8"---";
            _write_newline(oss);
        }

        if (_is_block_style(yaml))
        {
            if (yaml.is_mapping())
                _write_block_mapping(oss, yaml.as_mapping(), 0, false);
            else
                _write_block_sequence(oss, yaml.as_sequence(), 0, false);
        }
        else
        {
            _write_inline_value(oss, yaml, 1);
        }

        if (_format.explicit_document_end)
        {
            oss << u8"...";
            _write_newline(oss);
        }

        if (_format.add_empty_line)
            _write_newline(oss);

        return std::move(oss).str();
    }



    /**
     * Writes a block mapping, one entry per line, every line indented by `depth`
     * levels. With `inline_first_line`, the first entry's indentation is taken to
     * have been written already, which is what turns `-` and a mapping into the
     * compact `- key: value`.
     */
    void Generator::_write_block_mapping(cl7::u8osstream& oss, const mapping_t& mapping, size_t depth, bool inline_first_line) const
    {
        assert(!mapping.empty());

        bool first = true;

        for (const auto& p : mapping)
        {
            if (!first || !inline_first_line)
                _write_indentation(oss, depth);
            first = false;

            _write_key(oss, p.first, false);
            oss << u8':';

            const Yaml& value = p.second;

            if (_is_block_style(value))
            {
                _write_newline(oss);

                if (value.is_mapping())
                {
                    _write_block_mapping(oss, value.as_mapping(), depth + 1, false);
                }
                else
                {
                    // A block sequence may sit at its parent key's indentation.
                    const size_t sequence_depth = _format.sequence_indentation == Format::SequenceIndentation::OneLevel ? depth + 1 : depth;
                    _write_block_sequence(oss, value.as_sequence(), sequence_depth, false);
                }

                continue;
            }

            if (_writes_nothing(value))
            {
                _write_newline(oss);
                continue;
            }

            oss << u8' ';
            _write_inline_value(oss, value, depth + 1);
        }
    }

    /**
     * Writes a block sequence, one entry per line. See `_write_block_mapping` for
     * `depth` and `inline_first_line`.
     */
    void Generator::_write_block_sequence(cl7::u8osstream& oss, const sequence_t& sequence, size_t depth, bool inline_first_line) const
    {
        assert(!sequence.empty());

        bool first = true;

        for (const auto& entry : sequence)
        {
            if (!first || !inline_first_line)
                _write_indentation(oss, depth);
            first = false;

            oss << u8'-';

            if (_is_block_style(entry))
            {
                // Compact notation: the entry starts on the dash's own line, which
                // is why the dash is padded out to a full indentation step.
                oss << cl7::u8string(static_cast<size_t>(_format.indentation) - 1, u8' ');

                if (entry.is_mapping())
                    _write_block_mapping(oss, entry.as_mapping(), depth + 1, true);
                else
                    _write_block_sequence(oss, entry.as_sequence(), depth + 1, true);

                continue;
            }

            if (_writes_nothing(entry))
            {
                _write_newline(oss);
                continue;
            }

            oss << u8' ';
            _write_inline_value(oss, entry, depth + 1);
        }
    }

    /**
     * Writes what is left of a line once its `key:` or `-` is out of the way: a
     * scalar or an empty collection, plus the line ending. A multi-line string may
     * become a block scalar instead, the content lines of which are indented by
     * `content_depth` levels.
     */
    void Generator::_write_inline_value(cl7::u8osstream& oss, const Yaml& yaml, size_t content_depth) const
    {
        if (yaml.is_string() && _fits_block_scalar(yaml.as_string()))
        {
            _write_block_scalar(oss, yaml.as_string(), content_depth);
            return;
        }

        _write_flow_node(oss, yaml, false);
        _write_newline(oss);
    }

    void Generator::_write_block_scalar(cl7::u8osstream& oss, cl7::u8string_view string, size_t content_depth) const
    {
        auto lines = _split_lines(string);

        size_t trailing_line_breaks = 0;
        while (!lines.empty() && lines.back().empty())
        {
            lines.pop_back();
            ++trailing_line_breaks;
        }

        cl7::u8string header{u8"|"};

        // Without an explicit indicator, the content's indentation would be read
        // off its first line, swallowing whatever that line starts with.
        if (!lines.empty() && !lines.front().empty() && _is_space(lines.front().front()))
            header += static_cast<cl7::u8char_t>(u8'0' + static_cast<unsigned>(_format.indentation));

        // A single trailing line break is what clipping restores anyway. Having
        // none of them calls for stripping, having more than one for keeping.
        if (trailing_line_breaks == 0)
            header += u8'-';
        else if (trailing_line_breaks > 1)
            header += u8'+';

        oss << header;
        _write_newline(oss);

        // Everything but the last line break belongs to the content.
        // The last one is what the chomping indicator takes care of.
        for (size_t i = 0; i < lines.size() + (trailing_line_breaks > 1 ? trailing_line_breaks - 1 : 0); ++i)
        {
            const auto line = i < lines.size() ? lines[i] : cl7::u8string_view{};

            if (!line.empty())
            {
                _write_indentation(oss, content_depth);
                oss << line;
            }

            _write_newline(oss);
        }
    }



    /**
     * Writes a node in flow style, i.e., on a single line. `in_flow` says whether
     * the node sits inside a flow collection, where a scalar has more characters to
     * watch out for.
     */
    cl7::u8osstream& Generator::_write_flow_node(cl7::u8osstream& oss, const Yaml& yaml, bool in_flow) const
    {
        if (yaml.is_sequence())
        {
            const sequence_t& sequence = yaml.as_sequence();

            oss << u8'[';
            for (size_t i = 0; i < sequence.size(); ++i)
            {
                if (i > 0)
                    oss << u8", ";
                _write_flow_node(oss, sequence[i], true);
            }
            return oss << u8']';
        }

        if (yaml.is_mapping())
        {
            const mapping_t& mapping = yaml.as_mapping();

            oss << u8'{';
            size_t i = 0;
            for (const auto& p : mapping)
            {
                if (i++ > 0)
                    oss << u8", ";
                _write_key(oss, p.first, true);
                oss << u8": ";
                _write_flow_node(oss, p.second, true);
            }
            return oss << u8'}';
        }

        return _write_scalar(oss, yaml, in_flow);
    }

    cl7::u8osstream& Generator::_write_scalar(cl7::u8osstream& oss, const Yaml& yaml, bool in_flow) const
    {
        switch (yaml.get_type())
        {
        case Yaml::Type::Null:
            return _write_null(oss, in_flow);
        case Yaml::Type::Boolean:
            return oss << (yaml.is_true() ? u8"true" : u8"false");
        case Yaml::Type::Integer:
            return oss << cl7::text::format::to_string(yaml.as_integer());
        case Yaml::Type::Float:
            return _write_float(oss, yaml.as_float());
        case Yaml::Type::String:
            return _write_string(oss, yaml.as_string(), in_flow);
        default:
            assert(false);
            return oss;
        }
    }

    cl7::u8osstream& Generator::_write_null(cl7::u8osstream& oss, bool in_flow) const
    {
        switch (_format.null_representation)
        {
        case Format::NullRepresentation::Tilde:
            return oss << u8'~';
        case Format::NullRepresentation::Empty:
            // An empty scalar inside a flow collection is indistinguishable
            // from no entry at all, so there it is spelled out regardless.
            if (!in_flow)
                return oss;
            break;
        case Format::NullRepresentation::Null:
            break;
        }

        return oss << u8"null";
    }

    cl7::u8osstream& Generator::_write_float(cl7::u8osstream& oss, float_t number) const
    {
        if (std::isnan(number) || std::isinf(number))
        {
            switch (_format.float_policy)
            {
            case Format::FloatPolicy::ReplaceWithNull:
                return oss << u8"null";
            case Format::FloatPolicy::ReplaceWithZero:
                return oss << u8"0.0";
            case Format::FloatPolicy::Native:
                if (std::isnan(number))
                    return oss << u8".nan";
                return oss << (std::signbit(number) ? u8"-.inf" : u8".inf");
            }
        }

        return oss << cl7::text::format::to_string(number, 1);
    }

    cl7::u8osstream& Generator::_write_key(cl7::u8osstream& oss, cl7::u8string_view key, bool in_flow) const
    {
        return _write_string(oss, key, in_flow);
    }

    cl7::u8osstream& Generator::_write_string(cl7::u8osstream& oss, cl7::u8string_view string, bool in_flow) const
    {
        if (_format.scalar_quoting == Format::ScalarQuoting::WhereNecessary && !_format.escape_unicode && !_needs_quoting(string, in_flow))
            return oss << string;

        // Single quotes can hold anything but a line break or a control character,
        // and they escape nothing but themselves.
        const bool single_quoted = _format.scalar_quoting == Format::ScalarQuoting::SingleQuoted
            && !_format.escape_unicode
            && std::ranges::none_of(string, _is_control);

        if (!single_quoted)
            return _write_quoted_string(oss, string);

        oss << u8'\'';
        for (const auto ch : string)
        {
            oss << ch;
            if (ch == u8'\'')
                oss << ch;
        }
        return oss << u8'\'';
    }

    cl7::u8osstream& Generator::_write_quoted_string(cl7::u8osstream& oss, cl7::u8string_view string) const
    {
        // Every escape sequence JSON knows is one YAML knows,
        // so escaping for the one is escaping for the other.
        oss << u8'"';
        json::util::Escaper{get_diagnostics()->get_log_context()}.escape_string(oss, string, {.escape_unicode = _format.escape_unicode});
        return oss << u8'"';
    }

    cl7::u8osstream& Generator::_write_indentation(cl7::u8osstream& oss, size_t depth) const
    {
        return oss << cl7::u8string(depth * static_cast<size_t>(_format.indentation), u8' ');
    }

    cl7::u8osstream& Generator::_write_newline(cl7::u8osstream& oss) const
    {
        switch (_format.line_ending)
        {
        case Format::LineEnding::CRLF:
            return oss << u8"\r\n";
        case Format::LineEnding::CR:
            return oss << u8"\r";
        case Format::LineEnding::LF:
        default:
            return oss << u8"\n";
        }
    }



    /**
     * Returns true if the given string cannot be written as a plain scalar without
     * being read back as something else: a different type, a comment, or another
     * node altogether.
     */
    bool Generator::_needs_quoting(cl7::u8string_view string, bool in_flow) const
    {
        if (string.empty())
            return true;

        // A plain scalar has to read back as the very string it was written from.
        if (!util::Schema::resolves_to_string(string))
            return true;

        // A document marker is not a scalar, however much it looks like one.
        if (string == u8"---" || string == u8"...")
            return true;

        if (_is_space(string.front()) || _is_space(string.back()))
            return true;

        if (_is_leading_indicator(string.front()))
            return true;

        for (size_t i = 0; i < string.size(); ++i)
        {
            const auto ch = string[i];

            if (_is_control(ch))
                return true;

            // A `:` separates a key from its value where whitespace or the end of
            // the scalar follows, and a `#` starts a comment where it follows
            // whitespace.
            if (ch == u8':' && (i + 1 == string.size() || _is_space(string[i + 1])))
                return true;
            if (ch == u8'#' && i > 0 && _is_space(string[i - 1]))
                return true;

            if (!in_flow)
                continue;

            // Inside a flow collection, its own punctuation ends a plain scalar.
            switch (ch)
            {
            case u8',': case u8'[': case u8']': case u8'{': case u8'}':
                return true;
            case u8':':
                if (i + 1 < string.size())
                {
                    switch (string[i + 1])
                    {
                    case u8',': case u8']': case u8'}':
                        return true;
                    default:
                        break;
                    }
                }
                break;
            default:
                break;
            }
        }

        return false;
    }

    /**
     * Returns true if the given string is better off as a literal block scalar than
     * as a quoted one-liner, which takes a multi-line string whose lines survive
     * indentation unchanged.
     */
    bool Generator::_fits_block_scalar(cl7::u8string_view string) const
    {
        if (!_format.use_block_scalars || _format.escape_unicode)
            return false;
        if (_format.scalar_quoting != Format::ScalarQuoting::WhereNecessary)
            return false;
        if (string.find(u8'\n') == cl7::u8string_view::npos)
            return false;

        bool has_content = false;

        for (const auto& line : _split_lines(string))
        {
            if (line.empty())
                continue;

            // Trailing whitespace would not survive being read back, and neither
            // would a control character (a lone carriage return, most notably).
            if (_is_space(line.back()))
                return false;
            if (std::ranges::any_of(line, _is_control))
                return false;

            has_content = true;
        }

        return has_content;
    }

    /**
     * Splits the given string at its line breaks. A trailing line break yields a
     * trailing empty piece, which is what the chomping indicator is derived from.
     */
    std::vector<cl7::u8string_view> Generator::_split_lines(cl7::u8string_view string)
    {
        std::vector<cl7::u8string_view> lines;

        size_t begin = 0;
        for (size_t i = 0; i <= string.size(); ++i)
        {
            if (i < string.size() && string[i] != u8'\n')
                continue;

            lines.push_back(string.substr(begin, i - begin));
            begin = i + 1;
        }

        return lines;
    }

    /**
     * Returns true if the given value is written as nothing at all, which an empty
     * null is, and which the space before it then has to account for.
     */
    bool Generator::_writes_nothing(const Yaml& yaml) const
    {
        return yaml.is_null() && _format.null_representation == Format::NullRepresentation::Empty;
    }

    bool Generator::_is_block_style(const Yaml& yaml)
    {
        // An empty collection has no block representation of its own.
        return yaml.is_structured() && !yaml.is_empty();
    }



} // namespace dl7::yaml::detail
