#include "ImageAtlas.h"

#include <algorithm>
#include <limits>



namespace xl7::graphics::images {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    ImageAtlas::ImageAtlas()
        : _pixel_format(PixelFormat::UNKNOWN)
        , _channel_order(ChannelOrder::RGBA)
    {
    }

    ImageAtlas::ImageAtlas(PixelFormat pixel_format, ChannelOrder channel_order, al7::packing::AbstractRectPacker* default_rect_packer)
        : ImageAtlas()
    {
        init(pixel_format, channel_order, default_rect_packer);
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns true if all images were successfully positioned using a rect packer.
     */
    bool ImageAtlas::is_valid() const
    {
        return std::ranges::all_of(_entries, [](const auto& entry) {
            return entry.valid;
        });
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * (Re)initializes the image atlas.
     * The specified default rect packer is used to directly position newly added
     * images incrementally. If not specified, no auto-positioning will take place.
     */
    void ImageAtlas::init(PixelFormat pixel_format, ChannelOrder channel_order, al7::packing::AbstractRectPacker* default_rect_packer)
    {
        _entries.clear();

        _pixel_format = pixel_format;
        _channel_order = channel_order;
        _default_rect_packer = default_rect_packer;
    }

    /**
     * Adds the specified image to this image atlas by copying its data.
     * Returns false if the image does not confirm to the common pixel format and
     * channel order, or if the default rect packer, if specified, could not insert
     * a corresponding item.
     */
    bool ImageAtlas::add_image(Image image)
    {
        const auto& desc = image.get_desc();

        // We deliberately do not check the width and height.
        // Otherwise, only 2D images are generally allowed.
        if (desc.depth > 1)
        {
            // Should we log an error message or something?
            return false;
        }

        if (desc.pixel_format != _pixel_format || desc.channel_order != _channel_order)
        {
            // Should we log an error message or something?
            return false;
        }

        cl7::byte_vector image_data;
        image.swap_data(image_data);

        return add_image_data(desc.width, desc.height, std::move(image_data));
    }

    /**
     * Adds the specified image data to this image atlas by copying the data.
     * Returns false if the image data does not confirm to the specified image size
     * or if the default rect packer, if specified, could not insert a corresponding
     * item.
     */
    bool ImageAtlas::add_image_data(unsigned width, unsigned height, cl7::byte_vector image_data)
    {
        _entries.emplace_back(
            al7::packing::Rect{0, 0, width, height},
            false,
            false,
            std::move(image_data)
        );

        if (!_default_rect_packer)
            return true;

        al7::packing::Size item_size{width, height};
        const auto rect_result = _default_rect_packer->insert(item_size);

        auto& entry = _entries.back();

        return _try_update_entry(entry, rect_result);
    }

    /**
     * Returns the data of the specified image.
     */
    cl7::byte_view ImageAtlas::get_image_data(unsigned image_index) const
    {
        const unsigned image_count = get_image_count();
        assert(image_index < image_count);
        if (image_index >= image_count)
        {
            // Should we log an error message or something?
            return {};
        }

        return _entries[image_index].image_data;
    }

    /**
     * Returns the descriptor of the specified image.
     */
    ImageDesc ImageAtlas::get_image_desc(unsigned image_index) const
    {
        ImageDesc desc{
            .pixel_format = _pixel_format,
            .channel_order = _channel_order,
            .width = 0,
            .height = 0,
            .depth = 1,
        };

        const unsigned image_count = get_image_count();
        assert(image_index < image_count);
        if (image_index >= image_count)
        {
            // Should we log an error message or something?
            return desc;
        }

        const auto& entry = _entries[image_index];

        desc.width = entry.rect.size.width;
        desc.height = entry.rect.size.height;
        if (entry.flipped)
            std::swap(desc.width, desc.height);

        return desc;
    }

    /**
     * Returns the specified image as a "view" of the image data.
     */
    Image ImageAtlas::get_image(unsigned image_index) const
    {
        return {get_image_desc(image_index), get_image_data(image_index), true};
    }

    /**
     * Extracts the specified image by creating a new one and copying the data.
     */
    Image ImageAtlas::extract_image(unsigned image_index) const
    {
        return {get_image_desc(image_index), get_image_data(image_index)};
    }

    /**
     * Rearranges the contained images. If no rect packer is specified, the default
     * packer is used. Returns false if the effectively used rect packer could not
     * successfully process all images.
     */
    bool ImageAtlas::rearrange_images(al7::packing::AbstractRectPacker* rect_packer)
    {
        if (!rect_packer)
            rect_packer = _default_rect_packer;
        if (!rect_packer)
            return false;

        std::vector<al7::packing::Size> item_sizes;
        item_sizes.reserve(_entries.size());
        for (auto& entry : _entries)
        {
            // Reset/"normalize" entry.
            entry.rect.position = {0, 0};
            if (entry.flipped)
                entry.rect.size = entry.rect.size.flipped();
            entry.flipped = false;
            entry.valid = false;

            item_sizes.push_back(entry.rect.size);
        }

        const auto rect_results = rect_packer->insert(item_sizes);
        assert(rect_results.size() == item_sizes.size());
        assert(rect_results.size() == _entries.size());

        for (size_t i = 0; i < item_sizes.size(); ++i)
        {
            const auto rect_result = rect_results[i];

            auto& entry = _entries[i];

            _try_update_entry(entry, rect_result);
        }

        return is_valid();
    }

    /**
     * Calculates the required atlas size for the successfully positioned images,
     * including estimates for any possible padding. If in doubt, the specified size
     * of the rect packer should be used.
     */
    al7::packing::Size ImageAtlas::estimate_size() const
    {
        if (_entries.empty())
            return {0, 0};

        unsigned min_x = std::numeric_limits<unsigned>::max();
        unsigned min_y = std::numeric_limits<unsigned>::max();
        unsigned max_x = 0;
        unsigned max_y = 0;

        for (const auto& entry : _entries)
        {
            const auto& position = entry.rect.position;
            const auto& size = entry.rect.size;
            min_x = std::min(min_x, position.x);
            min_y = std::min(min_y, position.y);
            max_x = std::max(max_x, position.x + size.width);
            max_y = std::max(max_y, position.y + size.height);
        }

        assert(min_x == min_y);
        unsigned padding = std::max(min_x, min_y);

        return {
            max_x - min_x + 2 * padding,
            max_y - min_y + 2 * padding,
        };
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Attempts to update the positional information of an entry based on the
     * specified rect result. If the rect result is "empty", false is returned.
     * In any case, the "valid" flag of the entry will be updated accordingly.
     */
    bool ImageAtlas::_try_update_entry(Entry& entry, const std::optional<al7::packing::Rect>& rect_result)
    {
        entry.valid = rect_result.has_value();
        if (!entry.valid)
            return false;

        const auto item_size = entry.rect.size;

        assert(rect_result->size == item_size || rect_result->size == item_size.flipped());
        entry.rect = *rect_result;
        entry.flipped = rect_result->size != item_size;

        return true;
    }



} // namespace xl7::graphics::images
