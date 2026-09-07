#ifndef DL7_YAML_YAMLREADER_H
#define DL7_YAML_YAMLREADER_H

#include "./Yaml.h"

#include <DataLabs/syntax/Diagnostics.h>



namespace dl7::yaml {



/**
 * Parses YAML data from UTF-8 encoded string representations, keeping whatever it
 * had to complain about along the way.
 */
class YamlReader
{

public:
    /**
     * Parses a UTF-8 encoded YAML string and returns a `Yaml` object. Whatever
     * there is to complain about ends up in the diagnostics, which are cleared
     * beforehand.
     */
    Yaml parse(cl7::u8string_view source);

    /**
     * Returns the diagnostics of the most recent `parse` call.
     */
    const syntax::Diagnostics& get_diagnostics() const noexcept { return _diagnostics; }



private:
    syntax::Diagnostics _diagnostics;

}; // class YamlReader



} // namespace dl7::yaml

#endif // DL7_YAML_YAMLREADER_H
