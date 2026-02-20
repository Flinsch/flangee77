
#include <TestLabs/TestSuite.h>

#include <CoreLabs/text/inspect.h>

#include "../../shared.h"

#include <vector>



TESTLABS_CASE( u8"CoreLabs:  text:  inspect:  is_digit / is_hex_digit / is_ascii_letter / is_ascii_lower / is_ascii_upper" )
{
    for (unsigned i = 0; i < 256; ++i)
    {
        const auto ch = static_cast<cl7::achar_t>( i );

        TESTLABS_CHECK_EQ( cl7::text::inspect::is_digit( ch ), ch >= '0' && ch <= '9' );
        TESTLABS_CHECK_EQ( cl7::text::inspect::is_hex_digit( ch ), (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f') );
        TESTLABS_CHECK_EQ( cl7::text::inspect::is_ascii_letter( ch ), (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') );
        TESTLABS_CHECK_EQ( cl7::text::inspect::is_ascii_lower( ch ), ch >= 'a' && ch <= 'z' );
        TESTLABS_CHECK_EQ( cl7::text::inspect::is_ascii_upper( ch ), ch >= 'A' && ch <= 'Z' );
    }
}



TESTLABS_CASE( u8"CoreLabs:  text:  inspect:  count_whitespace_prefix" )
{
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::astring_view( "" ) ), 0 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::astring_view( " " ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::astring_view( " x" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::astring_view( " x " ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::astring_view( "  x" ) ), 2 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::astring_view( "  x  " ) ), 2 );

    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u0009" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u000a" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u000b" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u000c" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u000d" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u0020" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u0085" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u00a0" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u1680" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u2000" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u2001" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u2002" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u2003" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u2004" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u2005" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u2006" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u2007" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u2008" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u2009" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u200a" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u2028" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u2029" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u202f" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u205f" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u16string_view( u"\u3000" ) ), 1 );

    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u0009" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u000a" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u000b" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u000c" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u000d" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u0020" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u0085" ) ), 2 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u00a0" ) ), 2 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u1680" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u2000" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u2001" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u2002" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u2003" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u2004" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u2005" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u2006" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u2007" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u2008" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u2009" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u200a" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u2028" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u2029" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u202f" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u205f" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_prefix( cl7::u8string_view( u8"\u3000" ) ), 3 );
}

TESTLABS_CASE( u8"CoreLabs:  text:  inspect:  count_whitespace_suffix" )
{
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::astring_view( "" ) ), 0 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::astring_view( " " ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::astring_view( "x " ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::astring_view( " x " ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::astring_view( "x  " ) ), 2 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::astring_view( "  x  " ) ), 2 );

    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u0009" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u000a" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u000b" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u000c" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u000d" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u0020" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u0085" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u00a0" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u1680" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u2000" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u2001" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u2002" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u2003" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u2004" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u2005" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u2006" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u2007" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u2008" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u2009" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u200a" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u2028" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u2029" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u202f" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u205f" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u16string_view( u"\u3000" ) ), 1 );

    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u0009" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u000a" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u000b" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u000c" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u000d" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u0020" ) ), 1 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u0085" ) ), 2 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u00a0" ) ), 2 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u1680" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u2000" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u2001" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u2002" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u2003" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u2004" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u2005" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u2006" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u2007" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u2008" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u2009" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u200a" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u2028" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u2029" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u202f" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u205f" ) ), 3 );
    TESTLABS_CHECK_EQ( cl7::text::inspect::count_whitespace_suffix( cl7::u8string_view( u8"\u3000" ) ), 3 );
}
