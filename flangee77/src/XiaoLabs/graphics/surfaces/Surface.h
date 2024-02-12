#pragma once
#ifndef XL7_GRAPHICS_SURFACES_SURFACE_H
#define XL7_GRAPHICS_SURFACES_SURFACE_H
#include "../../resources/Resource.h"



namespace xl7 {
namespace graphics {
namespace surfaces {



class SurfaceManager;



class Surface
    : public resources::Resource
{

public:
    enum class Type
    {
        TextureSurface,
        ColorRenderTarget,
        DepthStencilTarget,
    };

public:
    struct Desc
    {
        
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    Surface(Type type, const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~Surface() = default;

private:
    /** Default constructor. */
    Surface() = delete;
    /** Copy constructor. */
    Surface(const Surface&) = delete;
    /** Copy assignment operator. */
    Surface& operator = (const Surface&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:
    /**
     * The type of the surface.
     */
    const Type _type;

    /**
     * The descriptor of the surface.
     */
    const Desc _desc;

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the type of the surface.
     */
    Type get_type() const { return _type; }

    /**
     * Returns the descriptor of the surface.
     */
    const Desc& get_desc() const { return _desc; }

public:



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    virtual bool _check_impl(const resources::DataProvider& data_provider) override;

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const resources::DataProvider& data_provider) override;

public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("surface"); }

}; // class Surface



} // namespace surfaces
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SURFACES_SURFACE_H
