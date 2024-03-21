#pragma once
#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE2D_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE2D_H
#include "./Texture.h"



namespace xl7 {
namespace graphics {
namespace textures {



class Texture2D
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
    Texture2D(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~Texture2D() = default;

private:
    /** Default constructor. */
    Texture2D() = delete;
    /** Copy constructor. */
    Texture2D(const Texture2D&) = delete;
    /** Copy assignment operator. */
    Texture2D& operator = (const Texture2D&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:
    /**
     * The descriptor of the 2D texture.
     */
    const Desc _desc;

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the descriptor of the 2D texture.
     */
    const Desc& get_desc() const { return _desc; }

public:



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Returns an "image view" of the texture data.
     */
    images::Image as_image() const { return _as_image( 0 ); }

    /**
     * Creates and returns mipmaps of the texture "image".
     */
    std::vector<images::Image> create_mipmaps(images::ResamplingMethod resampling_method = images::ResamplingMethod::LinearInterpolation) const { return _create_mipmaps( 0, resampling_method ); }



    // #############################################################################
    // Resource Implementations
    // #############################################################################
public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("2D texture"); }

}; // class Texture2D



} // namespace textures
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE2D_H
