#ifndef TL7_INTERNALS_H
#define TL7_INTERNALS_H

#include <CoreLabs/string.h>

#include <cfloat>



#define TL7_INTERNAL_DEFINE_AND_REGISTER_GLOBAL_FUNC(func, name) \
    static void func(tl7::Context&); \
    TL7_INTERNAL_REGISTER_GLOBAL_FUNC(func, name) \
    static void func(tl7::Context& ctx)

#define TL7_INTERNAL_REGISTER_GLOBAL_FUNC(func, name) \
    TL7_INTERNAL_CONSUME_REGISTRATION(PPCAT(_tl7_foo_, __COUNTER__), tl7::TestSuite::instance().register_test_case(func, name, __FILE__, __LINE__))

#define TL7_INTERNAL_CONSUME_REGISTRATION(foo, reg) \
    static const int foo = tl7::internals::consume(&foo, reg);



#define TL7_INTERNAL_DEFINE_AND_SWITCH_SUBCASE_BRANCH(subcase, name) \
    if (const tl7::SubcaseBranchSwitch& subcase = tl7::SubcaseBranchSwitch(ctx, name, __FILE__, __LINE__))

#define TL7_INTERNAL_DEFINE_AND_SWITCH_SUBCASE_ITERATION_BRANCH(subcase, name, iteration_number) \
    if (const tl7::SubcaseBranchSwitch& subcase = tl7::SubcaseBranchSwitch(ctx, name + cl7::u8string(u8" [") + tl7::internals::to_string(iteration_number) + cl7::u8string(u8"]"), __FILE__, __LINE__, iteration_number))

#define TL7_INTERNAL_DEFINE_AND_SWITCH_SUBCASE_BATCH_ITERATION_BRANCH(subcase, name, iteration_number, iterable_data_container, data, tmp, stringifiable) \
    signed iteration_number = 0; \
    for (auto tmp = std::begin(iterable_data_container); tmp != std::end(iterable_data_container); ++tmp, ++iteration_number) \
        if (const tl7::SubcaseBranchSwitch& subcase = tl7::SubcaseBranchSwitch(ctx, name + cl7::u8string(u8" [") + tl7::internals::to_string(iteration_number) + cl7::u8string(u8"]"), __FILE__, __LINE__, (data = *tmp, TL7_INTERNAL_TRY_SET_SUBCASE_DATA(stringifiable), iteration_number)))



#define TL7_INTERNAL_TRY_SET_SUBCASE_DATA(stringifiable) \
    (std::is_null_pointer_v<decltype(stringifiable)> ? ctx.subcases.reset_data_string() : TL7_INTERNAL_SET_SUBCASE_DATA(stringifiable))

#define TL7_INTERNAL_SET_SUBCASE_DATA(stringifiable) \
    ctx.subcases.set_data_string(std::make_shared<cl7::u8string>(tl7::internals::to_string(stringifiable)))



#define TL7_INTERNAL_TRY_POST_RESULT_1(expression, make_result, macro_base) \
    ctx.try_post_result(tl7::ResultBuilder().make_result(ctx, (u8##macro_base "( " #expression " )"), (u8##macro_base "( ") + tl7::internals::to_string(!!(expression)) + u8" )", __FILE__, __LINE__, tl7::Result::make_outcome(expression)))

#define TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_result, macro_base, op) \
    ctx.try_post_result(tl7::ResultBuilder().make_result(ctx, (u8##macro_base "( " #expr1 " " #op " " #expr2 " )"), (u8##macro_base "( ") + tl7::internals::to_string(expr1) + (u8" " #op " ") + tl7::internals::to_string(expr2) + u8" )", __FILE__, __LINE__, tl7::Result::make_outcome((expr1)op(expr2))))

#define TL7_INTERNAL_TRY_POST_RESULT_EQ_FLT(expr1, expr2, make_result, macro_base, op) \
    ctx.try_post_result(tl7::ResultBuilder().make_result(ctx, (u8##macro_base "( " #expr1 " " #op " " #expr2 " )"), (u8##macro_base "( ") + tl7::internals::to_string(expr1) + (u8" " #op " ") + tl7::internals::to_string(expr2) + u8" )", __FILE__, __LINE__, tl7::Result::make_outcome(::abs((expr1) - (expr2)) < 2.0f * FLT_EPSILON)))

#define TL7_INTERNAL_TRY_POST_RESULT_EQ_DBL(expr1, expr2, make_result, macro_base, op) \
    ctx.try_post_result(tl7::ResultBuilder().make_result(ctx, (u8##macro_base "( " #expr1 " " #op " " #expr2 " )"), (u8##macro_base "( ") + tl7::internals::to_string(expr1) + (u8" " #op " ") + tl7::internals::to_string(expr2) + u8" )", __FILE__, __LINE__, tl7::Result::make_outcome(::abs((expr1) - (expr2)) < 2.0*DBL_EPSILON)))

#define TL7_INTERNAL_TRY_POST_RESULT_EQ_STR(expr1, expr2, make_result, macro_base, op) \
    ctx.try_post_result(tl7::ResultBuilder().make_result(ctx, (u8##macro_base "( " #expr1 " " #op " " #expr2 " )"), (u8##macro_base "( ") + tl7::internals::to_string(expr1) + (u8" " #op " ") + tl7::internals::to_string(expr2) + u8" )", __FILE__, __LINE__, tl7::Result::make_outcome(cl7::to_string(expr1) op cl7::to_string(expr2))))



namespace tl7::internals {



    static constexpr int consume(const int* foo, int reg) noexcept
    {
        return 0;
    }



    inline cl7::u8string to_string(signed val) { return cl7::to_string(val); }
    inline cl7::u8string to_string(signed long val) { return cl7::to_string(val); }
    inline cl7::u8string to_string(signed long long val) { return cl7::to_string(val); }
    inline cl7::u8string to_string(unsigned val) { return cl7::to_string(val); }
    inline cl7::u8string to_string(unsigned long val) { return cl7::to_string(val); }
    inline cl7::u8string to_string(unsigned long long val) { return cl7::to_string(val); }
    inline cl7::u8string to_string(float val) { return cl7::to_string(val); }
    inline cl7::u8string to_string(double val) { return cl7::to_string(val); }
    inline cl7::u8string to_string(long double val) { return cl7::to_string(val); }

    inline cl7::u8string to_string(bool val) { return cl7::to_string(val); }

    cl7::u8string to_string(const std::nullptr_t& null);

    cl7::u8string to_string(const std::string& val);
    cl7::u8string to_string(const std::wstring& val);
    cl7::u8string to_string(const std::u8string& val);
    cl7::u8string to_string(const std::u16string& val);
    cl7::u8string to_string(const std::u32string& val);

    cl7::u8string to_string(std::string_view val);
    cl7::u8string to_string(std::wstring_view val);
    cl7::u8string to_string(std::u8string_view val);
    cl7::u8string to_string(std::u16string_view val);
    cl7::u8string to_string(std::u32string_view val);



} // namespace tl7::internals

#endif // TL7_INTERNALS_H
