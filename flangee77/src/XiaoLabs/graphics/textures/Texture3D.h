#pragma once
#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE3D_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE3D_H
#include "./Texture.h"



namespace xl7 {
namespace graphics {
namespace textures {



class Texture3D
    : public Texture
{

public:
    struct Desc
        : public Texture::Desc
    {
        /** The depth of the texture, in pixels (or the number of 2D image slices). */
        unsigned depth;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    Texture3D(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~Texture3D() = default;

private:
    /** Default constructor. */
    Texture3D() = delete;
    /** Copy constructor. */
    Texture3D(const Texture3D&) = delete;
    /** Copy assignment operator. */
    Texture3D& operator = (const Texture3D&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:
    /**
     * The descriptor of the 3D texture.
     */
    const Desc _desc;

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the descriptor of the 3D texture.
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
    virtual cl7::string_view get_type_string() const override { return TEXT("3D texture"); }

}; // class Texture3D



} // namespace textures
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE3D_H
