#pragma once
#ifndef XL7_GRAPHICS_PIPELINE_OUTPUTMERGERSTAGE_H
#define XL7_GRAPHICS_PIPELINE_OUTPUTMERGERSTAGE_H
#include "./AbstractStage.h"

#include "../surfaces/ColorRenderTarget.h"
#include "../surfaces/DepthStencilTarget.h"

#include "../states/BlendState.h"

#include "../Color.h"



namespace xl7 {
namespace graphics {
namespace pipeline {



class OutputMergerStage
    : public AbstractStage
{

public:
    /** The maximum number of concurrent (color) render targets (ignoring hardware/driver capabilities). */
    static constexpr unsigned MAX_RENDER_TARGETS = 8;

private:
    static constexpr unsigned DIRTY_COLOR_RENDER_TARGET_FLAG_BASE   = 0x1;
    static constexpr unsigned DIRTY_COLOR_RENDER_TARGET_FLAG_MASK   = ((DIRTY_COLOR_RENDER_TARGET_FLAG_BASE << MAX_RENDER_TARGETS) - 1) & ~(DIRTY_COLOR_RENDER_TARGET_FLAG_BASE - 1);
    static constexpr unsigned DIRTY_DEPTH_STENCTIL_TARGET_FLAG      = 0x2 << (MAX_RENDER_TARGETS - 1);
    static constexpr unsigned DIRTY_BLEND_STATE_FLAG                = 0x4 << (MAX_RENDER_TARGETS - 1);
    static constexpr unsigned DIRTY_BLEND_FACTOR_FLAG               = 0x8 << (MAX_RENDER_TARGETS - 1);



    // #############################################################################
    // States
    // #############################################################################
private:
    _XL7_GRAPHICS_PIPELINE_STATE_ARRAY_DEFAULT0( color_render_target_id, MAX_RENDER_TARGETS, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_COLOR_RENDER_TARGET_FLAG_BASE );
    _XL7_GRAPHICS_PIPELINE_SINGLE_STATE( depth_stencil_target_id, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_DEPTH_STENCTIL_TARGET_FLAG );
    _XL7_GRAPHICS_PIPELINE_SINGLE_STATE( blend_state_id, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_BLEND_STATE_FLAG );
    _XL7_GRAPHICS_PIPELINE_SINGLE_STATE( blend_factor, Color, Color( 1.0f, 1.0f, 1.0f, 1.0f ), DIRTY_BLEND_FACTOR_FLAG );

}; // class OutputMergerStage



} // namespace pipeline
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_PIPELINE_OUTPUTMERGERSTAGE_H
