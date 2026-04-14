#ifndef XL7_GRAPHICS_SURFACES_SURFACE_H
#define XL7_GRAPHICS_SURFACES_SURFACE_H
#include "../../resources/ResourceBase.h"

#include "./SurfaceDesc.h"



namespace xl7::graphics::surfaces {



class SurfaceManager;



class Surface
    : public resources::ResourceBase<Surface>
{

public:
    enum struct Type
    {
        OffScreenSurface,
        RenderTargetSurface,
        DepthStencilSurface,
        TextureSurface,
    };



    Surface() = delete;

    Surface(const Surface&) = delete;
    Surface& operator=(const Surface&) = delete;
    Surface(Surface&&) = delete;
    Surface& operator=(Surface&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"surface"; }

    /**
     * Returns the type of the surface.
     */
    Type get_type() const { return _type; }

    /**
     * Returns the descriptor of the surface.
     */
    const SurfaceDesc& get_desc() const { return _desc; }



protected:

    Surface(const CreateContext& ctx, Type type, const SurfaceDesc& desc);

    ~Surface() override = default;



private:

    /**
     * The type of the surface.
     */
    const Type _type;

    /**
     * The descriptor of the surface.
     */
    const SurfaceDesc _desc;

}; // class Surface



} // namespace xl7::graphics::surfaces

#endif // XL7_GRAPHICS_SURFACES_SURFACE_H
