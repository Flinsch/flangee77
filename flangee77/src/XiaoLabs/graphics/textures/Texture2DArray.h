#pragma once
#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAY_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAY_H
#include "./Texture.h"



namespace xl7::graphics::textures {



class Texture2DArray // NOLINT(*-virtual-class-destructor)
    : public Texture
{

public:
    struct Desc
        : public Texture::Desc
    {
        /** The number of textures in the texture array. */
        unsigned count;
    };



    Texture2DArray() = delete;

    Texture2DArray(const Texture2DArray&) = delete;
    Texture2DArray& operator = (const Texture2DArray&) = delete;
    Texture2DArray(Texture2DArray&&) = delete;
    Texture2DArray& operator = (Texture2DArray&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::string_view get_type_string() const override { return TEXT("2D array texture"); }

    /**
     * Returns the descriptor of the 2D array texture.
     */
    const Desc& get_desc() const { return _desc; }



    /**
     * Returns an "image view" of the specified texture slice data.
     */
    images::Image as_image(unsigned slice_index) const { return _as_image(slice_index); }

    /**
     * Creates and returns mipmaps of the specified texture slice "image".
     */
    std::vector<images::Image> create_mipmaps(unsigned slice_index, images::ResamplingMethod resampling_method = images::ResamplingMethod::LinearInterpolation) const { return _create_mipmaps(slice_index, resampling_method); }



protected:
    Texture2DArray(const CreateParams<Desc>& params);

    ~Texture2DArray() override = default;



private:
    /**
     * The descriptor of the 2D array texture.
     */
    const Desc _desc;

}; // class Texture2DArray



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAY_H
