#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_MESHES_MESHMANAGERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_MESHES_MESHMANAGERIMPL_H
#include "../../../meshes/MeshManager.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace meshes {



class MeshManagerImpl final
    : public xl7::graphics::meshes::MeshManager
{

public:
    class Attorney
    {
        static MeshManagerImpl* create() { return new MeshManagerImpl(); }
        static void destroy(MeshManager* mesh_manager) { _destroy( mesh_manager ); }
        friend class RenderingDeviceImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    MeshManagerImpl();

    /**
     * Destructor.
     */
    virtual ~MeshManagerImpl() = default;

private:
    /** Copy constructor. */
    MeshManagerImpl(const MeshManagerImpl&) = delete;
    /** Copy assignment operator. */
    MeshManagerImpl& operator = (const MeshManagerImpl&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:



    // #############################################################################
    // Properties
    // #############################################################################
public:



    // #############################################################################
    // Methods
    // #############################################################################
public:



    // #############################################################################
    // MeshManager Implementations
    // #############################################################################
private:



    // #############################################################################
    // Helpers
    // #############################################################################
private:

}; // class MeshManagerImpl



} // namespace meshes
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_MESHES_MESHMANAGERIMPL_H
