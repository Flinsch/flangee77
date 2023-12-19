#include "RenderingContext.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/memory.h>



namespace xl7 {
namespace graphics {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    RenderingContext::RenderingContext(unsigned index)
        : _index( index )
        , _the_scene_is_on( false )
    {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Begins a scene.
     */
    bool RenderingContext::begin_scene()
    {
        if ( _the_scene_is_on )
        {
            if ( _index == 0 )
                LOG_WARNING( TEXT("The scene of the primary rendering context is already on.") );
            else
                LOG_WARNING( TEXT("The scene of the rendering context with 0-based index ") + cl7::to_string(_index) + TEXT(" is already on.") );
            return false;
        }

        if ( !_begin_scene_impl() )
            return false;

        _the_scene_is_on = true;
        return true;
    }

    /**
     * Ends a scene that was begun by calling begin_scene.
     */
    bool RenderingContext::end_scene()
    {
        if ( !_the_scene_is_on )
        {
            if ( _index == 0 )
                LOG_WARNING( TEXT("The scene of the primary rendering context is not on.") );
            else
                LOG_WARNING( TEXT("The scene of the rendering context with 0-based index ") + cl7::to_string(_index) + TEXT(" is not on.") );
            return false;
        }

        if ( !_end_scene_impl() )
            return false;

        _the_scene_is_on = false;
        return true;
    }



} // namespace graphics
} // namespace xl7
