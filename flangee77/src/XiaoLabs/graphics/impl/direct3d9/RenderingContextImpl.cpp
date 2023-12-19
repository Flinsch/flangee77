#include "RenderingContextImpl.h"

#include "./errors.h"

#include <XiaoLabs/graphics.h>

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    RenderingContextImpl::RenderingContextImpl(unsigned index, wrl::ComPtr<IDirect3DDevice9> d3d_device)
        : RenderingContext( index )
        , _d3d_device( d3d_device )
    {
        assert( index == 0 );
        assert( d3d_device );
    }



    // #############################################################################
    // RenderingContext Implementations
    // #############################################################################

    /**
     * Begins a scene.
     */
    bool RenderingContextImpl::_begin_scene_impl()
    {
        HRESULT hresult = _d3d_device->BeginScene();
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::BeginScene") ) );
            return false;
        }

        return true;
    }

    /**
     * Ends a scene that was begun by calling begin_scene.
     */
    bool RenderingContextImpl::_end_scene_impl()
    {
        HRESULT hresult = _d3d_device->EndScene();
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::EndScene") ) );
            return false;
        }

        return true;
    }



} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
