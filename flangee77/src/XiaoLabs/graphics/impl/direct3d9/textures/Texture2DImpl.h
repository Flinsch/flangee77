#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_TEXTURES_TEXTURE2DIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_TEXTURES_TEXTURE2DIMPL_H
#include "../../../textures/Texture2D.h"

#include "../prerequisites.h"



namespace xl7::graphics::impl::direct3d9 {
    class ResourceFactoryImpl;
namespace textures {



class Texture2DImpl final
    : public xl7::graphics::textures::Texture2D
{

public:
    class Attorney
    {
        static Texture2DImpl* create(const CreateParams<Desc>& params) { return new Texture2DImpl(params); }
        friend class xl7::graphics::impl::direct3d9::ResourceFactoryImpl;
    };



    Texture2DImpl() = delete;

    Texture2DImpl(const Texture2DImpl&) = delete;
    Texture2DImpl& operator = (const Texture2DImpl&) = delete;
    Texture2DImpl(Texture2DImpl&&) = delete;
    Texture2DImpl& operator = (Texture2DImpl&&) = delete;



    /**
     * Returns the Direct3D 9 texture interface.
     */
    IDirect3DTexture9* get_raw_d3d_texture() const { return _d3d_texture.Get(); }



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Texture2DImpl(const CreateParams<Desc>& params);
    ~Texture2DImpl() override = default;



private:

    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    void* _get_raw_resource_impl() const override { return _d3d_texture.Get(); }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool _dispose_impl() override;



    // #############################################################################
    // Texture2D Implementations
    // #############################################################################

    /**
     * Requests/acquires the texture resource.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool _acquire_impl(const xl7::graphics::textures::ImageDataProvider& image_data_provider) override;

    /**
     * Updates the contents of this texture (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool _update_impl(const xl7::graphics::textures::ImageDataProvider& image_data_provider, bool discard, bool no_overwrite) override;



    // #############################################################################
    // Attributes
    // #############################################################################

    const D3DFORMAT _d3d_format;

    /**
     * The Direct3D 9 texture interface.
     */
    wrl::ComPtr<IDirect3DTexture9> _d3d_texture;

}; // class Texture2DImpl



} // namespace textures
} // namespace xl7::graphics::impl::direct3d9

#endif // XL7_GRAPHICS_IMPL_D3D9_TEXTURES_TEXTURE2DIMPL_H
