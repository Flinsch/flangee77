#pragma once
#ifndef TL7_EXCEPTIONS_ASSERTIONEXCEPTION_H
#define TL7_EXCEPTIONS_ASSERTIONEXCEPTION_H

#include "../Meta.h"



namespace tl7 {
namespace exceptions {



class assertion_exception
    : public std::exception
{

public:
    assertion_exception(cl7::string_view original_expression, cl7::string_view evaluated_expression, cl7::string_view stringification, const cl7::char_type* file_path, unsigned line_number)
        : original_expression( original_expression )
        , evaluated_expression( evaluated_expression )
        , meta{ cl7::string(stringification), file_path, line_number, 0 }
    {}

private:
    assertion_exception() = delete;

public:
    const cl7::string original_expression;
    const cl7::string evaluated_expression;
    const Meta meta;

public:
    virtual char const* what() const noexcept
    {
        return "tl7::assertion_exception";
    }

}; // class assertion_exception



} // namespace exceptions
} // namespace tl7

#endif // TL7_EXCEPTIONS_ASSERTIONEXCEPTION_H
