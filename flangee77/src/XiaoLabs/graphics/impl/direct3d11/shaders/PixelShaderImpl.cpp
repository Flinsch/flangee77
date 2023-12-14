#include "PixelShaderImpl.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace shaders {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    PixelShaderImpl::PixelShaderImpl(xl7::graphics::shaders::ShaderManager* manager)
        : PixelShader( manager )
    {
    }

    /**
     * Explicit constructor.
     */
    PixelShaderImpl::PixelShaderImpl(xl7::graphics::shaders::ShaderManager* manager, const cl7::string& identifier)
        : PixelShader( manager, identifier )
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool PixelShaderImpl::_request_impl()
    {
        return false;
    }

    /**
     * Releases the resource.
     */
    bool PixelShaderImpl::_release_impl()
    {
        return false;
    }

    /**
     * Temporarily resigns some stuff to free up some (hardware) memory etc.
     */
    bool PixelShaderImpl::_resign_impl()
    {
        return false;
    }

    /**
     * Restores the resource after it has been (temporarily) resigned, returning it
     * to a usable state.
     */
    bool PixelShaderImpl::_restore_impl()
    {
        return false;
    }



} // namespace shaders
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
