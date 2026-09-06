#ifndef DL7_YAML_UTIL_SCHEMA_H
#define DL7_YAML_UTIL_SCHEMA_H

#include "../Yaml.h"



namespace dl7::yaml::util {



/**
 * Resolves plain (unquoted) scalars according to the YAML 1.2 core schema, which is
 * what decides whether `null`, `true`, `0x1f`, or `.inf` is a value in its own
 * right or just a string.
 *
 * Reader and writer are two sides of the same rule and therefore share this: the
 * reader resolves what it reads, and the writer asks whether a string would survive
 * being written without quotes at all, which is the case exactly when the string
 * resolves back to itself.
 */
class Schema
{

public:
    /**
     * Resolves a plain scalar into the value it stands for: null, a boolean, an
     * integer, a floating-point number, or (when failing all of those) the string
     * itself.
     */
    static Yaml resolve(cl7::u8string_view text);

    /**
     * Returns true if the given text resolves to a string of the very same content,
     * i.e., writing that string as a plain scalar would not change its type.
     */
    static bool resolves_to_string(cl7::u8string_view text);

}; // class Schema



} // namespace dl7::yaml::util

#endif // DL7_YAML_UTIL_SCHEMA_H
