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
    static constexpr Format DEFAULT_FORMAT = {};



    /**
     * Generates a string representing the given `Yaml` object based on the specified
     * format.
     */
    static cl7::u8string to_string(const Yaml& yaml, const Format& format = DEFAULT_FORMAT);

}; // class YamlWriter



} // namespace dl7::yaml

#endif // DL7_YAML_YAMLWRITER_H
