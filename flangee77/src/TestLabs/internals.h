#pragma once
#ifndef TL7_INTERNALS_H
#define TL7_INTERNALS_H

#include <CoreLabs/string.h>



#define _TL7_DEFINE_AND_REGISTER_GLOBAL_FUNC(func, name) \
    static void func(tl7::Context&); \
    _TL7_REGISTER_GLOBAL_FUNC( func, name ) \
    static void func(tl7::Context& ctx)

#define _TL7_REGISTER_GLOBAL_FUNC(func, name) \
    _TL7_CONSUME_REGISTRATION( PPCAT(_tl7_foo_, __COUNTER__), tl7::TestSuite::instance().register_test_case( func, name, TEXT(__FILE__), __LINE__ ) )

#define _TL7_CONSUME_REGISTRATION(foo, reg) \
    static const int foo = tl7::internals::consume( &foo, reg );



#define _TL7_DEFINE_AND_SWITCH_SUBCASE_BRANCH(subcase, name) \
    if ( const tl7::SubcaseBranchSwitch& subcase = tl7::SubcaseBranchSwitch( ctx, name, TEXT(__FILE__), __LINE__ ) )

#define _TL7_DEFINE_AND_SWITCH_SUBCASE_ITERATION_BRANCH(subcase, name, iteration_number) \
    if ( const tl7::SubcaseBranchSwitch& subcase = tl7::SubcaseBranchSwitch( ctx, name + cl7::string(TEXT(" [")) + tl7::internals::to_string(iteration_number) + cl7::string(TEXT("]")), TEXT(__FILE__), __LINE__, iteration_number ) )

#define _TL7_DEFINE_AND_SWITCH_SUBCASE_BATCH_ITERATION_BRANCH(subcase, name, iteration_number, iterable_data_container, data, tmp, stringifiable) \
    signed iteration_number = 0; \
    for ( auto tmp = std::begin(iterable_data_container); tmp != std::end(iterable_data_container); ++tmp, ++iteration_number ) \
        if ( const tl7::SubcaseBranchSwitch& subcase = tl7::SubcaseBranchSwitch( ctx, name + cl7::string(TEXT(" [")) + tl7::internals::to_string(iteration_number) + cl7::string(TEXT("]")), TEXT(__FILE__), __LINE__, (data = *tmp, _TL7_TRY_SET_SUBCASE_DATA( stringifiable ), iteration_number) ) )



#define _TL7_TRY_SET_SUBCASE_DATA(stringifiable) \
    (std::is_null_pointer_v<decltype(stringifiable)> ? ctx.subcases.reset_data_string() : _TL7_SET_SUBCASE_DATA( stringifiable ))

#define _TL7_SET_SUBCASE_DATA(stringifiable) \
    ctx.subcases.set_data_string( std::make_shared<cl7::string>( tl7::internals::to_string( stringifiable ) ) )



#define _TL7_TRY_POST_RESULT_1(expression, make_result, macro_base) \
    ctx.try_post_result( tl7::ResultBuilder().make_result( ctx, TEXT(macro_base "( " #expression " )"), TEXT(macro_base "( ") + tl7::internals::to_string(!!(expression)) + TEXT(" )"), TEXT(__FILE__), __LINE__, tl7::Result::make_outcome( expression ) ) )

#define _TL7_TRY_POST_RESULT_2(expr1, expr2, make_result, macro_base, op) \
    ctx.try_post_result( tl7::ResultBuilder().make_result( ctx, TEXT(macro_base "( " #expr1 " " #op " " #expr2 " )"), TEXT(macro_base "( ") + tl7::internals::to_string(expr1) + TEXT(" " #op " ") + tl7::internals::to_string(expr2) + TEXT(" )"), TEXT(__FILE__), __LINE__, tl7::Result::make_outcome( (expr1) op (expr2) ) ) )

#define _TL7_TRY_POST_RESULT_EQ_FLT(expr1, expr2, make_result, macro_base, op) \
    ctx.try_post_result( tl7::ResultBuilder().make_result( ctx, TEXT(macro_base "( " #expr1 " " #op " " #expr2 " )"), TEXT(macro_base "( ") + tl7::internals::to_string(expr1) + TEXT(" " #op " ") + tl7::internals::to_string(expr2) + TEXT(" )"), TEXT(__FILE__), __LINE__, tl7::Result::make_outcome( ::abs((expr1) - (expr2)) < FLT_EPSILON ) ) )

#define _TL7_TRY_POST_RESULT_EQ_DBL(expr1, expr2, make_result, macro_base, op) \
    ctx.try_post_result( tl7::ResultBuilder().make_result( ctx, TEXT(macro_base "( " #expr1 " " #op " " #expr2 " )"), TEXT(macro_base "( ") + tl7::internals::to_string(expr1) + TEXT(" " #op " ") + tl7::internals::to_string(expr2) + TEXT(" )"), TEXT(__FILE__), __LINE__, tl7::Result::make_outcome( ::abs((expr1) - (expr2)) < DBL_EPSILON ) ) )



namespace tl7 {
namespace internals {



    static constexpr int consume(const int*, int) noexcept
    {
        return 0;
    }



    template <typename T>
    cl7::string to_string(const T& val) { return cl7::to_string( val ); }

    cl7::string to_string(const std::nullptr_t&);

    cl7::string to_string(const std::string& val);
    cl7::string to_string(const std::wstring& val);
    cl7::string to_string(const std::u8string& val);
    cl7::string to_string(const std::u16string& val);
    cl7::string to_string(const std::u32string& val);

    cl7::string to_string(const std::string_view& val);
    cl7::string to_string(const std::wstring_view& val);
    cl7::string to_string(const std::u8string_view& val);
    cl7::string to_string(const std::u16string_view& val);
    cl7::string to_string(const std::u32string_view& val);



} // namespace internals
} // namespace tl7

#endif // TL7_INTERNALS_H
