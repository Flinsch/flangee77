#pragma once
#ifndef XL7_GRAPHICS_PIPELINE_RASTERIZERSTAGE_H
#define XL7_GRAPHICS_PIPELINE_RASTERIZERSTAGE_H
#include "./AbstractStage.h"

#include "../states/RasterizerState.h"



namespace xl7 {
namespace graphics {
namespace pipeline {



class RasterizerStage
    : public AbstractStage
{

private:
    static constexpr unsigned DIRTY_RASTERIER_STATE_FLAG    = 0x1;



    // #############################################################################
    // States
    // #############################################################################
private:
    _XL7_GRAPHICS_PIPELINE_SINGLE_STATE( rasterizer_state_id, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_RASTERIER_STATE_FLAG );

}; // class RasterizerStage



} // namespace pipeline
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_PIPELINE_RASTERIZERSTAGE_H
