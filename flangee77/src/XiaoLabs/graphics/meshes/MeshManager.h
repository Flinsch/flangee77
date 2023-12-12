#pragma once
#ifndef XL7_GRAPHICS_MESHES_MESHMANAGER_H
#define XL7_GRAPHICS_MESHES_MESHMANAGER_H
#include "../../ResourceManager.h"



namespace xl7 {
namespace graphics {
namespace meshes {



class MeshManager
    : public ResourceManager
{

protected:
    static void _destroy(MeshManager* mesh_manager) { delete mesh_manager; }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    MeshManager();

    /**
     * Destructor.
     */
    virtual ~MeshManager();

private:
    /** Copy constructor. */
    MeshManager(const MeshManager&) = delete;
    /** Copy assignment operator. */
    MeshManager& operator = (const MeshManager&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:



    // #############################################################################
    // Methods
    // #############################################################################
public:



    // #############################################################################
    // Prototypes
    // #############################################################################
private:

}; // class MeshManager



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_MESHMANAGER_H
