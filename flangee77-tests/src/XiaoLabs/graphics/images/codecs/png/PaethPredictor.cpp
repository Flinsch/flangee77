
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/images/codecs/png/PaethPredictor.h>

#include "../../../../shared.h"



namespace xl7::graphics::images::codecs::png {



TESTLABS_CASE( u8"XiaoLabs:  graphics:  images:  codecs:  png:  PaethPredictor:  predict" )
{
    struct Entry
    {
        uint8_t a, b, c;
        uint8_t expected;
        cl7::u8string string;
    } entry;

    const std::vector<Entry> container = {
        // All equal => return a
        { 0,   0,   0,   0,   u8"a=0 b=0 c=0 => 0" },
        { 255, 255, 255, 255, u8"a=255 b=255 c=255 => 255" },
        { 128, 128, 128, 128, u8"a=128 b=128 c=128 => 128" },

        // pA minimal => return a
        { 1,   2,   3,   1,   u8"a=1 b=2 c=3 => a=1" },   // p=0, pA=1, pB=2, pC=3
        { 200, 100, 50,  200, u8"a=200 b=100 c=50 => a=200" }, // p=250, pA=50, pB=150, pC=200

        // pB minimal => return b
        { 100, 10,  90,  10,  u8"a=100 b=10 c=90 => b=10" },  // p=20, pA=80, pB=10, pC=70
        { 50,  20,  40,  20,  u8"a=50 b=20 c=40 => b=20" },   // p=30, pA=20, pB=10, pC=10 => pB<=pC => b

        // pC minimal => return c
        { 10,  100, 200, 10,  u8"a=10 b=100 c=200 => a=10" },  // p=-90, pA=100, pB=190, pC=290 => a (pA minimal)
        { 10,  200, 100, 100, u8"a=10 b=200 c=100 => c=100" }, // p=110, pA=100, pB=90, pC=10 => c

        // Boundary / wrap-around
        { 255, 0,   0,   255, u8"a=255 b=0 c=0 => a=255" },   // p=255, pA=0, pB=255, pC=255 => a
        { 0,   255, 0,   255, u8"a=0 b=255 c=0 => b=255" },   // p=255, pA=255, pB=0, pC=255 => b
        { 0,   0,   255, 0,   u8"a=0 b=0 c=255 => a=0" },     // p=-255, pA=255, pB=255, pC=510... signed: p=-255, pA=255, pB=255, pC=510 => a (pA==pB, pick a)
    };

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"predict", container, entry, entry.string )
    {
        TESTLABS_CHECK_EQ( PaethPredictor::predict( entry.a, entry.b, entry.c ), entry.expected );
    }
}



} // namespace xl7::graphics::images::codecs::png
