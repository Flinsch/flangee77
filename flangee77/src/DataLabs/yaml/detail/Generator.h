#ifndef DL7_YAML_DETAIL_GENERATOR_H
#define DL7_YAML_DETAIL_GENERATOR_H
#include <DataLabs/syntax/AstProcessor.h>

#include "../Yaml.h"
#include "../Format.h"

#include <CoreLabs/sstream.h>

#include <vector>



namespace dl7::yaml::detail {



class Generator
    : public syntax::AstProcessor<Yaml, cl7::u8string>
{

public:
    explicit Generator(syntax::Diagnostics* diagnostics, const Format& format);



    cl7::u8string process(const Yaml& yaml) override;



private:
    /**
     * Writes a block mapping, one entry per line, every line indented by `depth`
     * levels. With `inline_first_line`, the first entry's indentation is taken to
     * have been written already, which is what turns `-` and a mapping into the
     * compact `- key: value`.
     */
    void _write_block_mapping(cl7::u8osstream& oss, const mapping_t& mapping, size_t depth, bool inline_first_line) const;

    /**
     * Writes a block sequence, one entry per line. See `_write_block_mapping` for
     * `depth` and `inline_first_line`.
     */
    void _write_block_sequence(cl7::u8osstream& oss, const sequence_t& sequence, size_t depth, bool inline_first_line) const;

    /**
     * Writes what is left of a line once its `key:` or `-` is out of the way: a
     * scalar or an empty collection, plus the line ending. A multi-line string may
     * become a block scalar instead, the content lines of which are indented by
     * `content_depth` levels.
     */
    void _write_inline_value(cl7::u8osstream& oss, const Yaml& yaml, size_t content_depth) const;

    /**
     * Writes a literal block scalar, i.e., the `|` header and the content lines
     * that follow it, indented by `content_depth` levels.
     */
    void _write_block_scalar(cl7::u8osstream& oss, cl7::u8string_view string, size_t content_depth) const;

    /**
     * Writes a node in flow style, i.e., on a single line. `in_flow` says whether
     * the node sits inside a flow collection, where a scalar has more characters to
     * watch out for.
     */
    cl7::u8osstream& _write_flow_node(cl7::u8osstream& oss, const Yaml& yaml, bool in_flow) const;

    cl7::u8osstream& _write_scalar(cl7::u8osstream& oss, const Yaml& yaml, bool in_flow) const;
    cl7::u8osstream& _write_null(cl7::u8osstream& oss, bool in_flow) const;
    cl7::u8osstream& _write_float(cl7::u8osstream& oss, float_t number) const;
    cl7::u8osstream& _write_key(cl7::u8osstream& oss, cl7::u8string_view key, bool in_flow) const;
    cl7::u8osstream& _write_string(cl7::u8osstream& oss, cl7::u8string_view string, bool in_flow) const;
    cl7::u8osstream& _write_quoted_string(cl7::u8osstream& oss, cl7::u8string_view string) const;

    cl7::u8osstream& _write_indentation(cl7::u8osstream& oss, size_t depth) const;
    cl7::u8osstream& _write_newline(cl7::u8osstream& oss) const;

    /**
     * Returns true if the given string cannot be written as a plain scalar without
     * being read back as something else: a different type, a comment, or another
     * node altogether.
     */
    bool _needs_quoting(cl7::u8string_view string, bool in_flow) const;

    /**
     * Returns true if the given string is better off as a literal block scalar than
     * as a quoted one-liner, which takes a multi-line string whose lines survive
     * indentation unchanged.
     */
    bool _fits_block_scalar(cl7::u8string_view string) const;

    /**
     * Splits the given string at its line breaks. A trailing line break yields a
     * trailing empty piece, which is what the chomping indicator is derived from.
     */
    static std::vector<cl7::u8string_view> _split_lines(cl7::u8string_view string);

    /**
     * Returns true if the given value is written as nothing at all, which an empty
     * null is, and which the space before it then has to account for.
     */
    bool _writes_nothing(const Yaml& yaml) const;

    static bool _is_block_style(const Yaml& yaml);



    Format _format;

}; // class Generator



} // namespace dl7::yaml::detail

#endif // DL7_YAML_DETAIL_GENERATOR_H
