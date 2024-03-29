#pragma once
#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAY_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAY_H
#include "./Texture.h"



namespace xl7 {
namespace graphics {
namespace textures {



class Texture2DArray
    : public Texture
{

public:
    struct Desc
        : public Texture::Desc
    {
        /** The number of textures in the texture array. */
        unsigned count;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    Texture2DArray(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~Texture2DArray() = default;

private:
    /** Default constructor. */
    Texture2DArray() = delete;
    /** Copy constructor. */
    Texture2DArray(const Texture2DArray&) = delete;
    /** Copy assignment operator. */
    Texture2DArray& operator = (const Texture2DArray&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:
    /**
     * The descriptor of the 2D array texture.
     */
    const Desc _desc;

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the descriptor of the 2D array texture.
     */
    const Desc& get_desc() const { return _desc; }

public:



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Returns an "image view" of the specified texture slice data.
     */
    images::Image as_image(unsigned slice_index) const { return _as_image( slice_index ); }

    /**
     * Creates and returns mipmaps of the specified texture slice "image".
     */
    std::vector<images::Image> create_mipmaps(unsigned slice_index, images::ResamplingMethod resampling_method = images::ResamplingMethod::LinearInterpolation) const { return _create_mipmaps( slice_index, resampling_method ); }



    // #############################################################################
    // Resource Implementations
    // #############################################################################
public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("2D array texture"); }

}; // class Texture2DArray



} // namespace textures
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAY_H
