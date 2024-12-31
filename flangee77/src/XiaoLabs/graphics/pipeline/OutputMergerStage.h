#ifndef XL7_GRAPHICS_PIPELINE_OUTPUTMERGERSTAGE_H
#define XL7_GRAPHICS_PIPELINE_OUTPUTMERGERSTAGE_H
#include "./AbstractStage.h"

#include "../surfaces/ColorRenderTarget.h"
#include "../surfaces/DepthStencilTarget.h"

#include "../states/DepthStencilState.h"
#include "../states/BlendState.h"

#include "../Color.h"



namespace xl7::graphics::pipeline {



class OutputMergerStage
    : public AbstractStage
{

public:
    /** The maximum number of concurrent (color) render targets (ignoring hardware/driver capabilities). */
    static constexpr unsigned MAX_RENDER_TARGETS = 8;



private:
    static constexpr unsigned DIRTY_COLOR_RENDER_TARGET_FLAG_BASE   = 0x01;
    static constexpr unsigned DIRTY_COLOR_RENDER_TARGET_FLAG_MASK   = ((DIRTY_COLOR_RENDER_TARGET_FLAG_BASE << MAX_RENDER_TARGETS) - 1) & ~(DIRTY_COLOR_RENDER_TARGET_FLAG_BASE - 1);
    static constexpr unsigned DIRTY_DEPTH_STENCTIL_TARGET_FLAG      = 0x02 << (MAX_RENDER_TARGETS - 1);
    static constexpr unsigned DIRTY_DEPTH_STENCIL_STATE_FLAG        = 0x04 << (MAX_RENDER_TARGETS - 1);
    static constexpr unsigned DIRTY_STENCIL_REFERENCE_VALUE_FLAG    = 0x08 << (MAX_RENDER_TARGETS - 1);
    static constexpr unsigned DIRTY_BLEND_STATE_FLAG                = 0x10 << (MAX_RENDER_TARGETS - 1);
    static constexpr unsigned DIRTY_BLEND_FACTOR_FLAG               = 0x20 << (MAX_RENDER_TARGETS - 1);



    XL7_GRAPHICS_PIPELINE_STATE_ARRAY_DEFAULT0(color_render_target_id, MAX_RENDER_TARGETS, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_COLOR_RENDER_TARGET_FLAG_BASE);
    XL7_GRAPHICS_PIPELINE_SINGLE_STATE(depth_stencil_target_id, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_DEPTH_STENCTIL_TARGET_FLAG);
    XL7_GRAPHICS_PIPELINE_SINGLE_STATE(depth_stencil_state_id, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_DEPTH_STENCIL_STATE_FLAG);
    XL7_GRAPHICS_PIPELINE_SINGLE_STATE(stencil_reference_value, unsigned, 0x00000000, DIRTY_STENCIL_REFERENCE_VALUE_FLAG);
    XL7_GRAPHICS_PIPELINE_SINGLE_STATE(blend_state_id, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_BLEND_STATE_FLAG);
    XL7_GRAPHICS_PIPELINE_SINGLE_STATE(blend_factor, Color, Color(1.0f, 1.0f, 1.0f, 1.0f), DIRTY_BLEND_FACTOR_FLAG);

}; // class OutputMergerStage



} // namespace xl7::graphics::pipeline

#endif // XL7_GRAPHICS_PIPELINE_OUTPUTMERGERSTAGE_H
