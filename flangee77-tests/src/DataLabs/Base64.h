#ifndef F77_TESTS_CL7_BASE64_H
#define F77_TESTS_CL7_BASE64_H

#include <DataLabs/Base64.h>

#include <CoreLabs/text/codec.h>

#include <TestLabs/TestSuite.h>

#include "../shared.h"

#include <vector>



TESTLABS_CASE( u8"CoreLabs:  Base64:  encode/decode" )
{
    struct Entry
    {
        cl7::byte_vector data;
        cl7::u8string base64;
        dl7::Base64::Options options;
    } entry;

    dl7::Base64::Options default_options = dl7::Base64::default_options;

    dl7::Base64::Options without_padding = default_options;
    without_padding.pad = false;

    dl7::Base64::Options other_characters = default_options;
    other_characters.ch62 = '-';
    other_characters.ch63 = '_';

    dl7::Base64::Options insert_breaks = default_options;
    insert_breaks.insert_breaks = true;

    const std::vector<Entry> container {
        { {}, u8"" },

        { cl7::make_bytes( 0 ), u8"AA==", default_options },
        { cl7::make_bytes( 0, 0 ), u8"AAA=", default_options },
        { cl7::make_bytes( 0, 0, 0 ), u8"AAAA", default_options },
        { cl7::text::codec::to_bytes( "Hello, World!" ), u8"SGVsbG8sIFdvcmxkIQ==", default_options },
        { cl7::text::codec::to_bytes( "Dr. Stefan Fleischer" ), u8"RHIuIFN0ZWZhbiBGbGVpc2NoZXI=", default_options },
        { cl7::text::codec::to_bytes( "The quick brown fox jumps over the lazy dog." ), u8"VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZy4=", default_options },
        { cl7::text::codec::to_bytes( "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum." ), u8"TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNldGV0dXIgc2FkaXBzY2luZyBlbGl0ciwgc2VkIGRpYW0gbm9udW15IGVpcm1vZCB0ZW1wb3IgaW52aWR1bnQgdXQgbGFib3JlIGV0IGRvbG9yZSBtYWduYSBhbGlxdXlhbSBlcmF0LCBzZWQgZGlhbSB2b2x1cHR1YS4gQXQgdmVybyBlb3MgZXQgYWNjdXNhbSBldCBqdXN0byBkdW8gZG9sb3JlcyBldCBlYSByZWJ1bS4=", default_options },
        { cl7::make_bytes( 0xff, 0xee, 0xdd, 0x11 ), u8"/+7dEQ==", default_options },

        { cl7::make_bytes( 0 ), u8"AA", without_padding },
        { cl7::make_bytes( 0, 0 ), u8"AAA", without_padding },
        { cl7::make_bytes( 0, 0, 0 ), u8"AAAA", without_padding },
        { cl7::text::codec::to_bytes( "Hello, World!" ), u8"SGVsbG8sIFdvcmxkIQ", without_padding },
        { cl7::text::codec::to_bytes( "Dr. Stefan Fleischer" ), u8"RHIuIFN0ZWZhbiBGbGVpc2NoZXI", without_padding },
        { cl7::text::codec::to_bytes( "The quick brown fox jumps over the lazy dog." ), u8"VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZy4", without_padding },
        { cl7::text::codec::to_bytes( "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum." ), u8"TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNldGV0dXIgc2FkaXBzY2luZyBlbGl0ciwgc2VkIGRpYW0gbm9udW15IGVpcm1vZCB0ZW1wb3IgaW52aWR1bnQgdXQgbGFib3JlIGV0IGRvbG9yZSBtYWduYSBhbGlxdXlhbSBlcmF0LCBzZWQgZGlhbSB2b2x1cHR1YS4gQXQgdmVybyBlb3MgZXQgYWNjdXNhbSBldCBqdXN0byBkdW8gZG9sb3JlcyBldCBlYSByZWJ1bS4", without_padding },
        { cl7::make_bytes( 0xff, 0xee, 0xdd, 0x11 ), u8"/+7dEQ", without_padding },

        { cl7::make_bytes( 0 ), u8"AA==", other_characters },
        { cl7::make_bytes( 0, 0 ), u8"AAA=", other_characters },
        { cl7::make_bytes( 0, 0, 0 ), u8"AAAA", other_characters },
        { cl7::text::codec::to_bytes( "Hello, World!" ), u8"SGVsbG8sIFdvcmxkIQ==", other_characters },
        { cl7::text::codec::to_bytes( "Dr. Stefan Fleischer" ), u8"RHIuIFN0ZWZhbiBGbGVpc2NoZXI=", other_characters },
        { cl7::text::codec::to_bytes( "The quick brown fox jumps over the lazy dog." ), u8"VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZy4=", other_characters },
        { cl7::text::codec::to_bytes( "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum." ), u8"TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNldGV0dXIgc2FkaXBzY2luZyBlbGl0ciwgc2VkIGRpYW0gbm9udW15IGVpcm1vZCB0ZW1wb3IgaW52aWR1bnQgdXQgbGFib3JlIGV0IGRvbG9yZSBtYWduYSBhbGlxdXlhbSBlcmF0LCBzZWQgZGlhbSB2b2x1cHR1YS4gQXQgdmVybyBlb3MgZXQgYWNjdXNhbSBldCBqdXN0byBkdW8gZG9sb3JlcyBldCBlYSByZWJ1bS4=", other_characters },
        { cl7::make_bytes( 0xff, 0xee, 0xdd, 0x11 ), u8"_-7dEQ==", other_characters },

        { cl7::make_bytes( 0 ), u8"AA==", insert_breaks },
        { cl7::make_bytes( 0, 0 ), u8"AAA=", insert_breaks },
        { cl7::make_bytes( 0, 0, 0 ), u8"AAAA", insert_breaks },
        { cl7::text::codec::to_bytes( "Hello, World!" ), u8"SGVsbG8sIFdvcmxkIQ==", insert_breaks },
        { cl7::text::codec::to_bytes( "Dr. Stefan Fleischer" ), u8"RHIuIFN0ZWZhbiBGbGVpc2NoZXI=", insert_breaks },
        { cl7::text::codec::to_bytes( "The quick brown fox jumps over the lazy dog." ), u8"VGhlIHF1aWNrIGJyb3duIGZveCBqdW1wcyBvdmVyIHRoZSBsYXp5IGRvZy4=", insert_breaks },
        { cl7::text::codec::to_bytes( "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum." ), u8"TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNldGV0dXIgc2FkaXBzY2luZyBlbGl0ciwg\nc2VkIGRpYW0gbm9udW15IGVpcm1vZCB0ZW1wb3IgaW52aWR1bnQgdXQgbGFib3JlIGV0IGRvbG9y\nZSBtYWduYSBhbGlxdXlhbSBlcmF0LCBzZWQgZGlhbSB2b2x1cHR1YS4gQXQgdmVybyBlb3MgZXQg\nYWNjdXNhbSBldCBqdXN0byBkdW8gZG9sb3JlcyBldCBlYSByZWJ1bS4=", insert_breaks },
        { cl7::make_bytes( 0xff, 0xee, 0xdd, 0x11 ), u8"/+7dEQ==", insert_breaks },
    };

    for ( size_t i = 0; i < container.size(); ++i )
    {
        const Entry& entry = container[ i ];

        dl7::Base64 base64{ entry.options };

        TESTLABS_CHECK_EQ( base64.encode( entry.data ), entry.base64 );
        TESTLABS_CHECK_EQ( base64.decode( entry.base64 ), entry.data );

        TESTLABS_CHECK_EQ( base64.calculate_data_size( entry.base64 ), entry.data.size() );
    }
}

TESTLABS_CASE( u8"CoreLabs:  Base64:  decoding bad strings" )
{
    struct Entry
    {
        cl7::u8string base64;
    } entry;

    const std::vector<Entry> container {
        { u8"" },
        { u8"0" },
        { u8"=" },
        { u8"!" },
        { u8"=AAA" },
        // ...
    };

    for ( size_t i = 0; i < container.size(); ++i )
    {
        const Entry& entry = container[ i ];

        dl7::Base64 base64;

        TESTLABS_CHECK_EQ( base64.decode( entry.base64 ).size(), 0 );
    }
}



#endif // F77_TESTS_CL7_BASE64_H
