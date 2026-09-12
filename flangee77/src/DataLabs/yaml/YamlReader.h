#ifndef DL7_YAML_YAMLREADER_H
#define DL7_YAML_YAMLREADER_H

#include "./Yaml.h"

#include <DataLabs/syntax/Diagnostics.h>



namespace dl7::yaml {



/**
 * Parses YAML data from UTF-8 encoded string representations, keeping whatever it
 * had to complain about along the way.
 *
 * What it reads is a deliberate subset of YAML, cut to fit configuration files
 * rather than the specification: block mappings and sequences at any nesting, the
 * compact notation (`- key: value`), flow collections (`[...]`, `{...}`) spanning as
 * many lines as they like, literal and folded block scalars with their indentation
 * and chomping indicators, plain as well as single- and double-quoted scalars, plain
 * scalars folded across several lines, comments, and the YAML 1.2 core schema for
 * typing scalars (see `util::Schema`).
 *
 * The rest is deliberately left out. None of it is quietly mis-read, though: each
 * is recognized and reported, so that a document relying on it says so rather than
 * coming out wrong.
 * - Anchors, aliases (`&`, `*`), and merge keys (`<<`), which turn up in files
 *   written by tools rather than by hand. They end up as plain scalars.
 * - Tags (`!!str`) and explicit keys (`? `), likewise as plain scalars.
 * - Directives (`%YAML`, `%TAG`), whose lines are skipped.
 * - Every document of a stream but the first. The `---` and `...` markers are
 *   understood, what follows the first document is not read.
 *
 * Where the specification is stricter than a reader has to be, this one is lenient:
 * a tab in the indentation, an unexpected indentation, and a line that is no
 * key-value pair are all diagnosed, but none of them costs more than itself.
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
