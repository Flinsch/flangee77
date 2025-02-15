#ifndef XL7_GRAPHICS_SURFACES_SURFACE_H
#define XL7_GRAPHICS_SURFACES_SURFACE_H
#include "../../resources/Resource.h"



namespace xl7::graphics::surfaces {



class SurfaceManager;



class Surface
    : public resources::Resource
{

public:
    XL7_DECLARE_RESOURCE_ID();

    enum class Type
    {
        OffScreenSurface,
        RenderTargetSurface,
        DepthStencilSurface,
        TextureSurface,
    };

    struct Desc
    {
        /** The width of the surface, in pixels. */
        unsigned width;
        /** The height of the surface, in pixels. */
        unsigned height;
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
    const Desc& get_desc() const { return _desc; }



protected:
    Surface(Type type, const CreateParams<Desc>& params)
        : Resource(params)
        , _type(type)
        , _desc(params.desc)
    {
    }

    ~Surface() override = default;



private:

    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool _check_data_impl(const resources::DataProvider& data_provider) override;

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool _acquire_impl(const resources::DataProvider& data_provider) override;



    /**
     * The type of the surface.
     */
    const Type _type;

    /**
     * The descriptor of the surface.
     */
    const Desc _desc;

}; // class Surface



} // namespace xl7::graphics::surfaces

#endif // XL7_GRAPHICS_SURFACES_SURFACE_H
