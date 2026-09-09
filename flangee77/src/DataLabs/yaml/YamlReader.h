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
     * How deeply nodes may be nested by default. Parsing descends through several
     * stack frames per level, so a document nested deeply enough would run out of
     * stack before it ran out of tokens, anything remotely sane stays far below
     * this.
     */
    static constexpr size_t DEFAULT_MAX_NESTING_DEPTH = 200;



    explicit YamlReader(size_t max_nesting_depth = DEFAULT_MAX_NESTING_DEPTH);



    /**
     * Parses a UTF-8 encoded YAML string and returns a `Yaml` object. Whatever
     * there is to complain about ends up in the diagnostics, which are cleared
     * beforehand.
     */
    Yaml parse(cl7::u8string_view source);

    /**
     * Returns how deeply nodes may be nested before parsing gives up.
     */
    size_t get_max_nesting_depth() const noexcept { return _max_nesting_depth; }

    /**
     * Returns the diagnostics of the most recent `parse` call.
     */
    const syntax::Diagnostics& get_diagnostics() const noexcept { return _diagnostics; }



private:
    size_t _max_nesting_depth;
    syntax::Diagnostics _diagnostics;

}; // class YamlReader



} // namespace dl7::yaml

#endif // DL7_YAML_YAMLREADER_H
