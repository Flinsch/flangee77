#pragma once
#ifndef XL7_GRAPHICS_MESHES_MESHMANAGER_H
#define XL7_GRAPHICS_MESHES_MESHMANAGER_H
#include "../../ResourceManager.h"

#include "./IMeshFactory.h"



namespace xl7 {
namespace graphics {
    class RenderingDevice;
namespace meshes {



class MeshManager
    : public ResourceManager
{

public:
    class Attorney
    {
        static MeshManager* create(IMeshFactory* factory) { return new MeshManager( factory ); }
        static void destroy(MeshManager* manager) { delete manager; }
        friend class RenderingDevice;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    MeshManager(IMeshFactory* factory);

    /**
     * Destructor.
     */
    virtual ~MeshManager();

private:
    /** Default constructor. */
    MeshManager() = delete;
    /** Copy constructor. */
    MeshManager(const MeshManager&) = delete;
    /** Copy assignment operator. */
    MeshManager& operator = (const MeshManager&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The mesh factory.
     */
    IMeshFactory* const _factory;



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
