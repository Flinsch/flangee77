#include "VertexShaderImpl.h"

#include "./ShaderManagerImpl.h"



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
    VertexShaderImpl::VertexShaderImpl(ShaderManagerImpl* manager)
        : VertexShader( manager )
    {
    }

    /**
     * Explicit constructor.
     */
    VertexShaderImpl::VertexShaderImpl(ShaderManagerImpl* manager, const cl7::string& identifier)
        : VertexShader( manager, identifier )
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool VertexShaderImpl::_request_impl()
    {
        return false;
    }

    /**
     * Releases the resource.
     */
    bool VertexShaderImpl::_release_impl()
    {
        return false;
    }

    /**
     * Temporarily resigns some stuff to free up some (hardware) memory etc.
     */
    bool VertexShaderImpl::_resign_impl()
    {
        return false;
    }

    /**
     * Restores the resource after it has been (temporarily) resigned, returning it
     * to a usable state.
     */
    bool VertexShaderImpl::_restore_impl()
    {
        return false;
    }



} // namespace shaders
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
