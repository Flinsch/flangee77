
#include <TestLabs/TestSuite.h>

#include <CoreLabs/Guid.h>

#include "../shared.h"

#include <vector>



namespace tl7::internals {
    inline
    cl7::u8string to_string(const cl7::Guid& guid) { return guid.to_string(); }
}



TESTLABS_CASE( u8"CoreLabs:  Guid:  to_string/parse" )
{
    struct Entry
    {
        cl7::byte_vector bytes;
        cl7::u8string lowercase;
        cl7::u8string uppercase;
    } entry;

    const std::vector<Entry> container {
        { {}, u8"00000000-0000-0000-0000-000000000000", u8"00000000-0000-0000-0000-000000000000" },
        { cl7::make_bytes( 1,2,3,4, 5,6, 7,8, 9,10, 11,12,13,14,15,16 ), u8"01020304-0506-0708-090a-0b0c0d0e0f10", u8"01020304-0506-0708-090A-0B0C0D0E0F10" },
        { cl7::make_bytes( 16,32,48,64, 80,96, 112,128, 144,160, 176,192,208,224,240,0 ), u8"10203040-5060-7080-90a0-b0c0d0e0f000", u8"10203040-5060-7080-90A0-B0C0D0E0F000" },
        { cl7::make_bytes( 17,34,51,68, 85,102, 119,136, 153,170, 187,204,221,238,255,0 ), u8"11223344-5566-7788-99aa-bbccddeeff00", u8"11223344-5566-7788-99AA-BBCCDDEEFF00" },
    };

    for ( size_t i = 0; i < container.size(); ++i )
    {
        const Entry& entry = container[ i ];

        cl7::Guid guid;

        if ( !entry.bytes.empty() )
        {
            TESTLABS_ASSERT_EQ( entry.bytes.size(), 16 );

            std::array<std::byte, 16> bytes;
            for ( size_t k = 0; k < 16; ++k )
                bytes[ k ] = entry.bytes[ k ];

            guid = cl7::Guid( bytes );
        }

        TESTLABS_CHECK_EQ( guid.to_string( false ), entry.lowercase );
        TESTLABS_CHECK_EQ( guid.to_string( true ), entry.uppercase );
        TESTLABS_CHECK_EQ( cl7::Guid::parse( entry.lowercase ), guid );
        TESTLABS_CHECK_EQ( cl7::Guid::parse( entry.uppercase ), guid );
    }
}

TESTLABS_CASE( u8"CoreLabs:  Guid:  parsing bad strings" )
{
    struct Entry
    {
        cl7::u8string string;
    } entry;

    const std::vector<Entry> container {
        { u8"" },
        { u8"0" },
        { u8"1" },
        { u8"foo" },
        { u8"00000000-0000-0000-0000-00000000000G" },
        { u8"12345678+1234+1234+1234+1234567890AB" },
    };

    for ( size_t i = 0; i < container.size(); ++i )
    {
        const Entry& entry = container[ i ];

        TESTLABS_CHECK_EQ( cl7::Guid::parse( entry.string ), cl7::Guid() );
        TESTLABS_CHECK_EQ( cl7::Guid::parse( entry.string ).is_valid(), false );
    }
}
