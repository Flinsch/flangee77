#pragma once
#ifndef XL7_GRAPHICS_SURFACES_SURFACEMANAGER_H
#define XL7_GRAPHICS_SURFACES_SURFACEMANAGER_H
#include "../../resources/ResourceManager.h"

#include "./ISurfaceFactory.h"



namespace xl7 {
namespace graphics {
    class RenderingDevice;
namespace surfaces {



class SurfaceManager
    : public resources::ResourceManager
{

public:
    class Attorney
    {
        static SurfaceManager* create(ISurfaceFactory* factory) { return new SurfaceManager( factory ); }
        static void destroy(SurfaceManager* manager) { delete manager; }
        friend class RenderingDevice;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    SurfaceManager(ISurfaceFactory* factory);

    /**
     * Destructor.
     */
    virtual ~SurfaceManager();

private:
    /** Default constructor. */
    SurfaceManager() = delete;
    /** Copy constructor. */
    SurfaceManager(const SurfaceManager&) = delete;
    /** Copy assignment operator. */
    SurfaceManager& operator = (const SurfaceManager&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The surface factory.
     */
    ISurfaceFactory* const _factory;



    // #############################################################################
    // Methods
    // #############################################################################
public:

}; // class SurfaceManager



} // namespace surfaces
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SURFACES_SURFACEMANAGER_H
