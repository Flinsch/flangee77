#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE2D_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE2D_H
#include "./Texture.h"



namespace xl7::graphics::textures {



class Texture2D
    : public Texture
{

public:
    struct Desc
        : public Texture::Desc
    {
        
    };



    Texture2D() = delete;

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D(Texture2D&&) = delete;
    Texture2D& operator=(Texture2D&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"2D texture"; }

    /**
     * Returns the descriptor of the 2D texture.
     */
    const Desc& get_desc() const { return _desc; }



    /**
     * Returns an "image view" of the texture data.
     */
    images::Image as_image() const { return _as_image(0); }

    /**
     * Creates and returns mipmaps of the texture "image".
     */
    std::vector<images::Image> create_mipmaps(images::ResamplingMethod resampling_method = images::ResamplingMethod::LinearInterpolation) const { return _create_mipmaps(0, resampling_method); }

    /**
     * Updates the contents of this texture (unless it is immutable).
     */
    bool update(const ImageDataProvider& image_data_provider);



protected:
    Texture2D(const CreateParams<Desc>& params);

    ~Texture2D() override = default;



private:
    /**
     * The descriptor of the 2D texture.
     */
    const Desc _desc;

}; // class Texture2D



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE2D_H
