
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/graphics/meshes/DirtyRange.h>

#include "../../shared.h"



TESTLABS_CASE( u8"XiaoLabs:  graphics:  meshes:  DirtyRange:  new object, not dirty" )
{
    xl7::graphics::meshes::DirtyRange dirty_range;
    TESTLABS_CHECK( !dirty_range.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  meshes:  DirtyRange:  init empty, not dirty" )
{
    xl7::graphics::meshes::DirtyRange dirty_range;
    dirty_range.update( 0, 0 );
    TESTLABS_CHECK( !dirty_range.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  meshes:  DirtyRange:  init non-empty, is dirty" )
{
    xl7::graphics::meshes::DirtyRange dirty_range;
    dirty_range.update( 0, 1 );
    TESTLABS_CHECK( dirty_range.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  meshes:  DirtyRange:  init non-empty, update empty, still dirty" )
{
    xl7::graphics::meshes::DirtyRange dirty_range;
    dirty_range.update( 0, 1 );
    dirty_range.update( 0, 0 );
    TESTLABS_CHECK( dirty_range.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  meshes:  DirtyRange:  init non-empty, clear, not dirty" )
{
    xl7::graphics::meshes::DirtyRange dirty_range;
    dirty_range.update( 0, 1 );
    dirty_range.clear();
    TESTLABS_CHECK( !dirty_range.is_dirty() );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  meshes:  DirtyRange:  update without effect" )
{
    xl7::graphics::meshes::DirtyRange dirty_range;
    dirty_range.update( 20, 20 );
    dirty_range.update( 25, 10 );
    TESTLABS_CHECK( dirty_range.is_dirty() );
    TESTLABS_CHECK_EQ( dirty_range.first(), 20 );
    TESTLABS_CHECK_EQ( dirty_range.count(), 20 );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  meshes:  DirtyRange:  update with expansion" )
{
    xl7::graphics::meshes::DirtyRange dirty_range;
    dirty_range.update( 20, 20 );
    dirty_range.update( 5, 10 );
    TESTLABS_CHECK( dirty_range.is_dirty() );
    TESTLABS_CHECK_EQ( dirty_range.first(), 5 );
    TESTLABS_CHECK_EQ( dirty_range.count(), 35 );
}

TESTLABS_CASE( u8"XiaoLabs:  graphics:  meshes:  DirtyRange:  init, update, clear, not dirty" )
{
    xl7::graphics::meshes::DirtyRange dirty_range;
    dirty_range.update( 20, 20 );
    dirty_range.update( 25, 10 );
    dirty_range.update( 5, 10 );
    dirty_range.clear();
    TESTLABS_CHECK( !dirty_range.is_dirty() );
}
