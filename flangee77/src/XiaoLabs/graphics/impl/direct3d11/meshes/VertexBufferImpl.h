#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_MESHES_VERTEXBUFFERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_MESHES_VERTEXBUFFERIMPL_H
#include "../../../meshes/VertexBuffer.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace meshes {



class VertexBufferImpl final
    : public xl7::graphics::meshes::VertexBuffer
{

public:
    class Attorney
    {
        static VertexBufferImpl* create(const CreateParams<Desc>& params) { return new VertexBufferImpl( params ); }
        friend class ResourceFactoryImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    VertexBufferImpl(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~VertexBufferImpl() = default;

private:
    /** Default constructor. */
    VertexBufferImpl() = delete;
    /** Copy constructor. */
    VertexBufferImpl(const VertexBufferImpl&) = delete;
    /** Copy assignment operator. */
    VertexBufferImpl& operator = (const VertexBufferImpl&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The Direct3D 11 device interface.
     */
    wrl::ComPtr<ID3D11DeviceN> _d3d_device;

    /**
     * The Direct3D 11 vertex buffer interface.
     */
    wrl::ComPtr<ID3D11Buffer> _d3d_vertex_buffer;



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

}; // class VertexBufferImpl



} // namespace meshes
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_MESHES_VERTEXBUFFERIMPL_H
