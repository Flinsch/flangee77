#ifndef TL7_EXCEPTIONS_ASSERTIONEXCEPTION_H
#define TL7_EXCEPTIONS_ASSERTIONEXCEPTION_H

#include "../Meta.h"



namespace tl7::exceptions {



struct assertion_exception
    : public std::exception
{

    cl7::u8string original_expression;
    cl7::u8string evaluated_expression;
    Meta meta;

    assertion_exception() = delete;

    assertion_exception(cl7::u8string_view original_expression, cl7::u8string_view evaluated_expression, cl7::u8string_view stringification, const char* file_path, unsigned line_number)
        : original_expression(original_expression)
        , evaluated_expression(evaluated_expression)
        , meta{cl7::u8string(stringification), file_path, line_number, 0}
    {}

    char const* what() const noexcept override
    {
        return "tl7::assertion_exception";
    }

}; // struct assertion_exception



} // namespace tl7::exceptions

#endif // TL7_EXCEPTIONS_ASSERTIONEXCEPTION_H
