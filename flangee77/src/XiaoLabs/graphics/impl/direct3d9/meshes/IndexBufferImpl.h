#ifndef XL7_GRAPHICS_IMPL_D3D9_MESHES_INDEXBUFFERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_MESHES_INDEXBUFFERIMPL_H
#include "../../../meshes/IndexBuffer.h"

#include "../prerequisites.h"



namespace xl7::graphics::impl::direct3d9 {
    class ResourceFactoryImpl;
namespace meshes {



class IndexBufferImpl final
    : public xl7::graphics::meshes::IndexBuffer
{

public:
    class Attorney
    {
        static IndexBufferImpl* create(const CreateParams<Desc>& params) { return new IndexBufferImpl(params); }
        friend class xl7::graphics::impl::direct3d9::ResourceFactoryImpl;
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

    IndexBufferImpl(const CreateParams<Desc>& params);
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
    // IndexBuffer Implementations
    // #############################################################################

    /**
     * Updates the contents of this index buffer (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool _update_impl(const resources::DataProvider& data_provider, bool discard, bool no_overwrite) override;



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
