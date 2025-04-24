#ifndef DL7_JSON_UTIL_VALIDATOR_H
#define DL7_JSON_UTIL_VALIDATOR_H

#include <CoreLabs/string.h>



namespace dl7::json::util {



/**
 * A utility class for validating JSON-specific components.
 */
class Validator
{

public:
    /**
     * Checks whether the specified string is a valid identifier for an unquoted
     * object key. Not all conditions of JavaScript's identifier naming rules are
     * considered, as with JSON5, for example. We allow identifiers that begin with
     * a letter (A-Z or a-z), an underscore, or a dollar sign; consist of letters,
     * digits, underscores, and/or dollar signs; and do not correspond to any
     * reserved keyword (as per the ECMAScript specification).
     */
    static bool is_valid_unquoted_key(cl7::u8string_view key);

}; // class Validator



} // namespace dl7::json::util

#endif // DL7_JSON_UTIL_VALIDATOR_H
