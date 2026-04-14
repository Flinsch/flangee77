#include "Texture.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"
#include "../PixelLayout.h"

#include "../images/ImageResizer.h"

#include <MathLabs/functions.h>

#include <CoreLabs/logging.h>



namespace xl7::graphics::textures {



    Texture::Texture(const CreateContext& ctx, Type type, const TextureDesc& desc)
        : ResourceWithData(ctx, static_cast<size_t>(desc.layer_count * desc.extent.volume() * PixelLayout::determine_bytes_per_pixel(desc.pixel_format)))
        , _type(type)
        , _desc(desc)
        , _channel_order(GraphicsSystem::instance().get_rendering_device()->recommend_channel_order(type, desc.pixel_format, desc.preferred_channel_order).first)
        , _bytes_per_pixel(PixelLayout::determine_bytes_per_pixel(desc.pixel_format))
        , _row_pitch(desc.extent.width * _bytes_per_pixel)
        , _slice_pitch(desc.extent.height * _row_pitch)
        , _layer_pitch(desc.extent.depth * _slice_pitch)
    {
        assert(get_data_size() > 0);

        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.textures.square_only && _desc.extent.width != _desc.extent.height)
            LOG_WARNING(u8"A non-square " + get_qualified_identifier() + u8" is supposed to be created, but only square textures are supported.");

        if (capabilities.textures.max_aspect_ratio && _desc.extent.height && _desc.extent.width / _desc.extent.height > capabilities.textures.max_aspect_ratio)
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has an width/height aspect ratio of " + cl7::to_string(_desc.extent.width / _desc.extent.height) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_aspect_ratio) + u8" is supported.");
        if (capabilities.textures.max_aspect_ratio && _desc.extent.width && _desc.extent.height / _desc.extent.width > capabilities.textures.max_aspect_ratio)
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has an height/width aspect ratio of " + cl7::to_string(_desc.extent.height / _desc.extent.width) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_aspect_ratio) + u8" is supported.");

        if (!ml7::is_power_of_two(_bytes_per_pixel))
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a pixel stride of " + cl7::to_string(_bytes_per_pixel) + u8" bytes (" + cl7::to_string(_bytes_per_pixel * 8) + u8" bits). Even if this were supported by the API, it should be avoided and an alternative, power-of-two format should be explicitly used (e.g., " + cl7::to_string(ml7::next_power_of_two(_bytes_per_pixel) * 8) + u8" bits).");
    }



    /**
     * Returns the specified "image view" of the texture data.
     */
    images::Image Texture::_as_image(unsigned layer) const
    {
        const auto size = static_cast<size_t>(_slice_pitch);
        const auto offset = static_cast<size_t>(layer) * size;

        assert(offset + size <= get_data_size());

        images::ImageDesc desc;
        desc.pixel_format = _desc.pixel_format;
        desc.channel_order = _channel_order;
        desc.width = _desc.extent.width;
        desc.height = _desc.extent.height;
        desc.depth = _desc.extent.depth;
        assert(size == desc.calculate_data_size());

        cl7::byte_view data;
        if (offset + size <= get_data().size())
            data = {get_data().data() + offset, size};

        return {desc, data, true};
    }

    /**
     * Creates and returns mipmaps of the specified texture "image".
     */
    std::vector<images::Image> Texture::_create_mipmaps(unsigned layer, images::ResamplingMethod resampling_method) const
    {
        std::vector<images::Image> mipmaps;

        images::Image image = _as_image(layer);
        while (image.get_width() > 1 || image.get_height() > 1 || image.get_depth() > 1)
        {
            images::Image mipmap = images::ImageResizer::create_mipmap(image, resampling_method);
            mipmaps.emplace_back(std::move(mipmap));
            image = {mipmaps.back().get_desc(), mipmaps.back().get_data(), true};
        }

        return mipmaps;
    }



} // namespace xl7::graphics::textures
