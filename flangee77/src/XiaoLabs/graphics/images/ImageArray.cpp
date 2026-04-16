#include "ImageArray.h"



namespace xl7::graphics::images {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    ImageArray::ImageArray()
        : _desc({.pixel_format = PixelFormat::UNKNOWN, .channel_order = ChannelOrder::RGBA, .width = 0, .height = 0})
    {
    }

    ImageArray::ImageArray(const ImageDesc& desc)
        : ImageArray()
    {
        init(desc);
    }

    ImageArray::ImageArray(const Image& image)
        : ImageArray()
    {
        init(image);
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the number of contained images.
     */
    unsigned ImageArray::get_image_count() const
    {
        if (_data.empty())
            return 0;

        const size_t image_size = _desc.calculate_data_size();
        assert(image_size > 0);
        assert(_data.size() % image_size == 0);

        return static_cast<unsigned>(_data.size() / image_size);
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * (Re)initializes the image array.
     */
    void ImageArray::init(const ImageDesc& desc)
    {
        _desc = desc;
        _data.clear();
    }

    /**
     * (Re)initializes the image array.
     */
    void ImageArray::init(const Image& image)
    {
        init(image.get_desc());
        add_image_data(image.get_data());
    }

    /**
     * Adds the specified image to this image array by copying its data.
     */
    bool ImageArray::add_image(const Image& image)
    {
        const auto& desc = image.get_desc();

        if (desc.width != _desc.width || desc.height != _desc.height || desc.depth != _desc.depth)
        {
            // Should we log an error message or something?
            return false;
        }

        if (desc.pixel_format != _desc.pixel_format || desc.channel_order != _desc.channel_order)
        {
            // Should we log an error message or something?
            return false;
        }

        return add_image_data(image.get_data());
    }

    /**
     * Adds the specified image data to this image array by copying the data.
     */
    bool ImageArray::add_image_data(cl7::byte_view image_data)
    {
        const size_t image_size = _desc.calculate_data_size();
        assert(image_data.size() == image_size);
        if (image_data.size() != image_size)
        {
            // Should we log an error message or something?
            return false;
        }

        _data.insert(_data.end(), image_data.begin(), image_data.end());

        return true;
    }

    /**
     * Returns the data of the specified image.
     */
    cl7::byte_view ImageArray::get_image_data(unsigned image_index) const
    {
        const unsigned image_count = get_image_count();
        assert(image_index < image_count);
        if (image_index >= image_count)
        {
            // Should we log an error message or something?
            return {};
        }

        const size_t image_size = _desc.calculate_data_size();
        assert(image_size > 0);
        assert(_data.size() % image_size == 0);

        const size_t offset = image_index * image_size;
        assert(offset + image_size <= _data.size());

        return cl7::byte_view{_data}.subspan(offset, image_size);
    }

    /**
     * Returns the specified image as a "view" of the image data.
     */
    Image ImageArray::get_image(unsigned image_index) const
    {
        return {_desc, get_image_data(image_index), true};
    }

    /**
     * Extracts the specified image by creating a new one and copying the data.
     */
    Image ImageArray::extract_image(unsigned image_index) const
    {
        return {_desc, get_image_data(image_index)};
    }



} // namespace xl7::graphics::images
