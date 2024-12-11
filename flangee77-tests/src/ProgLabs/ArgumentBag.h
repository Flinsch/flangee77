#pragma once
#ifndef F77_TESTS_PL7_ARGUMENTBAG_H
#define F77_TESTS_PL7_ARGUMENTBAG_H

#include <ProgLabs/ArgumentBag.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



TESTLABS_CASE( TEXT("ProgLabs:  ArgumentBag:  ") )
{
    pl7::ArgumentBag argument_bag;

    std::vector<cl7::string_view> arguments = { TEXT("test"), TEXT("--help"), TEXT("-h"), TEXT("--verbose"), TEXT("-abc"), TEXT("foobar"), TEXT("--output-file=\"result.txt\""), TEXT("-o"), TEXT("result.txt"), TEXT("--f"), TEXT("foo"), TEXT("-f=bar"), TEXT("-f"), TEXT("\"foo bar\""), TEXT("\"--key=\\\"value\\\"\"") };
    argument_bag = pl7::ArgumentBag( arguments );
    TESTLABS_CHECK_EQ( argument_bag.get_arguments().size(), 15 );
    TESTLABS_ASSERT_EQ( argument_bag.get_positional_arguments().size(), 2 );
    TESTLABS_CHECK_EQ( argument_bag.get_positional_arguments()[0], TEXT("test") );
    TESTLABS_CHECK_EQ( argument_bag.get_positional_arguments()[1], TEXT("foobar") );
    TESTLABS_CHECK_EQ( argument_bag.get_flags().size(), 6 );
    TESTLABS_CHECK( argument_bag.has_flag( TEXT("help") ) );
    TESTLABS_CHECK( argument_bag.has_flag( TEXT("h") ) );
    TESTLABS_CHECK( argument_bag.has_flag( TEXT("verbose") ) );
    TESTLABS_CHECK( argument_bag.has_flag( TEXT("a") ) );
    TESTLABS_CHECK( argument_bag.has_flag( TEXT("b") ) );
    TESTLABS_CHECK( argument_bag.has_flag( TEXT("c") ) );
    TESTLABS_CHECK_EQ( argument_bag.get_option_names().size(), 4 );
    TESTLABS_CHECK( argument_bag.has_option( TEXT("output-file") ) );
    TESTLABS_CHECK( argument_bag.has_single_value( TEXT("output-file") ) );
    TESTLABS_CHECK_EQ( argument_bag.get_value( TEXT("output-file") ), TEXT("result.txt") );
    TESTLABS_CHECK( argument_bag.has_option( TEXT("o") ) );
    TESTLABS_CHECK( argument_bag.has_single_value( TEXT("o") ) );
    TESTLABS_CHECK_EQ( argument_bag.get_value( TEXT("o") ), TEXT("result.txt") );
    TESTLABS_CHECK( argument_bag.has_option( TEXT("f") ) );
    TESTLABS_CHECK( !argument_bag.has_single_value( TEXT("f") ) );
    TESTLABS_ASSERT_EQ( argument_bag.get_values( TEXT("f") ).size(), 3 );
    TESTLABS_CHECK_EQ( argument_bag.get_values( TEXT("f") )[0], TEXT("foo") );
    TESTLABS_CHECK_EQ( argument_bag.get_values( TEXT("f") )[1], TEXT("bar") );
    TESTLABS_CHECK_EQ( argument_bag.get_values( TEXT("f") )[2], TEXT("foo bar") );
    TESTLABS_CHECK_EQ( argument_bag.get_value( TEXT("f") ), TEXT("foo bar") );
    TESTLABS_CHECK( argument_bag.has_option( TEXT("key") ) );
    TESTLABS_CHECK( argument_bag.has_single_value( TEXT("key") ) );
    TESTLABS_CHECK_EQ( argument_bag.get_value( TEXT("key") ), TEXT("value") );
}



#endif // F77_TESTS_PL7_ARGUMENTBAG_H
