#ifndef DL7_YAML_YAMLWRITER_H
#define DL7_YAML_YAMLWRITER_H

#include "./Yaml.h"
#include "./Format.h"

#include <DataLabs/syntax/Diagnostics.h>


namespace dl7::yaml {



/**
 * Serializes `Yaml` objects into UTF-8 encoded string representations, keeping
 * whatever it had to complain about along the way.
 */
class YamlWriter
{

public:
    static constexpr Format DEFAULT_BLOCK_FORMAT = {
        .style = Format::Style::Block,
        .indentation = Format::Indentation::Spaces2,
        .line_ending = Format::LineEnding::LF,
        .sequence_indentation = Format::SequenceIndentation::None,
        .scalar_quoting = Format::ScalarQuoting::WhereNecessary,
        .null_representation = Format::NullRepresentation::Null,
        .float_policy = Format::FloatPolicy::Native,
        .use_block_scalars = true,
        .explicit_document_start = false,
        .explicit_document_end = false,
        .escape_unicode = false,
        .add_empty_line = false,
    };
    static constexpr Format DEFAULT_FLOW_FORMAT = {
        .style = Format::Style::Flow,
        .indentation = Format::Indentation::Spaces2,
        .line_ending = Format::LineEnding::LF,
        .sequence_indentation = Format::SequenceIndentation::None,
        .scalar_quoting = Format::ScalarQuoting::WhereNecessary,
        .null_representation = Format::NullRepresentation::Null,
        .float_policy = Format::FloatPolicy::Native,
        .use_block_scalars = false,
        .explicit_document_start = false,
        .explicit_document_end = false,
        .escape_unicode = false,
        .add_empty_line = false,
    };
    static constexpr Format DEFAULT_FORMAT = DEFAULT_BLOCK_FORMAT;



    explicit YamlWriter(const Format& format = DEFAULT_FORMAT);



    /**
     * Generates a string representing the given YAML node/document based on the
     * format this writer was given. Whatever there is to complain about ends
     * up in the diagnostics, which are cleared beforehand.
     */
    cl7::u8string to_string(const Yaml& yaml);

    /**
     * Returns the format the strings are generated according to.
     */
    const Format& get_format() const noexcept { return _format; }

    /**
     * Returns the diagnostics of the most recent `to_string` call.
     */
    const syntax::Diagnostics& get_diagnostics() const noexcept { return _diagnostics; }



private:
    Format _format;
    syntax::Diagnostics _diagnostics;

}; // class YamlWriter



} // namespace dl7::yaml

#endif // DL7_YAML_YAMLWRITER_H
