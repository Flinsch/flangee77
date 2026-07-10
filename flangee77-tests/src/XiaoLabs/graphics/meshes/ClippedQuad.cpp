
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/meshes/ClippedQuad.h>

#include "../../shared.h"

#include <limits>



namespace {
    constexpr float infinity = std::numeric_limits<float>::infinity();
}



TESTLABS_CASE( u8"XiaoLabs:  graphics:  meshes:  ClippedQuad:  clip - fully inside stays unchanged" )
{
    const auto result = xl7::graphics::meshes::ClippedQuad::clip({10.0f, 10.0f}, {20.0f, 30.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {100.0f, 100.0f});

    TESTLABS_ASSERT( result.has_value() );
    TESTLABS_CHECK_EQ_FLT( result->position_min.x, 10.0f );
    TESTLABS_CHECK_EQ_FLT( result->position_min.y, 10.0f );
    TESTLABS_CHECK_EQ_FLT( result->position_max.x, 20.0f );
    TESTLABS_CHECK_EQ_FLT( result->position_max.y, 30.0f );
    TESTLABS_CHECK_EQ_FLT( result->uv_min.x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( result->uv_min.y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( result->uv_max.x, 1.0f );
    TESTLABS_CHECK_EQ_FLT( result->uv_max.y, 1.0f );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  meshes:  ClippedQuad:  clip - an unconstrained (infinite) clip rect is a no-op" )
{
    const auto result = xl7::graphics::meshes::ClippedQuad::clip({-5.0f, -7.0f}, {5.0f, 7.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {-infinity, -infinity}, {infinity, infinity});

    TESTLABS_ASSERT( result.has_value() );
    TESTLABS_CHECK_EQ_FLT( result->position_min.x, -5.0f );
    TESTLABS_CHECK_EQ_FLT( result->position_max.x, 5.0f );
    TESTLABS_CHECK_EQ_FLT( result->position_min.y, -7.0f );
    TESTLABS_CHECK_EQ_FLT( result->position_max.y, 7.0f );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  meshes:  ClippedQuad:  clip - fully outside on each side returns nullopt" )
{
    // To the left, right, above, and below a [0,10]x[0,10] clip rect.
    TESTLABS_CHECK( !xl7::graphics::meshes::ClippedQuad::clip({-20.0f, 0.0f}, {-10.0f, 10.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {10.0f, 10.0f}).has_value() );
    TESTLABS_CHECK( !xl7::graphics::meshes::ClippedQuad::clip({20.0f, 0.0f}, {30.0f, 10.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {10.0f, 10.0f}).has_value() );
    TESTLABS_CHECK( !xl7::graphics::meshes::ClippedQuad::clip({0.0f, -20.0f}, {10.0f, -10.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {10.0f, 10.0f}).has_value() );
    TESTLABS_CHECK( !xl7::graphics::meshes::ClippedQuad::clip({0.0f, 20.0f}, {10.0f, 30.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f}, {10.0f, 10.0f}).has_value() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  meshes:  ClippedQuad:  clip - partial clip on one side remaps UVs proportionally" )
{
    // Quad spans x in [0,10], clipped to x in [0,10] cap at 4 -> right half (x in [4,10]) is cut away.
    const auto result = xl7::graphics::meshes::ClippedQuad::clip({0.0f, 10.0f}, {10.0f, 20.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {-infinity, -infinity}, {4.0f, infinity});

    TESTLABS_ASSERT( result.has_value() );
    TESTLABS_CHECK_EQ_FLT( result->position_min.x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( result->position_max.x, 4.0f );
    TESTLABS_CHECK_EQ_FLT( result->uv_min.x, 0.0f );
    TESTLABS_CHECK_EQ_FLT( result->uv_max.x, 0.4f );
    // The unclipped axis stays untouched.
    TESTLABS_CHECK_EQ_FLT( result->position_min.y, 10.0f );
    TESTLABS_CHECK_EQ_FLT( result->position_max.y, 20.0f );
    TESTLABS_CHECK_EQ_FLT( result->uv_min.y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( result->uv_max.y, 1.0f );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  meshes:  ClippedQuad:  clip - partial clip on two sides (corner) remaps both axes" )
{
    // Quad spans [0,10]x[0,10], clipped to [2,10]x[-inf,6] -> left+bottom portion removed.
    const auto result = xl7::graphics::meshes::ClippedQuad::clip({0.0f, 0.0f}, {10.0f, 10.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {2.0f, -infinity}, {infinity, 6.0f});

    TESTLABS_ASSERT( result.has_value() );
    TESTLABS_CHECK_EQ_FLT( result->position_min.x, 2.0f );
    TESTLABS_CHECK_EQ_FLT( result->position_max.x, 10.0f );
    TESTLABS_CHECK_EQ_FLT( result->position_min.y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( result->position_max.y, 6.0f );
    TESTLABS_CHECK_EQ_FLT( result->uv_min.x, 0.2f );
    TESTLABS_CHECK_EQ_FLT( result->uv_max.x, 1.0f );
    TESTLABS_CHECK_EQ_FLT( result->uv_min.y, 0.0f );
    TESTLABS_CHECK_EQ_FLT( result->uv_max.y, 0.6f );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  meshes:  ClippedQuad:  clip - a clip reducing width or height to zero returns nullopt" )
{
    TESTLABS_CHECK( !xl7::graphics::meshes::ClippedQuad::clip({0.0f, 10.0f}, {10.0f, 20.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {-infinity, -infinity}, {0.0f, infinity}).has_value() );
    TESTLABS_CHECK( !xl7::graphics::meshes::ClippedQuad::clip({0.0f, 0.0f}, {10.0f, 10.0f}, {0.0f, 0.0f}, {1.0f, 1.0f}, {-infinity, 10.0f}, {infinity, infinity}).has_value() );
}
