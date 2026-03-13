
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/textures/DirtyRect.h>

#include "../../shared.h"



TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyRect:  new object, not dirty" )
{
    xl7::graphics::textures::DirtyRect dirty_rect;
    TESTLABS_CHECK( !dirty_rect.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyRect:  init empty, not dirty" )
{
    xl7::graphics::textures::DirtyRect dirty_rect;
    dirty_rect.update( 0, 0, 0, 0 );
    TESTLABS_CHECK( !dirty_rect.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyRect:  init empty width, not dirty" )
{
    xl7::graphics::textures::DirtyRect dirty_rect;
    dirty_rect.update( 0, 0, 0, 1 );
    TESTLABS_CHECK( !dirty_rect.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyRect:  init empty height, not dirty" )
{
    xl7::graphics::textures::DirtyRect dirty_rect;
    dirty_rect.update( 0, 0, 1, 0 );
    TESTLABS_CHECK( !dirty_rect.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyRect:  init non-empty, is dirty" )
{
    xl7::graphics::textures::DirtyRect dirty_rect;
    dirty_rect.update( 0, 0, 1, 1 );
    TESTLABS_CHECK( dirty_rect.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyRect:  init non-empty, update empty, still dirty" )
{
    xl7::graphics::textures::DirtyRect dirty_rect;
    dirty_rect.update( 0, 0, 1, 1 );
    dirty_rect.update( 0, 0, 0, 0 );
    TESTLABS_CHECK( dirty_rect.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyRect:  init non-empty, clear, not dirty" )
{
    xl7::graphics::textures::DirtyRect dirty_rect;
    dirty_rect.update( 0, 0, 1, 1 );
    dirty_rect.clear();
    TESTLABS_CHECK( !dirty_rect.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyRect:  update without effect" )
{
    xl7::graphics::textures::DirtyRect dirty_rect;
    dirty_rect.update( 20, 20, 20, 20 );
    dirty_rect.update( 25, 25, 10, 10 );
    TESTLABS_CHECK( dirty_rect.is_dirty() );
    TESTLABS_CHECK_EQ( dirty_rect.x(), 20 );
    TESTLABS_CHECK_EQ( dirty_rect.y(), 20 );
    TESTLABS_CHECK_EQ( dirty_rect.width(), 20 );
    TESTLABS_CHECK_EQ( dirty_rect.height(), 20 );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyRect:  update with expansion" )
{
    xl7::graphics::textures::DirtyRect dirty_rect;
    dirty_rect.update( 20, 20, 20, 20 );
    dirty_rect.update( 5, 10, 10, 20 );
    TESTLABS_CHECK( dirty_rect.is_dirty() );
    TESTLABS_CHECK_EQ( dirty_rect.x(), 5 );
    TESTLABS_CHECK_EQ( dirty_rect.y(), 10 );
    TESTLABS_CHECK_EQ( dirty_rect.width(), 35 );
    TESTLABS_CHECK_EQ( dirty_rect.height(), 30 );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  textures:  DirtyRect:  init, update, clear, not dirty" )
{
    xl7::graphics::textures::DirtyRect dirty_rect;
    dirty_rect.update( 20, 20, 20, 20 );
    dirty_rect.update( 25, 25, 10, 10 );
    dirty_rect.update( 5, 10, 10, 20 );
    dirty_rect.clear();
    TESTLABS_CHECK( !dirty_rect.is_dirty() );
}
