#include "ShelfPacker.h"

#include <algorithm>
#include <limits>



namespace al7::packing {



    ShelfPacker::ShelfPacker(Size bin_size, Heuristic heuristic) noexcept
        : AbstractRectPacker(bin_size)
        , _heuristic(heuristic)
    {
        _shelves.emplace_back(0, 0, bin_size.height, 0);
    }



    void ShelfPacker::_on_reset(Size bin_size)
    {
        _shelves.clear();
        _shelves.emplace_back(0, 0, bin_size.height, 0);
    }

    bool ShelfPacker::_can_insert(Size padded_size, RectOptions options) const
    {
        return std::ranges::any_of(_shelves, [this, &padded_size, options](const auto& shelf) {
            return _try_fit_on_shelf(shelf, padded_size, options.allow_flip);
        });
    }

    std::optional<Rect> ShelfPacker::_insert(Size padded_size, RectOptions options)
    {
        return _insert_impl(padded_size, options.allow_flip);
    }



    std::optional<Rect> ShelfPacker::_insert_impl(Size padded_size, bool allow_flip)
    {
        switch (_heuristic)
        {

        case Heuristic::FirstFit:
        {
            for (auto& shelf : _shelves)
            {
                if (_try_fit_on_shelf(shelf, padded_size, allow_flip))
                    return _add_to_shelf(shelf, padded_size);
            }
            break;
        }

        case Heuristic::BestHeightFit:
        {
            unsigned best_height_diff = std::numeric_limits<unsigned>::max();
            Shelf* best_shelf = nullptr;
            Size best_size = {};

            for (auto& shelf : _shelves)
            {
                if (_try_fit_on_shelf(shelf, padded_size, allow_flip))
                {
                    assert(shelf.height >= padded_size.height);
                    const unsigned height_diff = shelf.height - padded_size.height;
                    if (height_diff < best_height_diff)
                    {
                        best_height_diff = height_diff;
                        best_shelf = &shelf;
                        best_size = padded_size;
                    }
                }
            }

            if (best_shelf)
                return _add_to_shelf(*best_shelf, best_size);
            break;
        }

        default:
            assert(false);
        } // switch heuristic

        assert(!_shelves.empty());
        auto& last_shelf = _shelves.back();
        assert(last_shelf.start_y + last_shelf.item_height <= bin_size().height);

        // Prepare new shelf.
        Shelf shelf;
        shelf.current_x = 0;
        shelf.start_y = last_shelf.start_y + last_shelf.item_height;
        shelf.height = bin_size().height - shelf.start_y;
        shelf.item_height = 0;

        if (_try_fit_on_shelf(shelf, padded_size, allow_flip))
        {
            // "Close" last open shelf.
            assert(last_shelf.item_height <= last_shelf.height);
            last_shelf.height = last_shelf.item_height;
            // Add new shelf.
            _shelves.emplace_back(shelf);
            // Add item to new shelf.
            return _add_to_shelf(_shelves.back(), padded_size);
        }

        return {};
    }



    bool ShelfPacker::_try_fit_on_shelf(const Shelf& shelf, Size& padded_size, bool allow_flip) const
    {
        if (allow_flip)
        {
            // Prefer a flat, sideways orientation to an upright orientation if it's the
            // first item on the shelf, in order to keep the shelf height as low as possible.
            // Otherwise, prefer upright items to save horizontal shelf space.
            if ((padded_size.width > padded_size.height && shelf.current_x + padded_size.width > bin_size().width) || // Forced reorientation (but without any guarantee that it fits that way).
                (padded_size.width > padded_size.height && shelf.current_x > 0 && padded_size.width <= shelf.height) || // Prefer upright orientation if not first item.
                (padded_size.width < padded_size.height && shelf.current_x == 0 && padded_size.height <= bin_size().width)) // Prefer sideways orientation if first item.
            {
                std::swap(padded_size.width, padded_size.height);
            }
        }

        return shelf.current_x + padded_size.width <= bin_size().width && padded_size.height <= shelf.height;
    }

    Rect ShelfPacker::_add_to_shelf(Shelf& shelf, Size padded_size)
    {
        assert(shelf.current_x + padded_size.width <= bin_size().width);
        assert(shelf.start_y + padded_size.height <= bin_size().height);
        assert(padded_size.height <= shelf.height);

        const unsigned current_x = shelf.current_x;
        shelf.current_x += padded_size.width;
        shelf.item_height = std::max(shelf.item_height, padded_size.height);
        return {current_x, shelf.start_y, padded_size};
    }



} // namespace al7::packing
