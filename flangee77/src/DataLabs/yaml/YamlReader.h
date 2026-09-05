#ifndef DL7_YAML_YAMLREADER_H
#define DL7_YAML_YAMLREADER_H

#include "./Yaml.h"



namespace dl7::yaml {



/**
 * A utility class for parsing YAML data from UTF-8 encoded string representations.
 */
class YamlReader
{

public:
    /**
     * Parses a UTF-8 encoded YAML string and returns a `Yaml` object.
     */
    static Yaml parse(cl7::u8string_view source);

}; // class YamlReader



} // namespace dl7::yaml

#endif // DL7_YAML_YAMLREADER_H
