#include "Texture.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"
#include "../PixelLayout.h"

#include "../images/ImageConverter.h"
#include "../images/ImageResizer.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::textures {



    Texture::Texture(Type type, const CreateParams<Desc>& params, unsigned depth, unsigned image_count)
        : ResourceBase(params)
        , _type(type)
        , _desc(params.desc)
        , _channel_order(GraphicsSystem::instance().get_rendering_device()->recommend_channel_order(type, params.desc.pixel_format, params.desc.preferred_channel_order).first)
        , _depth(depth)
        , _stride(PixelLayout::determine_stride(params.desc.pixel_format))
        , _line_pitch(_stride * params.desc.width)
        , _slice_pitch(_line_pitch * params.desc.height)
        , _data_size(_slice_pitch * depth * image_count)
    {
        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.textures.square_only && _desc.width != _desc.height)
            LOG_WARNING(u8"A non-square " + get_typed_identifier_string() + u8" is supposed to be created, but only square textures are supported.");

        if (capabilities.textures.max_aspect_ratio && _desc.height && _desc.width / _desc.height > capabilities.textures.max_aspect_ratio)
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has an width/height aspect ratio of " + cl7::to_string(_desc.width / _desc.height) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_aspect_ratio) + u8" is supported.");
        if (capabilities.textures.max_aspect_ratio && _desc.width && _desc.height / _desc.width > capabilities.textures.max_aspect_ratio)
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" to be created has an height/width aspect ratio of " + cl7::to_string(_desc.height / _desc.width) + u8", but a maximum of " + cl7::to_string(capabilities.textures.max_aspect_ratio) + u8" is supported.");
    }



    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool Texture::_check_data_impl(const resources::DataProvider& data_provider)
    {
        assert(typeid(data_provider) == typeid(const ImageDataProvider&));
        auto image_data_provider = static_cast<const ImageDataProvider&>(data_provider); // NOLINT(*-pro-type-static-cast-downcast)

        if (image_data_provider.get_image_desc().width != _desc.width)
        {
            LOG_ERROR(u8"The image width provided for " + get_typed_identifier_string() + u8" does not match the width of the " + cl7::u8string(get_type_string()) + u8".");
            return false;
        }
        if (image_data_provider.get_image_desc().height != _desc.height)
        {
            LOG_ERROR(u8"The image height provided for " + get_typed_identifier_string() + u8" does not match the height of the " + cl7::u8string(get_type_string()) + u8".");
            return false;
        }
        if (image_data_provider.get_image_desc().depth != _depth)
        {
            LOG_ERROR(u8"The image depth provided for " + get_typed_identifier_string() + u8" does not match the depth of the " + cl7::u8string(get_type_string()) + u8".");
            return false;
        }

        if (!_check_against_size(data_provider, image_data_provider.get_image_desc().calculate_data_size() * image_data_provider.get_image_count()))
            return false;
        if (!_check_against_stride(data_provider, image_data_provider.get_image_desc().determine_pixel_stride()))
            return false;

        return true;
    }

    /**
     * (Re)populates the local data buffer based on the given data provider.
     */
    bool Texture::_fill_data_impl(const resources::DataProvider& data_provider)
    {
        assert(typeid(data_provider) == typeid(const ImageDataProvider&));
        auto image_data_provider = static_cast<const ImageDataProvider&>(data_provider); // NOLINT(*-pro-type-static-cast-downcast)

        cl7::byte_vector image_data;
        data_provider.fill(image_data);

        if (image_data_provider.get_image_desc().pixel_format == _desc.pixel_format && image_data_provider.get_image_desc().channel_order == _channel_order)
        {
            // No conversion required at all.
            image_data.swap(_access_data());
        }
        else
        {
            // Perform image format conversion.
            images::Image source_image(image_data_provider.get_image_desc(), std::move(image_data));
            images::Image target_image = images::ImageConverter::convert_image(source_image, _desc.pixel_format, _channel_order);
            target_image.swap_data(_access_data());
        }

        return true;
    }

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool Texture::_acquire_impl(const resources::DataProvider& data_provider)
    {
        assert(typeid(data_provider) == typeid(const ImageDataProvider&));
        auto image_data_provider = static_cast<const ImageDataProvider&>(data_provider); // NOLINT(*-pro-type-static-cast-downcast)

        return _acquire_impl(image_data_provider);
    }



    /**
     * Returns the specified "image view" of the texture data.
     */
    images::Image Texture::_as_image(unsigned image_index) const
    {
        const auto size = static_cast<size_t>(_slice_pitch);
        const auto offset = static_cast<size_t>(image_index) * size;

        assert(offset + size <= _data_size);

        images::Image::Desc desc;
        desc.pixel_format = _desc.pixel_format;
        desc.channel_order = _channel_order;
        desc.width = _desc.width;
        desc.height = _desc.height;
        desc.depth = _depth;
        assert(size == desc.calculate_data_size());

        cl7::byte_view data;
        if (offset + size <= get_data().size())
            data = {get_data().data() + offset, size};

        return {desc, data, true};
    }

    /**
     * Creates and returns mipmaps of the specified texture "image".
     */
    std::vector<images::Image> Texture::_create_mipmaps(unsigned image_index, images::ResamplingMethod resampling_method) const
    {
        std::vector<images::Image> mipmaps;

        images::Image image = _as_image(image_index);
        while (image.get_width() > 1 || image.get_height() > 1 || image.get_depth() > 1)
        {
            images::Image mipmap = images::ImageResizer::create_mipmap(image, resampling_method);
            mipmaps.emplace_back(std::move(mipmap));
            image = {mipmaps.back().get_desc(), mipmaps.back().get_data(), true};
        }

        return mipmaps;
    }

    /**
     * Updates the contents of this texture (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool Texture::_update(const ImageDataProvider& image_data_provider)
    {
        if (_desc.usage == TextureUsage::Immutable)
        {
            LOG_ERROR(u8"The immutable " + get_typed_identifier_string() + u8" cannot be updated.");
            return false;
        }

        if (!_try_fill_data(image_data_provider))
            return false;

        assert(image_data_provider.get_offset() == 0 && get_data().size() == _data_size);
        bool discard = true;
        bool no_overwrite = false;

        return _update_impl(image_data_provider, discard, no_overwrite);
    }



} // namespace xl7::graphics::textures
