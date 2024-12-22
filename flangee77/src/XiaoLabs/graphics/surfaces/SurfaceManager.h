#pragma once
#ifndef XL7_GRAPHICS_SURFACES_SURFACEMANAGER_H
#define XL7_GRAPHICS_SURFACES_SURFACEMANAGER_H
#include "../../resources/ResourceManager.h"

#include "./ISurfaceFactory.h"



namespace xl7::graphics {
    class RenderingDevice;
namespace surfaces {



class SurfaceManager final // NOLINT(*-virtual-class-destructor)
    : public resources::ResourceManager
{

public:
    class Attorney
    {
        static SurfaceManager* create(ISurfaceFactory* factory) { return new SurfaceManager(factory); }
        static void destroy(SurfaceManager* manager) { delete manager; }
        friend class xl7::graphics::RenderingDevice;
    };



    SurfaceManager() = delete;

    SurfaceManager(const SurfaceManager&) = delete;
    SurfaceManager& operator=(const SurfaceManager&) = delete;
    SurfaceManager(SurfaceManager&&) = delete;
    SurfaceManager& operator=(SurfaceManager&&) = delete;



protected:
    SurfaceManager(ISurfaceFactory* factory) : _factory(factory) {}
    ~SurfaceManager() override = default;



private:
    /**
     * The surface factory.
     */
    ISurfaceFactory* const _factory;

}; // class SurfaceManager



} // namespace surfaces
} // namespace xl7::graphics

#endif // XL7_GRAPHICS_SURFACES_SURFACEMANAGER_H
