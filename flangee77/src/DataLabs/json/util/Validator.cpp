#include "Validator.h"

#include "../detail/IdentifierMatcher.h"



namespace dl7::json::util {



    /**
     * Checks whether the specified string is a valid identifier for an unquoted
     * object key. Not all conditions of JavaScript's identifier naming rules are
     * considered, as with JSON5, for example. We allow identifiers that begin with
     * a letter (A-Z or a-z), an underscore, or a dollar sign; consist of letters,
     * digits, underscores, and/or dollar signs; and do not correspond to any
     * reserved keyword (as per the ECMAScript specification).
     */
    bool Validator::is_valid_unquoted_key(cl7::u8string_view key)
    {
        return !key.empty() && detail::IdentifierMatcher{}(key) == key.size();
    }



} // namespace dl7::json::util
