#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_TEXTURES_TEXTURE2DIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_TEXTURES_TEXTURE2DIMPL_H
#include "../../../textures/Texture2D.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace textures {



class Texture2DImpl final
    : public xl7::graphics::textures::Texture2D
{

public:
    class Attorney
    {
        static Texture2DImpl* create(const CreateParams<Desc>& params) { return new Texture2DImpl( params ); }
        friend class ResourceFactoryImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    Texture2DImpl(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~Texture2DImpl() = default;

private:
    /** Default constructor. */
    Texture2DImpl() = delete;
    /** Copy constructor. */
    Texture2DImpl(const Texture2DImpl&) = delete;
    /** Copy assignment operator. */
    Texture2DImpl& operator = (const Texture2DImpl&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    const DXGI_FORMAT _dxgi_format;

private:
    /**
     * The Direct3D 11 texture interface.
     */
    wrl::ComPtr<ID3D11Texture2D> _d3d_texture;

    /**
     * The Direct3D 11 shader resource view interface.
     */
    wrl::ComPtr<ID3D11ShaderResourceView> _d3d_shader_resource_view;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the Direct3D 11 texture interface.
     */
    ID3D11Texture2D* get_raw_d3d_texture() const { return _d3d_texture.Get(); }

    /**
     * Returns the Direct3D 11 shader resource view interface.
     */
    ID3D11ShaderResourceView* get_raw_d3d_shader_resource_view() const { return _d3d_shader_resource_view.Get(); }



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    virtual void* _get_raw_resource_impl() const override { return _d3d_shader_resource_view.Get(); }

private:
    /**
     * Releases/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    virtual bool _release_impl() override;



    // #############################################################################
    // Texture2D Implementations
    // #############################################################################
private:
    /**
     * Requests/acquires the texture resource.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const xl7::graphics::textures::ImageDataProvider& image_data_provider) override;

    /**
     * Updates the contents of this texture (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _update_impl(const xl7::graphics::textures::ImageDataProvider& image_data_provider, bool discard, bool no_overwrite) override;

}; // class Texture2DImpl



} // namespace textures
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_TEXTURES_TEXTURE2DIMPL_H
