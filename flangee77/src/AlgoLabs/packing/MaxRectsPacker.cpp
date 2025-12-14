#include "MaxRectsPacker.h"



namespace al7::packing {



    MaxRectsPacker::MaxRectsPacker(Size bin_size, Heuristic heuristic) noexcept
        : AbstractFreeRectPacker(bin_size)
        , _heuristic(heuristic)
    {
    }



    MaxRectsPacker::ScorePair MaxRectsPacker::_score_free_rect(Rect free_rect, Size padded_size) const
    {
        switch (_heuristic)
        {

        case Heuristic::BestShortSideFit:
        {
            const int leftover_width = std::abs(static_cast<int>(free_rect.size.width) - static_cast<int>(padded_size.width));
            const int leftover_height = std::abs(static_cast<int>(free_rect.size.height) - static_cast<int>(padded_size.height));
            const int short_side_fit = std::min(leftover_width, leftover_height);
            const int long_side_fit = std::max(leftover_width, leftover_height);
            return {short_side_fit, long_side_fit};
        }

        case Heuristic::BestAreaFit:
        {
            const int leftover_width = std::abs(static_cast<int>(free_rect.size.width) - static_cast<int>(padded_size.width));
            const int leftover_height = std::abs(static_cast<int>(free_rect.size.height) - static_cast<int>(padded_size.height));
            const int leftover_area = static_cast<int>(static_cast<ptrdiff_t>(free_rect.size.area()) - static_cast<ptrdiff_t>(padded_size.area()));
            const int area_fit = leftover_area;
            const int short_side_fit = std::min(leftover_width, leftover_height);
            return {area_fit, short_side_fit};
        }

        default:
            assert(false);
        } // switch heuristic

        return {};
    }

    void MaxRectsPacker::_split_free_rects(std::vector<Rect>& free_rects, size_t free_rect_index, const Rect& inserted_rect)
    {
        assert(free_rect_index < free_rects.size());
        assert(free_rects[free_rect_index].contains(inserted_rect));

        _split_free_rects_impl(free_rects, inserted_rect);
        _merge_free_rects_helper(free_rects);

        assert(_new_free_rects.empty());
    }



    void MaxRectsPacker::_split_free_rects_impl(std::vector<Rect>& free_rects, const Rect& inserted_rect)
    {
        assert(_new_free_rects.empty());
        _new_free_rects.clear();
        _new_free_rects.reserve(free_rects.size() * 3/2); // Just a wild "reasonable" guess.

        const unsigned inserted_x0 = inserted_rect.position.x;
        const unsigned inserted_x1 = inserted_rect.position.x + inserted_rect.size.width;
        const unsigned inserted_y0 = inserted_rect.position.y;
        const unsigned inserted_y1 = inserted_rect.position.y + inserted_rect.size.height;

        for (auto it = free_rects.begin(); it != free_rects.end(); )
        {
            const auto free_rect = *it;

            if (!free_rect.overlaps(inserted_rect))
            {
                // The rect was not touched, so we will leave it unchanged.
                ++it;
                continue;
            }

            // Remove the free rect and "replace" it
            // with up to four newly subdivided rects.
            it = free_rects.erase(it);

            const unsigned free_x0 = free_rect.position.x;
            const unsigned free_x1 = free_rect.position.x + free_rect.size.width;
            const unsigned free_y0 = free_rect.position.y;
            const unsigned free_y1 = free_rect.position.y + free_rect.size.height;

            // Split free rect into up to four new rects.
            if (inserted_x0 > free_x0)
            {
                // Left split
                _new_free_rects.emplace_back(free_x0, free_rect.position.y, inserted_x0 - free_x0, free_rect.size.height);
            }
            if (inserted_x1 < free_x1)
            {
                // Right split
                _new_free_rects.emplace_back(inserted_x1, free_rect.position.y, free_x1 - inserted_x1, free_rect.size.height);
            }
            if (inserted_y0 > free_y0)
            {
                // Bottom split
                _new_free_rects.emplace_back(free_rect.position.x, free_y0, free_rect.size.width, inserted_y0 - free_y0);
            }
            if (inserted_y1 < free_y1)
            {
                // Top split
                _new_free_rects.emplace_back(free_rect.position.x, inserted_y1, free_rect.size.width, free_y1 - inserted_y1);
            }
        } // for each free rect
    }

    void MaxRectsPacker::_merge_free_rects_helper(std::vector<Rect>& free_rects)
    {
        // Merge new free rects with old, unchanged free rects
        // and skip those new rects that are already covered.

        // First, we will check whether there are any redundant
        // constellations among the new free rects, and resolve them.
        for (size_t i = 0; i + 1 < _new_free_rects.size(); ++i)
        {
            for (size_t j = i + 1; j < _new_free_rects.size(); ++j)
            {
                if (_new_free_rects[i].contains(_new_free_rects[j]))
                {
                    _new_free_rects.erase(_new_free_rects.begin() + static_cast<ptrdiff_t>(j));
                    --j;
                    continue;
                }
                if (_new_free_rects[j].contains(_new_free_rects[i]))
                {
                    _new_free_rects.erase(_new_free_rects.begin() + static_cast<ptrdiff_t>(i));
                    --i;
                    break;
                }
            } // for j
        } // for i

        const size_t old_count = free_rects.size();

        // Then, we add the new free rects to our "list", but only those
        // that are not already covered by previous ones. We don't need to
        // check the reverse case, because the previous rects were already
        // "unique" before, and new rects can only shrink when split.
        for (const auto& new_free_rect : _new_free_rects)
        {
            bool covered = false;

            for (size_t k = 0; k < old_count; ++k)
            {
                if (free_rects[k].contains(new_free_rect))
                {
                    covered = true;
                    break;
                }
            } // for each old free rect

            if (!covered)
                free_rects.push_back(new_free_rect);
        } // for each new free rect

        _new_free_rects.clear();
    }



} // namespace al7::packing
