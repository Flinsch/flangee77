#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_TEXTURES_TEXTURE2DIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_TEXTURES_TEXTURE2DIMPL_H
#include "../../../textures/Texture2D.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
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
    const D3DFORMAT _d3d_format;

private:
    /**
     * The Direct3D 9 texture interface.
     */
    wrl::ComPtr<IDirect3DTexture9> _d3d_texture;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the Direct3D 9 texture interface.
     */
    IDirect3DTexture9* get_raw_d3d_texture() const { return _d3d_texture.Get(); }



    // #############################################################################
    // Resource Implementations
    // #############################################################################
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
     * Requests/acquires a precompiled shader resource.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const resources::DataProvider& data_provider, ChannelOrder& channel_order_out) override;

}; // class Texture2DImpl



} // namespace textures
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_TEXTURES_TEXTURE2DIMPL_H
