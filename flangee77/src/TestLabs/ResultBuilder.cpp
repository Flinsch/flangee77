#include "ResultBuilder.h"



namespace tl7 {



    // #############################################################################
    // Methods
    // #############################################################################

    Result ResultBuilder::make_test_case_result(const Context& ctx, Result::Outcome outcome)
    {
        Result result;
        result.origin_type = Result::OriginType::TestCase;
        result.outcome = outcome;

        const Meta* meta = ctx.root_meta;
        assert( meta );

        result.context_meta = *meta;
        result.result_meta = *meta;

        return result;
    }

    Result ResultBuilder::make_check_result(const Context& ctx, const cl7::string_view& original_expression, const cl7::string_view& evaluated_expression, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome)
    {
        return _make_result( Result::OriginType::Check, ctx, original_expression, evaluated_expression, file_path, line_number, outcome );
    }

    Result ResultBuilder::make_presumption_result(const Context& ctx, const cl7::string_view& original_expression, const cl7::string_view& evaluated_expression, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome)
    {
        return _make_result( Result::OriginType::Presumption, ctx, original_expression, evaluated_expression, file_path, line_number, outcome );
    }

    Result ResultBuilder::make_assertion_result(const Context& ctx, const cl7::string_view& original_expression, const cl7::string_view& evaluated_expression, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome)
    {
        return _make_result( Result::OriginType::Assertion, ctx, original_expression, evaluated_expression, file_path, line_number, outcome );
    }

    Result ResultBuilder::make_exception_result(const Context& ctx, const cl7::string_view& message, const cl7::char_type* file_path, unsigned line_number)
    {
        return _make_result( Result::OriginType::Exception, ctx, message, file_path, line_number, Result::Outcome::Failure );
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    Result ResultBuilder::_make_result(Result::OriginType origin_type, const Context& ctx, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome)
    {
        Result result;
        result.origin_type = origin_type;
        result.outcome = outcome;

        result.data_string = ctx.subcases.get_data_string();

        const Meta* meta = ctx.subcases.get_current_meta();
        if ( !meta ) meta = ctx.root_meta;
        assert( meta );

        result.context_meta = *meta;

        result.result_meta.file_path = file_path;
        result.result_meta.line_number = line_number;
        result.result_meta.iteration_number = ctx.root_meta->iteration_number;

        return result;
    }

    Result ResultBuilder::_make_result(Result::OriginType origin_type, const Context& ctx, const cl7::string_view& stringification, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome)
    {
        Result result = _make_result( origin_type, ctx, file_path, line_number, outcome );
        result.result_meta.stringification = stringification;
        return result;
    }

    Result ResultBuilder::_make_result(Result::OriginType origin_type, const Context& ctx, const cl7::string_view& original_expression, const cl7::string_view& evaluated_expression, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome)
    {
        Result result = _make_result( origin_type, ctx, file_path, line_number, outcome );
        result.original_expression = original_expression;
        result.evaluated_expression = evaluated_expression;
        return result;
    }


} // namespace tl7
