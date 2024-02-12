#pragma once
#ifndef XL7_GRAPHICS_STATES_TARGETSTATES_H
#define XL7_GRAPHICS_STATES_TARGETSTATES_H
#include "./AbstractStates.h"

#include "../surfaces/ColorRenderTarget.h"
#include "../surfaces/DepthStencilTarget.h"



namespace xl7 {
namespace graphics {
namespace states {



class TargetStates
    : public AbstractStates
{

public:
    /** The maximum number of concurrent (color) render targets (ignoring hardware/driver capabilities). */
    static constexpr unsigned MAX_RENDER_TARGETS = 8;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    StateArray<resources::ResourceID, MAX_RENDER_TARGETS> _color_render_target_ids;
    SingleState<resources::ResourceID> _depth_stencil_target_id;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    resources::ResourceID get_color_render_target_id() const { return get_color_render_target_id( 0 ); }
    resources::ResourceID get_color_render_target_id(unsigned index) const;

    resources::ResourceID get_depth_stencil_target_id() const { return _depth_stencil_target_id.get_value(); }

public:
    void set_color_render_target_id(resources::ResourceID color_render_target_id) { set_color_render_target_id( 0, color_render_target_id ); }
    void set_color_render_target_id(unsigned index, resources::ResourceID color_render_target_id);

    void set_depth_stencil_target_id(resources::ResourceID depth_stencil_target_id) { _depth_stencil_target_id.set_value( depth_stencil_target_id ); }

}; // class StreamStates



} // namespace states
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_STATES_TARGETSTATES_H
