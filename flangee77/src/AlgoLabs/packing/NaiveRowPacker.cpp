#include "NaiveRowPacker.h"



namespace al7::packing {



    NaiveRowPacker::NaiveRowPacker(Size bin_size) noexcept
        : AbstractRectPacker(bin_size)
    {
    }



    void NaiveRowPacker::_on_reset(Size bin_size)
    {
        _position = {0, 0};
        _row_height = 0;
    }

    bool NaiveRowPacker::_can_insert(Size padded_size, RectOptions options) const
    {
        return _find_insert_rect(padded_size, options.allow_flip).has_value();
    }

    std::optional<Rect> NaiveRowPacker::_insert(Size padded_size, RectOptions options)
    {
        auto rect_result = _find_insert_rect(padded_size, options.allow_flip);
        if (!rect_result.has_value())
            return {};

        assert(rect_result->size == padded_size || (options.allow_flip && rect_result->size == padded_size.flipped()));

        // "Insert" item by adjusting position and row height.
        _position = rect_result->position;
        if (_position.x > 0)
            _row_height = std::max(_row_height, rect_result->size.height);
        else
            _row_height = rect_result->size.height;
        _position.x += rect_result->size.width;

        return rect_result;
    }



    std::optional<Rect> NaiveRowPacker::_find_insert_rect(Size padded_size, bool allow_flip) const
    {
        // Does the item fit in the current row?
        if (_position.x + padded_size.width <= bin_size().width && _position.y + padded_size.height <= bin_size().height)
            return Rect{_position, padded_size};

        Size flipped_size = padded_size.flipped();

        // Does the item fit in the current row when flipped (if allowed)?
        if (allow_flip && _position.x + flipped_size.width <= bin_size().width && _position.y + flipped_size.height <= bin_size().height)
            return Rect{_position, flipped_size};

        unsigned next_row_y = _position.y + _row_height;

        // Does the item fit in the next row?
        if (padded_size.width <= bin_size().width && next_row_y + padded_size.height <= bin_size().height)
            return Rect{0, next_row_y, padded_size};

        // Does the item fit in the next row when flipped (if allowed)?
        if (allow_flip && flipped_size.width <= bin_size().width && next_row_y + flipped_size.height <= bin_size().height)
            return Rect{0, next_row_y, flipped_size};

        // No.
        return {};
    }



} // namespace al7::packing
