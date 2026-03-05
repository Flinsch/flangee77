#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAY_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAY_H
#include "./Texture.h"

#include "./Texture2DArrayDesc.h"



namespace xl7::graphics::textures {



class Texture2DArray
    : public resources::detail::ResourceBase<Texture2DArray, Texture>
{

public:
    Texture2DArray() = delete;

    Texture2DArray(const Texture2DArray&) = delete;
    Texture2DArray& operator=(const Texture2DArray&) = delete;
    Texture2DArray(Texture2DArray&&) = delete;
    Texture2DArray& operator=(Texture2DArray&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"2D array texture"; }

    /**
     * Returns the descriptor of the 2D array texture.
     */
    const Texture2DArrayDesc& get_desc() const { return _desc; }



    /**
     * Returns an "image view" of the specified texture slice data.
     */
    images::Image as_image(unsigned slice_index) const { return _as_image(slice_index); }

    /**
     * Creates and returns mipmaps of the specified texture slice "image".
     */
    std::vector<images::Image> create_mipmaps(unsigned slice_index, images::ResamplingMethod resampling_method = images::ResamplingMethod::LinearInterpolation) const { return _create_mipmaps(slice_index, resampling_method); }



protected:

    explicit Texture2DArray(const CreateParams<Texture2DArrayDesc>& params);

    ~Texture2DArray() override = default;



private:
    /**
     * The descriptor of the 2D array texture.
     */
    const Texture2DArrayDesc _desc;

}; // class Texture2DArray



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAY_H
