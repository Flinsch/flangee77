#include "Result.h"



namespace tl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    Result::Result()
        : origin_type( OriginType::Unknown ), outcome( Outcome::Failure ), original_expression(), evaluated_expression(), data_string(), meta{ cl7::string(), TEXT(""), 0, 0 }
    {
    }

    /**
     * Explicit constructor.
     */
    Result::Result(OriginType origin_type, Outcome outcome, const cl7::string_view& stringification, const cl7::char_type* file_path, unsigned line_number, signed iteration_number)
        : origin_type( origin_type ), outcome( outcome ), original_expression(), evaluated_expression(), data_string(), meta{ stringification, file_path, line_number, iteration_number }
    {
    }

    /**
     * Explicit constructor.
     */
    Result::Result(OriginType origin_type, Outcome outcome, const cl7::string_view& original_expression, const cl7::string_view& evaluated_expression, const std::shared_ptr<cl7::string>& data_string, const cl7::char_type* file_path, unsigned line_number, signed iteration_number)
        : origin_type( origin_type ), outcome( outcome ), original_expression( original_expression ), evaluated_expression( evaluated_expression ), data_string( data_string ), meta{ cl7::string(), file_path, line_number, iteration_number }
    {
    }



} // namespace tl7
