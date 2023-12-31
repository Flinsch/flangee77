#include "PixelShaderImpl.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace shaders {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    PixelShaderImpl::PixelShaderImpl(const CreateParams<Desc>& params)
        : PixelShader( params )
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool PixelShaderImpl::_acquire_impl()
    {
        return false;
    }

    /**
     * Releases/"unacquires" the resource.
     */
    bool PixelShaderImpl::_release_impl()
    {
        return false;
    }



    // #############################################################################
    // Shader Implementations
    // #############################################################################

    /**
    * Recompiles the shader code. This tends to result in the resource having to be
    * completely recreated in the background.
    */
    bool PixelShaderImpl::_recompile_impl()
    {
        return false;
    }



} // namespace shaders
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
