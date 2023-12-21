#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_MESHES_INDEXBUFFERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_MESHES_INDEXBUFFERIMPL_H
#include "../../../meshes/IndexBuffer.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace meshes {



class IndexBufferImpl final
    : public xl7::graphics::meshes::IndexBuffer
{

public:
    class Attorney
    {
        static IndexBufferImpl* create(xl7::graphics::meshes::MeshManager* manager, const cl7::string& identifier, const Desc& desc) { return new IndexBufferImpl( manager, identifier, desc ); }
        friend class ResourceFactoryImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    IndexBufferImpl(xl7::graphics::meshes::MeshManager* manager, const cl7::string& identifier, const Desc& desc);

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
public:
    const D3DFORMAT d3d_format;

private:
    /**
     * The Direct3D 9 device interface.
     */
    wrl::ComPtr<IDirect3DDevice9> _d3d_device;

    /**
     * The Direct3D 9 index buffer interface.
     */
    wrl::ComPtr<IDirect3DIndexBuffer9> _d3d_index_buffer;



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



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Creates the Direct3D 9 index buffer interface.
     */
    bool _create_index_buffer();

}; // class IndexBufferImpl



} // namespace meshes
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_MESHES_INDEXBUFFERIMPL_H
