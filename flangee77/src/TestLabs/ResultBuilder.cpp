#include "ResultBuilder.h"



namespace tl7 {



    // #############################################################################
    // Methods
    // #############################################################################

    Result ResultBuilder::make_test_case_result(const Meta& meta, Result::Outcome outcome)
    {
        return Result( Result::OriginType::TestCase, outcome, meta.stringification, meta.file_path, meta.line_number );
    }

    Result ResultBuilder::make_check_result(const cl7::string_view& original_expression, const cl7::string_view& evaluated_expression, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome)
    {
        return Result( Result::OriginType::Check, outcome, original_expression, evaluated_expression, file_path, line_number );
    }

    Result ResultBuilder::make_presumption_result(const cl7::string_view& original_expression, const cl7::string_view& evaluated_expression, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome)
    {
        return Result( Result::OriginType::Presumption, outcome, original_expression, evaluated_expression, file_path, line_number );
    }

    Result ResultBuilder::make_assertion_result(const cl7::string_view& original_expression, const cl7::string_view& evaluated_expression, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome)
    {
        return Result( Result::OriginType::Assertion, outcome, original_expression, evaluated_expression, file_path, line_number );
    }

    Result ResultBuilder::make_exception_result(const cl7::string_view& message, const cl7::char_type* file_path, unsigned line_number)
    {
        return Result( Result::OriginType::Exception, Result::Outcome::Failure, message, file_path, line_number );
    }


} // namespace tl7
