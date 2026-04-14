#include "ImageStack.h"



namespace xl7::graphics::images {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    ImageStack::ImageStack()
        : _desc({.pixel_format = PixelFormat::UNKNOWN, .channel_order = ChannelOrder::RGBA, .width = 0, .height = 0})
    {
    }

    ImageStack::ImageStack(const ImageDesc& desc)
        : ImageStack()
    {
        init(desc);
    }

    ImageStack::ImageStack(const Image& image)
        : ImageStack()
    {
        init(image);
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the number of contained images.
     */
    unsigned ImageStack::get_image_count() const
    {
        if (_data.empty())
            return 0;

        const size_t image_size = _desc.calculate_data_size();
        assert(image_size > 0);
        assert(_data.size() % image_size == 0);

        return static_cast<unsigned>(_data.size() / image_size);
    }

    /**
     * Returns the data of the specified image.
     */
    cl7::byte_view ImageStack::get_image_data(unsigned image_index) const
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



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * (Re)initializes the image stack.
     */
    bool ImageStack::init(const ImageDesc& desc)
    {
        _desc = desc;
        _data.clear();
        return true;
    }

    /**
     * (Re)initializes the image stack.
     */
    bool ImageStack::init(const Image& image)
    {
        return init(image.get_desc()) && add_image(image);
    }

    /**
     * Adds the specified image to this image stack by copying its data.
     */
    bool ImageStack::add_image(const Image& image)
    {
        const ImageDesc& desc = image.get_desc();
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
     * Adds the specified image data to this image stack by copying the data.
     */
    bool ImageStack::add_image_data(cl7::byte_view image_data)
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



} // namespace xl7::graphics::images
