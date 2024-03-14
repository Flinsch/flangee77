#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_MESHES_VERTEXBUFFERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_MESHES_VERTEXBUFFERIMPL_H
#include "../../../meshes/VertexBuffer.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
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
    const DWORD _d3d_fvf;

private:
    /**
     * The Direct3D 9 vertex buffer interface.
     */
    wrl::ComPtr<IDirect3DVertexBuffer9> _d3d_vertex_buffer;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the Direct3D 9 vertex buffer interface.
     */
    IDirect3DVertexBuffer9* get_raw_d3d_vertex_buffer() const { return _d3d_vertex_buffer.Get(); }



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const resources::DataProvider& data_provider) override;

    /**
     * Releases/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    virtual bool _release_impl() override;



    // #############################################################################
    // VertexBuffer Implementations
    // #############################################################################
private:
    /**
     * Updates the contents of this vertex buffer (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _update_impl(const resources::DataProvider& data_provider, bool discard, bool no_overwrite) override;

}; // class VertexBufferImpl



} // namespace meshes
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_MESHES_VERTEXBUFFERIMPL_H
