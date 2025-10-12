#ifndef F77_TESTS_PL7_ARGUMENTBAG_H
#define F77_TESTS_PL7_ARGUMENTBAG_H

#include <ProgLabs/ArgumentBag.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"



TESTLABS_CASE( u8"ProgLabs:  ArgumentBag:  " )
{
    std::vector<cl7::u8string_view> arguments = { u8"test", u8"--help", u8"-h", u8"--verbose", u8"-abc", u8"foobar", u8"--output-file=\"result.txt\"", u8"-o", u8"result.txt", u8"--f", u8"foo", u8"-f=bar", u8"-f", u8"\"foo bar\"", u8"\"--key=\\\"value\\\"\"" };
    pl7::ArgumentBag argument_bag = pl7::ArgumentBag( arguments );

    TESTLABS_CHECK_EQ( argument_bag.get_arguments().size(), 15 );
    TESTLABS_ASSERT_EQ( argument_bag.get_positional_arguments().size(), 2 );
    TESTLABS_CHECK_EQ( argument_bag.get_positional_arguments()[0], u8"test" );
    TESTLABS_CHECK_EQ( argument_bag.get_positional_arguments()[1], u8"foobar" );
    TESTLABS_CHECK_EQ( argument_bag.get_flags().size(), 6 );
    TESTLABS_CHECK( argument_bag.has_flag( u8"help" ) );
    TESTLABS_CHECK( argument_bag.has_flag( u8"h" ) );
    TESTLABS_CHECK( argument_bag.has_flag( u8"verbose" ) );
    TESTLABS_CHECK( argument_bag.has_flag( u8"a" ) );
    TESTLABS_CHECK( argument_bag.has_flag( u8"b" ) );
    TESTLABS_CHECK( argument_bag.has_flag( u8"c" ) );
    TESTLABS_CHECK_EQ( argument_bag.get_option_names().size(), 4 );
    TESTLABS_CHECK( argument_bag.has_option( u8"output-file" ) );
    TESTLABS_CHECK( argument_bag.has_single_value( u8"output-file" ) );
    TESTLABS_CHECK_EQ( argument_bag.get_value( u8"output-file" ), u8"result.txt" );
    TESTLABS_CHECK( argument_bag.has_option( u8"o" ) );
    TESTLABS_CHECK( argument_bag.has_single_value( u8"o" ) );
    TESTLABS_CHECK_EQ( argument_bag.get_value( u8"o" ), u8"result.txt" );
    TESTLABS_CHECK( argument_bag.has_option( u8"f" ) );
    TESTLABS_CHECK( !argument_bag.has_single_value( u8"f" ) );
    TESTLABS_ASSERT_EQ( argument_bag.get_values( u8"f" ).size(), 3 );
    TESTLABS_CHECK_EQ( argument_bag.get_values( u8"f" )[0], u8"foo" );
    TESTLABS_CHECK_EQ( argument_bag.get_values( u8"f" )[1], u8"bar" );
    TESTLABS_CHECK_EQ( argument_bag.get_values( u8"f" )[2], u8"foo bar" );
    TESTLABS_CHECK_EQ( argument_bag.get_value( u8"f" ), u8"foo bar" );
    TESTLABS_CHECK( argument_bag.has_option( u8"key" ) );
    TESTLABS_CHECK( argument_bag.has_single_value( u8"key" ) );
    TESTLABS_CHECK_EQ( argument_bag.get_value( u8"key" ), u8"value" );
}



#endif // F77_TESTS_PL7_ARGUMENTBAG_H
