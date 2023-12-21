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
        static VertexBufferImpl* create(xl7::graphics::meshes::MeshManager* manager, const cl7::string& identifier, const Desc& desc) { return new VertexBufferImpl( manager, identifier, desc ); }
        friend class ResourceFactoryImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    VertexBufferImpl(xl7::graphics::meshes::MeshManager* manager, const cl7::string& identifier, const Desc& desc);

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
public:
    const DWORD d3d_fvf;

private:
    /**
     * The Direct3D 9 device interface.
     */
    wrl::ComPtr<IDirect3DDevice9> _d3d_device;

    /**
     * The Direct3D 9 vertex buffer interface.
     */
    wrl::ComPtr<IDirect3DVertexBuffer9> _d3d_vertex_buffer;



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
     * Creates the Direct3D 9 vertex buffer interface.
     */
    bool _create_vertex_buffer();

}; // class VertexBufferImpl



} // namespace meshes
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_MESHES_VERTEXBUFFERIMPL_H
