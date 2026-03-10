#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE3D_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE3D_H
#include "./Texture.h"

#include "./Texture3DDesc.h"
#include "./DirtyBox.h"



namespace xl7::graphics::textures {



class Texture3D
    : public resources::detail::ResourceBaseDirty<Texture3D, DirtyBox, Texture>
{

public:
    Texture3D() = delete;

    Texture3D(const Texture3D&) = delete;
    Texture3D& operator=(const Texture3D&) = delete;
    Texture3D(Texture3D&&) = delete;
    Texture3D& operator=(Texture3D&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"3D texture"; }

    /**
     * Returns the descriptor of the 3D texture.
     */
    const Texture3DDesc& get_desc() const { return _desc; }



    /**
     * Returns an "image view" of the texture data.
     */
    images::Image as_image() const { return _as_image(0); }

    /**
     * Creates and returns mipmaps of the texture "image".
     */
    std::vector<images::Image> create_mipmaps(images::ResamplingMethod resampling_method = images::ResamplingMethod::LinearInterpolation) const { return _create_mipmaps(0, resampling_method); }



protected:

    explicit Texture3D(const CreateContext& ctx, const Texture3DDesc& desc);

    ~Texture3D() override = default;



private:
    /**
     * The descriptor of the 3D texture.
     */
    const Texture3DDesc _desc;

}; // class Texture3D



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE3D_H
