#pragma once
#ifndef XL7_GRAPHICS_SURFACES_SURFACEMANAGER_H
#define XL7_GRAPHICS_SURFACES_SURFACEMANAGER_H
#include "../../ResourceManager.h"



namespace xl7 {
namespace graphics {
namespace surfaces {



class SurfaceManager
    : public ResourceManager
{

protected:
    static void _destroy(SurfaceManager* surface_manager) { delete surface_manager; }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    SurfaceManager();

    /**
     * Destructor.
     */
    virtual ~SurfaceManager();

private:
    /** Copy constructor. */
    SurfaceManager(const SurfaceManager&) = delete;
    /** Copy assignment operator. */
    SurfaceManager& operator = (const SurfaceManager&) = delete;



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

}; // class SurfaceManager



} // namespace surfaces
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SURFACES_SURFACEMANAGER_H
