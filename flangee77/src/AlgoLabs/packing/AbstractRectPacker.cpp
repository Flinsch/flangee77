#include "AbstractRectPacker.h"



namespace al7::packing {



    AbstractRectPacker::AbstractRectPacker(Size bin_size) noexcept
        : _bin_size(bin_size)
    {
    }



    /** Checks if an item of the specified size could be inserted. */
    bool AbstractRectPacker::can_insert(Size item_size, RectOptions options) const
    {
        Size padded_size = _pad_item_size(item_size, options.padding);

        return _can_insert(padded_size, options);
    }

    /** Tries to insert an item of the specified size. */
    std::optional<Rect> AbstractRectPacker::insert(Size item_size, RectOptions options)
    {
        Size padded_size = _pad_item_size(item_size, options.padding);

        auto rect_result = _insert(padded_size, options);
        if (!rect_result.has_value())
            return {};

        ++_count;
        _used_area += rect_result->size.area();

        return _unpad_item_rect(*rect_result, options.padding);
    }

    /**
     * Tries to insert items of the specified sizes in an offline batch mode.
     * For each requested item size, an optional rectangle is returned, in the
     * corresponding order. However, it is (deliberately) not guaranteed that
     * "earlier" (or even all) rectangles will be considered before later rectangles.
     * In case of doubt, the decision rests with the specific algorithm used.
     */
    std::vector<std::optional<Rect>> AbstractRectPacker::insert(const std::vector<Size>& item_sizes, RectOptions options)
    {
        std::vector<std::pair<Size, size_t>> padded_sizes_and_indices(item_sizes.size());
        for (size_t i = 0; i < item_sizes.size(); ++i)
            padded_sizes_and_indices[i] = std::make_pair(_pad_item_size(item_sizes[i], options.padding), i);

        std::vector<std::optional<Rect>> rect_results(item_sizes.size());

        _insert(rect_results, std::move(padded_sizes_and_indices), options);

        for (auto& rect_result : rect_results)
        {
            if (!rect_result.has_value())
                continue;

            ++_count;
            _used_area += rect_result->size.area();

            rect_result = _unpad_item_rect(*rect_result, options.padding);
        }

        return rect_results;
    }



    /** Override this function to implement offline batch logic that is "smarter" than the "naive" default logic that simply tries to add the items one after the other. */
    void AbstractRectPacker::_insert(std::span<std::optional<Rect>> rect_results, std::vector<std::pair<Size, size_t>>&& padded_sizes_and_indices, RectOptions options)
    {
        assert(rect_results.size() == padded_sizes_and_indices.size());

        for (const auto [padded_size, item_index] : std::move(padded_sizes_and_indices))
            rect_results[item_index] = _insert(padded_size, options);
    }



    /** Resets the packer by essentially clearing the bin. */
    void AbstractRectPacker::reset()
    {
        reset(_bin_size);
    }

    /** Resets the packer by essentially clearing the bin and adjusts the bin size. */
    void AbstractRectPacker::reset(Size bin_size)
    {
        _bin_size = bin_size;

        _count = 0;
        _used_area = 0;

        _on_reset(bin_size);
    }



    /** Calculates the occupancy as the used area divided by the total area. */
    float AbstractRectPacker::occupancy() const
    {
        const auto total_area = _bin_size.area();
        if (total_area == 0)
            return 0.0f;

        return static_cast<float>(used_area())
            / static_cast<float>(total_area);
    }



    Size AbstractRectPacker::_pad_item_size(Size item_size, unsigned padding)
    {
        Size padded_size = item_size;
        padded_size.width += padding * 2;
        padded_size.height += padding * 2;
        return padded_size;
    }

    Rect AbstractRectPacker::_unpad_item_rect(Rect padded_rect, unsigned padding)
    {
        assert(padded_rect.size.width >= padding * 2);
        assert(padded_rect.size.height >= padding * 2);
        Rect item_rect = padded_rect;
        item_rect.position.x += padding;
        item_rect.position.y += padding;
        item_rect.size.width -= padding * 2;
        item_rect.size.height -= padding * 2;
        return item_rect;
    }



} // namespace al7::packing
