#include "TargetStates.h"



namespace xl7 {
namespace graphics {
namespace states {



    // #############################################################################
    // Properties
    // #############################################################################

    resources::ResourceID TargetStates::get_color_render_target_id(unsigned index) const
    {
        if ( index >= MAX_RENDER_TARGETS )
            return resources::ResourceID();

        return _color_render_target_ids.get_value( static_cast<size_t>( index ) );
    }

    void TargetStates::set_color_render_target_id(unsigned index, resources::ResourceID color_render_target_id)
    {
        if ( index >= MAX_RENDER_TARGETS )
            return;

        _color_render_target_ids.set_value( static_cast<size_t>( index ), color_render_target_id );
    }



} // namespace states
} // namespace graphics
} // namespace xl7
