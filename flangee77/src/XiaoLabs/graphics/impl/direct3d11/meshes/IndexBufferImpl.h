#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_MESHES_INDEXBUFFERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_MESHES_INDEXBUFFERIMPL_H
#include "../../../meshes/IndexBuffer.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace meshes {



class IndexBufferImpl final
    : public xl7::graphics::meshes::IndexBuffer
{

public:
    class Attorney
    {
        static IndexBufferImpl* create(const CreateParams<Desc>& params) { return new IndexBufferImpl( params ); }
        friend class ResourceFactoryImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    IndexBufferImpl(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~IndexBufferImpl() = default;

private:
    /** Default constructor. */
    IndexBufferImpl() = delete;
    /** Copy constructor. */
    IndexBufferImpl(const IndexBufferImpl&) = delete;
    /** Copy assignment operator. */
    IndexBufferImpl& operator = (const IndexBufferImpl&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The Direct3D 11 device interface.
     */
    wrl::ComPtr<ID3D11DeviceN> _d3d_device;

    /**
     * The Direct3D 11 index buffer interface.
     */
    wrl::ComPtr<ID3D11Buffer> _d3d_index_buffer;



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

}; // class IndexBufferImpl



} // namespace meshes
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_MESHES_INDEXBUFFERIMPL_H
