#pragma once
#ifndef XL7_GRAPHICS_TEXTURES_CUBEMAP_H
#define XL7_GRAPHICS_TEXTURES_CUBEMAP_H
#include "./Texture.h"



namespace xl7 {
namespace graphics {
namespace textures {



class Cubemap
    : public Texture
{

public:
    struct Desc
        : public Texture::Desc
    {
        
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    Cubemap(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~Cubemap() = default;

private:
    /** Default constructor. */
    Cubemap() = delete;
    /** Copy constructor. */
    Cubemap(const Cubemap&) = delete;
    /** Copy assignment operator. */
    Cubemap& operator = (const Cubemap&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:
    /**
     * The descriptor of the cubemap.
     */
    const Desc _desc;

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the descriptor of the cubemap.
     */
    const Desc& get_desc() const { return _desc; }

public:



    // #############################################################################
    // Resource Implementations
    // #############################################################################
public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("cubemap"); }

}; // class Cubemap



} // namespace textures
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_TEXTURES_CUBEMAP_H
