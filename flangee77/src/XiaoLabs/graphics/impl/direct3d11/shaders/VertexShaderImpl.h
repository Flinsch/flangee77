#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_SHADERS_VERTEXSHADERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_SHADERS_VERTEXSHADERIMPL_H
#include "../../../shaders/VertexShader.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace shaders {



class VertexShaderImpl final
    : public xl7::graphics::shaders::VertexShader
{

public:
    class Attorney
    {
        static VertexShaderImpl* create(const CreateParams<Desc>& params) { return new VertexShaderImpl( params ); }
        friend class ResourceFactoryImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    VertexShaderImpl(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~VertexShaderImpl() = default;

private:
    /** Default constructor. */
    VertexShaderImpl() = delete;
    /** Copy constructor. */
    VertexShaderImpl(const VertexShaderImpl&) = delete;
    /** Copy assignment operator. */
    VertexShaderImpl& operator = (const VertexShaderImpl&) = delete;



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    virtual bool _acquire_impl() override;

    /**
     * Releases/"unacquires" the resource.
     */
    virtual bool _release_impl() override;



    // #############################################################################
    // Shader Implementations
    // #############################################################################
private:
    /**
    * Recompiles the shader code. This tends to result in the resource having to be
    * completely recreated in the background.
    */
    virtual bool _recompile_impl() override;

}; // class VertexShaderImpl



} // namespace shaders
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_SHADERS_VERTEXSHADERIMPL_H
