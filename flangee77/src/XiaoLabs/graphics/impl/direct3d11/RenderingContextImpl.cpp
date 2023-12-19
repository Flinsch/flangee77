#include "RenderingContextImpl.h"

#include "./errors.h"

#include <XiaoLabs/graphics.h>

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    RenderingContextImpl::RenderingContextImpl(unsigned index, wrl::ComPtr<ID3D11DeviceContextN> d3d_device_context)
        : RenderingContext( index )
        , _d3d_device_context( d3d_device_context )
    {
        assert( d3d_device_context );
    }



    // #############################################################################
    // RenderingContext Implementations
    // #############################################################################

    /**
     * Begins a scene.
     */
    bool RenderingContextImpl::_begin_scene_impl()
    {
        // Nothing to do here?

        return true;
    }

    /**
     * Ends a scene that was begun by calling begin_scene.
     */
    bool RenderingContextImpl::_end_scene_impl()
    {
        // Nothing to do here?

        return true;
    }



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
