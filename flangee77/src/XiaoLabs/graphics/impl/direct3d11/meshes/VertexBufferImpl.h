#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_MESHES_VERTEXBUFFERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_MESHES_VERTEXBUFFERIMPL_H
#include "../../../meshes/VertexBuffer.h"

#include "../prerequisites.h"



namespace xl7::graphics::impl::direct3d11 {
    class ResourceFactoryImpl;
namespace meshes {



class VertexBufferImpl final
    : public xl7::graphics::meshes::VertexBuffer
{

public:
    class Attorney
    {
        static VertexBufferImpl* create(const CreateParams<Desc>& params) { return new VertexBufferImpl(params); }
        friend class xl7::graphics::impl::direct3d11::ResourceFactoryImpl;
    };



    VertexBufferImpl() = delete;

    VertexBufferImpl(const VertexBufferImpl&) = delete;
    VertexBufferImpl& operator = (const VertexBufferImpl&) = delete;
    VertexBufferImpl(VertexBufferImpl&&) = delete;
    VertexBufferImpl& operator = (VertexBufferImpl&&) = delete;



    /**
     * Returns the Direct3D 11 vertex buffer interface.
     */
    ID3D11Buffer* get_raw_d3d_vertex_buffer() const { return _d3d_vertex_buffer.Get(); }



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    VertexBufferImpl(const CreateParams<Desc>& params);
    ~VertexBufferImpl() override = default;



private:

    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    void* _get_raw_resource_impl() const override { return _d3d_vertex_buffer.Get(); }

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool _acquire_impl(const resources::DataProvider& data_provider) override;

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool _dispose_impl() override;



    // #############################################################################
    // VertexBuffer Implementations
    // #############################################################################

    /**
     * Updates the contents of this vertex buffer (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool _update_impl(const resources::DataProvider& data_provider, bool discard, bool no_overwrite) override;



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The Direct3D 11 vertex buffer interface.
     */
    wrl::ComPtr<ID3D11Buffer> _d3d_vertex_buffer;

}; // class VertexBufferImpl



} // namespace meshes
} // namespace xl7::graphics::impl::direct3d11

#endif // XL7_GRAPHICS_IMPL_D3D11_MESHES_VERTEXBUFFERIMPL_H
