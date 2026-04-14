#ifndef XL7_GRAPHICS_IMPL_D3D9_MESHES_INDEXBUFFERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_MESHES_INDEXBUFFERIMPL_H
#include "../../../meshes/IndexBuffer.h"

#include "../prerequisites.h"



namespace xl7::graphics::impl::direct3d9 {
    class ResourceFactoryImpl;
namespace meshes {



class IndexBufferImpl final
    : public graphics::meshes::IndexBuffer
{

public:
    class Attorney
    {
        static IndexBufferImpl* create(const CreateContext& ctx, const graphics::meshes::IndexBufferDesc& desc) { return new IndexBufferImpl(ctx, desc); }
        friend class direct3d9::ResourceFactoryImpl;
    };



    IndexBufferImpl() = delete;

    IndexBufferImpl(const IndexBufferImpl&) = delete;
    IndexBufferImpl& operator=(const IndexBufferImpl&) = delete;
    IndexBufferImpl(IndexBufferImpl&&) = delete;
    IndexBufferImpl& operator=(IndexBufferImpl&&) = delete;



    /**
     * Returns the Direct3D 9 index buffer interface.
     */
    IDirect3DIndexBuffer9* get_raw_d3d_index_buffer() const { return _d3d_index_buffer.Get(); }



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    explicit IndexBufferImpl(const CreateContext& ctx, const graphics::meshes::IndexBufferDesc& desc);
    ~IndexBufferImpl() override = default;



private:

    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    void* _get_raw_resource_impl() const override { return _d3d_index_buffer.Get(); }

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool _acquire_impl() override;

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state before this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool _dispose_impl() override;

    /**
     * Flushes recent changes made to the local data copy by transferring them
     * "dirty" parts to the hardware and returns true after a successful transfer.
     */
    bool _flush_data_impl() override;



    // #############################################################################
    // Attributes
    // #############################################################################

    const D3DFORMAT _d3d_format;

    /**
     * The Direct3D 9 index buffer interface.
     */
    wrl::ComPtr<IDirect3DIndexBuffer9> _d3d_index_buffer;

}; // class IndexBufferImpl



} // namespace meshes
} // namespace xl7::graphics::impl::direct3d9

#endif // XL7_GRAPHICS_IMPL_D3D9_MESHES_INDEXBUFFERIMPL_H
