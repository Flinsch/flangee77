#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_SHADERS_PIXELSHADERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_SHADERS_PIXELSHADERIMPL_H
#include "../../../shaders/PixelShader.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace shaders {



class ShaderManagerImpl;



class PixelShaderImpl final
    : public xl7::graphics::shaders::PixelShader
{

public:
    class Attorney
    {
        static PixelShaderImpl* create(ShaderManagerImpl* manager) { return new PixelShaderImpl( manager ); }
        friend class ShaderManagerImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    explicit PixelShaderImpl(ShaderManagerImpl* manager);

    /**
     * Explicit constructor.
     */
    PixelShaderImpl(ShaderManagerImpl* manager, const cl7::string& identifier);

    /**
     * Destructor.
     */
    virtual ~PixelShaderImpl() = default;

private:
    /** Default constructor. */
    PixelShaderImpl() = delete;
    /** Copy constructor. */
    PixelShaderImpl(const PixelShaderImpl&) = delete;
    /** Copy assignment operator. */
    PixelShaderImpl& operator = (const PixelShaderImpl&) = delete;



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

}; // class PixelShaderImpl



} // namespace shaders
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_SHADERS_PIXELSHADERIMPL_H
