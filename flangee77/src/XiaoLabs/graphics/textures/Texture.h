#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE_H
#include "../../resources/Resource.h"

#include "./TextureUsage.h"

#include "../PixelFormat.h"
#include "../ChannelOrder.h"

#include "../images/ResamplingMethod.h"

#include "./ImageDataProvider.h"



namespace xl7::graphics::textures {



class TextureManager;



class Texture
    : public resources::detail::ResourceBase<Texture>
{

public:
    enum struct Type
    {
        Texture2D,
        Texture3D,
        Texture2DArray,
        Cubemap,
    };

    struct Desc
    {
        /** Identifies how the texture is expected to be updated (frequency of update is a key factor). */
        TextureUsage usage;

        /** The pixel format. */
        PixelFormat pixel_format;
        /** The preferred channel order. (The actual channel order may vary depending on hardware capabilities.) */
        ChannelOrder preferred_channel_order;

        /** The maximum number of mipmap levels in the texture. (Use 1 for a multisampled texture; or 0 to generate a full set of subtextures, down to 1 by 1). */
        unsigned mip_levels;

        /** The width of the texture, in pixels. */
        unsigned width;
        /** The height of the texture, in pixels. */
        unsigned height;
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
    const Desc& get_desc() const { return _desc; }

    /**
     * Returns the actual channel order. This may differ from the preferred channel
     * order, depending on hardware capabilities.
     */
    ChannelOrder get_channel_order() const { return _channel_order; }

    /**
     * Returns the size of each pixel, in bytes.
     */
    unsigned get_stride() const { return _stride; }

    /**
     * Returns the size of a line (the offset between the start of one line and the
     * start of the next line), in bytes.
     */
    unsigned get_line_pitch() const { return _line_pitch; }

    /**
     * Returns the size of a 2D image slice (the offset between the start of one 2D
     * image slice and the start of the next 2D image slice if applicable), in bytes.
     */
    unsigned get_slice_pitch() const { return _slice_pitch; }

    /**
     * Returns the total data size of this texture, in bytes.
     */
    unsigned get_data_size() const { return _data_size; }



protected:

    Texture(Type type, const CreateParams<Desc>& params, unsigned depth, unsigned image_count);

    ~Texture() override = default;



    /**
     * Returns the specified "image view" of the texture data.
     */
    images::Image _as_image(unsigned image_index) const;

    /**
     * Creates and returns mipmaps of the specified texture "image".
     */
    std::vector<images::Image> _create_mipmaps(unsigned image_index, images::ResamplingMethod resampling_method = images::ResamplingMethod::LinearInterpolation) const;

    /**
     * Updates the contents of this texture (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool _update(const ImageDataProvider& image_data_provider);



private:

    /**
     * Requests/acquires the texture resource.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const ImageDataProvider& image_data_provider) = 0;

    /**
     * Updates the contents of this texture (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _update_impl(const ImageDataProvider& image_data_provider, bool discard, bool no_overwrite) = 0;



    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool _check_data_impl(const resources::DataProvider& data_provider) override;

    /**
     * (Re)populates the local data buffer based on the given data provider.
     */
    bool _fill_data_impl(const resources::DataProvider& data_provider) override;

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool _acquire_impl(const resources::DataProvider& data_provider) override;



    /**
     * The type of the texture.
     */
    const Type _type;

    /**
     * The descriptor of the texture.
     */
    const Desc _desc;

    /**
     * The actual channel order. This may differ from the preferred channel order,
     * depending on hardware capabilities.
     */
    const ChannelOrder _channel_order;

    /**
     * The depth of the texture, in pixels (if 3D texture, otherwise trivially 1).
     */
    const unsigned _depth;

    /**
     * The size of each pixel, in bytes.
     */
    const unsigned _stride;

    /**
     * The size of a line (the offset between the start of one line and the start of
     * the next line), in bytes.
     */
    const unsigned _line_pitch;

    /**
     * The size of a 2D image slice (the offset between the start of one 2D image
     * slice and the start of the next 2D image slice if applicable), in bytes.
     */
    const unsigned _slice_pitch;

    /**
     * The total data size of this texture, in bytes.
     */
    const unsigned _data_size;

}; // class Texture



namespace detail {

template <class TTexture>
class TextureBase
    : public Texture
{
public:
    using Texture::Texture;

    class Id :
        public Texture::Id
    {
        using Texture::Id::Id;
    };

    Id get_id() const { return Resource::get_id<Id>(); }
}; // class TextureBase

} // namespace detail



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE_H
