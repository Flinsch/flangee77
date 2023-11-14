#pragma once
#ifndef TL7_MACROS_H
#define TL7_MACROS_H

#include <CoreLabs/root.h>



#define TL7_TEST_CASE(name) \
    _TL7_DEFINE_AND_REGISTER_GLOBAL_FUNC( PPCAT(_tl7_anon_, __LINE__), name )

#define TL7_SUBCASE(name) \
    _TL7_DEFINE_AND_SWITCH_SUBCASE_BRANCH( PPCAT(_tl7_subcase_, __LINE__), name )

#define TL7_SUBCASE_ITERATION(name, iteration_number) \
    _TL7_DEFINE_AND_SWITCH_SUBCASE_ITERATION_BRANCH( PPCAT(_tl7_subcase_iteration_, __LINE__), name, iteration_number )

#define TL7_SUBCASE_BATCH(name, iterable_data_container, data) \
    _TL7_DEFINE_AND_SWITCH_SUBCASE_BATCH_ITERATION_BRANCH( PPCAT(_tl7_subcase_iteration_, __LINE__), name, PPCAT(_tl7_subcase_iteration_number_, __LINE__), iterable_data_container, data, PPCAT(_tl7_tmp, __LINE__) )



#define TL7_CHECK(expression)           _TL7_TRY_POST_RESULT_1( expression, make_check_result, "TL7_CHECK" )
#define TL7_CHECK_EQ(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_check_result, "TL7_CHECK", == )
#define TL7_CHECK_NE(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_check_result, "TL7_CHECK", != )
#define TL7_CHECK_LT(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_check_result, "TL7_CHECK", < )
#define TL7_CHECK_GT(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_check_result, "TL7_CHECK", > )
#define TL7_CHECK_LE(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_check_result, "TL7_CHECK", <= )
#define TL7_CHECK_GE(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_check_result, "TL7_CHECK", >= )

#define TL7_PRESUME(expression)           _TL7_TRY_POST_RESULT_1( expression, make_presumption_result, "TL7_PRESUME" )
#define TL7_PRESUME_EQ(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_presumption_result, "TL7_PRESUME", == )
#define TL7_PRESUME_NE(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_presumption_result, "TL7_PRESUME", != )
#define TL7_PRESUME_LT(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_presumption_result, "TL7_PRESUME", < )
#define TL7_PRESUME_GT(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_presumption_result, "TL7_PRESUME", > )
#define TL7_PRESUME_LE(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_presumption_result, "TL7_PRESUME", <= )
#define TL7_PRESUME_GE(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_presumption_result, "TL7_PRESUME", >= )

#define TL7_ASSERT(expression)           _TL7_TRY_POST_RESULT_1( expression, make_assertion_result, "TL7_ASSERT" )
#define TL7_ASSERT_EQ(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_assertion_result, "TL7_ASSERT", == )
#define TL7_ASSERT_NE(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_assertion_result, "TL7_ASSERT", != )
#define TL7_ASSERT_LT(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_assertion_result, "TL7_ASSERT", < )
#define TL7_ASSERT_GT(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_assertion_result, "TL7_ASSERT", > )
#define TL7_ASSERT_LE(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_assertion_result, "TL7_ASSERT", <= )
#define TL7_ASSERT_GE(expr1, expr2)      _TL7_TRY_POST_RESULT_2( expr1, expr2, make_assertion_result, "TL7_ASSERT", >= )



#endif // TL7_MACROS_H
