#ifndef TL7_MACROS_H
#define TL7_MACROS_H

#include <CoreLabs/root.h>



#define TESTLABS_DECLARE_CASE(func, name) \
    TL7_INTERNAL_DECLARE_AND_REGISTER_GLOBAL_FUNC(PPCAT(_tl7_, func), name)

#define TESTLABS_DEFINE_CASE(func) \
    TL7_INTERNAL_DEFINE_GLOBAL_FUNC(PPCAT(_tl7_, func))

#define TESTLABS_CASE(name) \
    TL7_INTERNAL_DECLARE_AND_REGISTER_AND_DEFINE_GLOBAL_FUNC(PPCAT(_tl7_anon_, __COUNTER__), name)

#define TESTLABS_SUBCASE(name) \
    TL7_INTERNAL_DEFINE_AND_SWITCH_SUBCASE_BRANCH(PPCAT(_tl7_subcase_, __COUNTER__), name)

#define TESTLABS_SUBCASE_ITERATION(name, iteration_number) \
    TL7_INTERNAL_DEFINE_AND_SWITCH_SUBCASE_ITERATION_BRANCH(PPCAT(_tl7_subcase_iteration_, __COUNTER__), name, iteration_number)

#define TESTLABS_SUBCASE_BATCH(name, iterable_data_container, data) \
    TL7_INTERNAL_DEFINE_AND_SWITCH_SUBCASE_BATCH_ITERATION_BRANCH(PPCAT(_tl7_subcase_iteration_, __COUNTER__), name, PPCAT(_tl7_subcase_iteration_number_, __COUNTER__), iterable_data_container, data, PPCAT(_tl7_tmp, __COUNTER__), nullptr)

#define TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING(name, iterable_data_container, data, stringifiable) \
    TL7_INTERNAL_DEFINE_AND_SWITCH_SUBCASE_BATCH_ITERATION_BRANCH(PPCAT(_tl7_subcase_iteration_, __COUNTER__), name, PPCAT(_tl7_subcase_iteration_number_, __COUNTER__), iterable_data_container, data, PPCAT(_tl7_tmp, __COUNTER__), stringifiable)



#define TESTLABS_SUBCASE_DATA_STRING(stringifiable) \
    TL7_INTERNAL_SET_SUBCASE_DATA(stringifiable)



#define TESTLABS_CHECK(expression)           TL7_INTERNAL_TRY_POST_RESULT_1(expression, make_check_result, "CHECK")
#define TESTLABS_CHECK_EQ(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_check_result, "CHECK", ==)
#define TESTLABS_CHECK_NE(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_check_result, "CHECK", !=)
#define TESTLABS_CHECK_LT(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_check_result, "CHECK", <)
#define TESTLABS_CHECK_GT(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_check_result, "CHECK", >)
#define TESTLABS_CHECK_LE(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_check_result, "CHECK", <=)
#define TESTLABS_CHECK_GE(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_check_result, "CHECK", >=)
#define TESTLABS_CHECK_EQ_STR(expr1, expr2)  TL7_INTERNAL_TRY_POST_RESULT_EQ_STR(expr1, expr2, make_check_result, "CHECK", ==)
#define TESTLABS_CHECK_EQ_FLT(expr1, expr2)  TL7_INTERNAL_TRY_POST_RESULT_EQ_FLT(expr1, expr2, make_check_result, "CHECK", ==)
#define TESTLABS_CHECK_EQ_DBL(expr1, expr2)  TL7_INTERNAL_TRY_POST_RESULT_EQ_DBL(expr1, expr2, make_check_result, "CHECK", ==)
#define TESTLABS_CHECK_EQ_ROUND(num_decimals, expr1, expr2) TL7_INTERNAL_TRY_POST_RESULT_EQ_ROUND(num_decimals, expr1, expr2, make_check_result, "CHECK", ==)

#define TESTLABS_PRESUME(expression)           TL7_INTERNAL_TRY_POST_RESULT_1(expression, make_presumption_result, "PRESUME")
#define TESTLABS_PRESUME_EQ(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_presumption_result, "PRESUME", ==)
#define TESTLABS_PRESUME_NE(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_presumption_result, "PRESUME", !=)
#define TESTLABS_PRESUME_LT(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_presumption_result, "PRESUME", <)
#define TESTLABS_PRESUME_GT(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_presumption_result, "PRESUME", >)
#define TESTLABS_PRESUME_LE(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_presumption_result, "PRESUME", <=)
#define TESTLABS_PRESUME_GE(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_presumption_result, "PRESUME", >=)
#define TESTLABS_PRESUME_EQ_STR(expr1, expr2)  TL7_INTERNAL_TRY_POST_RESULT_EQ_STR(expr1, expr2, make_presumption_result, "PRESUME", ==)
#define TESTLABS_PRESUME_EQ_FLT(expr1, expr2)  TL7_INTERNAL_TRY_POST_RESULT_EQ_FLT(expr1, expr2, make_presumption_result, "PRESUME", ==)
#define TESTLABS_PRESUME_EQ_DBL(expr1, expr2)  TL7_INTERNAL_TRY_POST_RESULT_EQ_DBL(expr1, expr2, make_presumption_result, "PRESUME", ==)
#define TESTLABS_PRESUME_EQ_ROUND(num_decimals, expr1, expr2) TL7_INTERNAL_TRY_POST_RESULT_EQ_ROUND(num_decimals, expr1, expr2, make_presumption_result, "PRESUME", ==)

#define TESTLABS_ASSERT(expression)           TL7_INTERNAL_TRY_POST_RESULT_1(expression, make_assertion_result, "ASSERT")
#define TESTLABS_ASSERT_EQ(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_assertion_result, "ASSERT", ==)
#define TESTLABS_ASSERT_NE(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_assertion_result, "ASSERT", !=)
#define TESTLABS_ASSERT_LT(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_assertion_result, "ASSERT", <)
#define TESTLABS_ASSERT_GT(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_assertion_result, "ASSERT", >)
#define TESTLABS_ASSERT_LE(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_assertion_result, "ASSERT", <=)
#define TESTLABS_ASSERT_GE(expr1, expr2)      TL7_INTERNAL_TRY_POST_RESULT_2(expr1, expr2, make_assertion_result, "ASSERT", >=)
#define TESTLABS_ASSERT_EQ_STR(expr1, expr2)  TL7_INTERNAL_TRY_POST_RESULT_EQ_STR(expr1, expr2, make_assertion_result, "ASSERT", ==)
#define TESTLABS_ASSERT_EQ_FLT(expr1, expr2)  TL7_INTERNAL_TRY_POST_RESULT_EQ_FLT(expr1, expr2, make_assertion_result, "ASSERT", ==)
#define TESTLABS_ASSERT_EQ_DBL(expr1, expr2)  TL7_INTERNAL_TRY_POST_RESULT_EQ_DBL(expr1, expr2, make_assertion_result, "ASSERT", ==)
#define TESTLABS_ASSERT_EQ_ROUND(num_decimals, expr1, expr2) TL7_INTERNAL_TRY_POST_RESULT_EQ_ROUND(num_decimals, expr1, expr2, make_assertion_result, "ASSERT", ==)



#endif // TL7_MACROS_H
