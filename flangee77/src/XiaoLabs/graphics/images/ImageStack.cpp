#include "ImageStack.h"



namespace xl7::graphics::images {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    ImageStack::ImageStack()
        : _desc({.pixel_format=PixelFormat::UNKNOWN, .channel_order=ChannelOrder::RGBA, .width=0, .height=0})
    {
    }

    ImageStack::ImageStack(const Image::Desc& desc)
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

        size_t image_size = _desc.calculate_data_size();
        assert(image_size > 0);
        assert(_data.size() % image_size == 0);

        return static_cast<unsigned>(_data.size() / image_size);
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * (Re)initializes the image stack.
     */
    bool ImageStack::init(const Image::Desc& desc)
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
        const Image::Desc& desc = image.get_desc();
        if (desc.width != _desc.width || desc.height != _desc.height || desc.depth != _desc.depth)
        {
            // Should we log an error message or something?
            return false;
        }

        assert(desc.pixel_format == _desc.pixel_format);
        assert(desc.channel_order == _desc.channel_order);

        _data.insert(_data.end(), image.get_data().begin(), image.get_data().end());

        return true;
    }



} // namespace xl7::graphics::images
