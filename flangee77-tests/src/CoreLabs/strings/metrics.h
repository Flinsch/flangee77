#ifndef F77_TESTS_CL7_STRINGS_METRICS_H
#define F77_TESTS_CL7_STRINGS_METRICS_H

#include <CoreLabs/strings/metrics.h>

#include <TestLabs/TestSuite.h>

#include "../../shared.h"

#include <vector>



TESTLABS_CASE( u8"CoreLabs:  strings:  metrics:  Levenshtein distance" )
{
    struct Entry
    {
        cl7::u8string s1;
        cl7::u8string s2;
        size_t distance;
        float distance_normalized;
    } entry;

    const std::vector<Entry> container {
        { u8"", u8"", 0, 0.0f },
        { u8"", u8"b", 1, 1.0f },
        { u8"a", u8"", 1, 1.0f },
        { u8"a", u8"b", 1, 1.0f },
        { u8"kitten", u8"sitting", 3, 3.0f/7.0f },
        { u8"uninformed", u8"uniformed", 1, 0.1f },
        { u8"flaw", u8"lawn", 2, 0.5f },
        { u8"Bar", u8"Bier", 2, 0.5f },
        { u8"flangee77", u8"flangee77", 0, 0.0f },
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"", container, entry, entry.s1 + u8" <-> " + entry.s2 )
    {
        const auto& s1 = entry.s1;
        const auto& s2 = entry.s2;
        const auto expected_distance = entry.distance;
        const auto expected_normalized = entry.distance_normalized;

        TESTLABS_CHECK_EQ( cl7::strings::metrics::levenshtein( s1, s2 ), expected_distance );
        TESTLABS_CHECK_EQ( cl7::strings::metrics::levenshtein_normalized( s1, s2 ), expected_normalized );
    }
}



#endif // F77_TESTS_CL7_STRINGS_METRICS_H
