#pragma once
#ifndef TL7_RESULTBUILDER_H
#define TL7_RESULTBUILDER_H

#include "./Result.h"
#include "./Context.h"



namespace tl7 {



class ResultBuilder
{

    // #############################################################################
    // Methods
    // #############################################################################
public:
    static Result make_test_case_result(const Context& ctx, Result::Outcome outcome);
    static Result make_check_result(const Context& ctx, cl7::string_view original_expression, cl7::string_view evaluated_expression, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome);
    static Result make_presumption_result(const Context& ctx, cl7::string_view original_expression, cl7::string_view evaluated_expression, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome);
    static Result make_assertion_result(const Context& ctx, cl7::string_view original_expression, cl7::string_view evaluated_expression, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome);
    static Result make_exception_result(const Context& ctx, cl7::string_view message, const cl7::char_type* file_path = nullptr, unsigned line_number = 0);



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    static Result _make_result(Result::OriginType origin_type, const Context& ctx, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome);
    static Result _make_result(Result::OriginType origin_type, const Context& ctx, cl7::string_view stringification, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome);
    static Result _make_result(Result::OriginType origin_type, const Context& ctx, cl7::string_view original_expression, cl7::string_view evaluated_expression, const cl7::char_type* file_path, unsigned line_number, Result::Outcome outcome);

}; // class ResultBuilder



} // namespace tl7

#endif // TL7_RESULTBUILDER_H
