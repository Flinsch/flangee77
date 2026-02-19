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
    static Result make_check_result(const Context& ctx, cl7::u8string_view original_expression, cl7::u8string_view evaluated_expression, const char* file_path, unsigned line_number, const char* function_name, Result::Outcome outcome);
    static Result make_presumption_result(const Context& ctx, cl7::u8string_view original_expression, cl7::u8string_view evaluated_expression, const char* file_path, unsigned line_number, const char* function_name, Result::Outcome outcome);
    static Result make_assertion_result(const Context& ctx, cl7::u8string_view original_expression, cl7::u8string_view evaluated_expression, const char* file_path, unsigned line_number, const char* function_name, Result::Outcome outcome);
    static Result make_exception_result(const Context& ctx, cl7::u8string_view message, const char* file_path = nullptr, unsigned line_number = 0, const char* function_name = nullptr);



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    static Result _make_result(Result::OriginType origin_type, const Context& ctx, const char* file_path, unsigned line_number, const char* function_name, Result::Outcome outcome);
    static Result _make_result(Result::OriginType origin_type, const Context& ctx, cl7::u8string_view stringification, const char* file_path, unsigned line_number, const char* function_name, Result::Outcome outcome);
    static Result _make_result(Result::OriginType origin_type, const Context& ctx, cl7::u8string_view original_expression, cl7::u8string_view evaluated_expression, const char* file_path, unsigned line_number, const char* function_name, Result::Outcome outcome);

}; // class ResultBuilder



} // namespace tl7

#endif // TL7_RESULTBUILDER_H
