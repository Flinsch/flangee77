#include "Result.h"



namespace tl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    Result::Result()
        : Result( OriginType::Unknown, Outcome::Failure, {}, TEXT(""), 0 )
    {
    }

    /**
     * Explicit constructor.
     */
    Result::Result(OriginType origin_type, Outcome outcome, const cl7::string_view& stringification, const cl7::char_type* file_path, unsigned line_number)
        : Result( origin_type, outcome, {}, {}, stringification, file_path, line_number )
    {
    }

    /**
     * Explicit constructor.
     */
    Result::Result(OriginType origin_type, Outcome outcome, const cl7::string_view& original_expression, const cl7::string_view& evaluated_expression, const cl7::char_type* file_path, unsigned line_number)
        : origin_type( origin_type ), outcome( outcome ), original_expression( original_expression ), evaluated_expression( evaluated_expression ), meta{ cl7::string(), file_path, line_number, 0 }
    {
    }

    /**
     * Explicit constructor.
     */
    Result::Result(OriginType origin_type, Outcome outcome, const cl7::string_view& original_expression, const cl7::string_view& evaluated_expression, const cl7::string_view& stringification, const cl7::char_type* file_path, unsigned line_number)
        : origin_type( origin_type ), outcome( outcome ), original_expression( original_expression ), evaluated_expression( evaluated_expression ), meta{ cl7::string(stringification), file_path, line_number, 0 }
    {
    }



} // namespace tl7
