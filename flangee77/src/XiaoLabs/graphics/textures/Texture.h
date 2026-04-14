#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE_H
#include "../../resources/UpdatableResource.h"

#include "./TextureDesc.h"
#include "./TextureUpdater.h"

#include "../images/Image.h"
#include "../images/ResamplingMethod.h"



namespace xl7::graphics::textures {



class TextureManager;



class Texture
    : public resources::ResourceWithData<Texture>
{

public:
    enum struct Type
    {
        Texture2D,
        Texture3D,
        Texture2DArray,
        Cubemap,
    };



    Texture() = delete;

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(Texture&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"texture"; }

    /**
     * Returns the type of the texture.
     */
    Type get_type() const { return _type; }

    /**
     * Returns the descriptor of the texture.
     */
    const TextureDesc& get_desc() const { return _desc; }

    /**
     * Returns the extent of the texture, in pixels.
     */
    const TextureExtent& get_extent() const { return _desc.extent; }

    /**
     * Returns the width of the texture, in pixels.
     */
    unsigned get_width() const { return _desc.extent.width; }

    /**
     * The height of the texture, in pixels.
     */
    unsigned get_height() const { return _desc.extent.height; }

    /**
     * The depth of the texture, in pixels (i.e., the number of 2D image slices, if 3D texture, otherwise trivially 1).
     */
    unsigned get_depth() const { return _desc.extent.depth; }

    /**
     * The number of texture layers (if texture array or cubemap, otherwise trivially 1).
     */
    unsigned get_layer_count() const { return _desc.layer_count; }

    /**
     * Returns the actual channel order. This may differ from the preferred channel
     * order, depending on hardware capabilities.
     */
    ChannelOrder get_channel_order() const { return _channel_order; }

    /**
     * Returns the size of each pixel, in bytes.
     */
    unsigned get_bytes_per_pixel() const { return _bytes_per_pixel; }

    /**
     * Returns the size of a pixel row, in bytes.
     */
    unsigned get_row_pitch() const { return _row_pitch; }

    /**
     * Returns the size of a 2D image slice, in bytes.
     */
    unsigned get_slice_pitch() const { return _slice_pitch; }

    /**
     * Returns the size of a 2D image slice or a 3D volume, in bytes.
     */
    unsigned get_layer_pitch() const { return _layer_pitch; }



protected:

    Texture(const CreateContext& ctx, Type type, const TextureDesc& desc);

    ~Texture() override = default;



    /**
     * Returns the specified "image view" of the texture data.
     */
    images::Image _as_image(unsigned layer) const;

    /**
     * Creates and returns mipmaps of the specified texture "image".
     */
    std::vector<images::Image> _create_mipmaps(unsigned layer, images::ResamplingMethod resampling_method = images::ResamplingMethod::LinearInterpolation) const;



private:

    /**
     * The type of the texture.
     */
    const Type _type;

    /**
     * The descriptor of the texture.
     */
    const TextureDesc _desc;

    /**
     * The actual channel order. This may differ from the preferred channel order,
     * depending on hardware capabilities.
     */
    const ChannelOrder _channel_order;

    /**
     * The size of each pixel, in bytes.
     */
    const unsigned _bytes_per_pixel;

    /**
     * The size of a pixel row, in bytes.
     */
    const unsigned _row_pitch;

    /**
     * The size of a 2D image slice, in bytes.
     */
    const unsigned _slice_pitch;

    /**
     * The size of a 2D image slice or a 3D volume, in bytes.
     */
    const unsigned _layer_pitch;

}; // class Texture



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE_H
