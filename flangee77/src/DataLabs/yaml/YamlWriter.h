#ifndef DL7_YAML_YAMLWRITER_H
#define DL7_YAML_YAMLWRITER_H

#include "./Yaml.h"
#include "./Format.h"


namespace dl7::yaml {



/**
 * A utility class for serializing `Yaml` objects into UTF-8 encoded string
 * representations.
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



    /**
     * Generates a string representing the given YAML node/document based on the
     * specified format.
     */
    static cl7::u8string to_string(const Yaml& yaml, const Format& format = DEFAULT_FORMAT);

}; // class YamlWriter



} // namespace dl7::yaml

#endif // DL7_YAML_YAMLWRITER_H
