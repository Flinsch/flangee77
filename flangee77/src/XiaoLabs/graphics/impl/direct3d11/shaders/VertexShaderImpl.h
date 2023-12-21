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
        static VertexShaderImpl* create(xl7::graphics::shaders::ShaderManager* manager, const cl7::string& identifier) { return new VertexShaderImpl( manager, identifier ); }
        friend class ResourceFactoryImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    VertexShaderImpl(xl7::graphics::shaders::ShaderManager* manager, const cl7::string& identifier);

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
    virtual bool _request_impl() override;

    /**
     * Releases the resource.
     */
    virtual bool _release_impl() override;

    /**
     * Temporarily resigns some stuff to free up some (hardware) memory etc.
     */
    virtual bool _resign_impl() override;

    /**
     * Restores the resource after it has been (temporarily) resigned, returning it
     * to a usable state.
     */
    virtual bool _restore_impl() override;

}; // class VertexShaderImpl



} // namespace shaders
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_SHADERS_VERTEXSHADERIMPL_H
