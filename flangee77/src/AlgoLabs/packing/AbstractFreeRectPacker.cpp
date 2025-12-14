#include "AbstractFreeRectPacker.h"

#include <algorithm>



namespace al7::packing {



    AbstractFreeRectPacker::AbstractFreeRectPacker(Size bin_size) noexcept
        : AbstractRectPacker(bin_size)
    {
        _free_rects.emplace_back(0, 0, bin_size);
    }



    void AbstractFreeRectPacker::_on_reset(Size bin_size)
    {
        _free_rects.clear();
        _free_rects.emplace_back(0, 0, bin_size);
    }

    bool AbstractFreeRectPacker::_can_insert(Size padded_size, RectOptions options) const
    {
        return std::ranges::any_of(_free_rects, [padded_size, options](const auto& free_rect) {
            if (free_rect.size.covers(padded_size))
                return true;
            return options.allow_flip && free_rect.size.covers(padded_size.flipped());
        });
    }

    std::optional<Rect> AbstractFreeRectPacker::_insert(Size padded_size, RectOptions options)
    {
        return _insert_impl(padded_size, options.allow_flip);
    }

    void AbstractFreeRectPacker::_insert(std::span<std::optional<Rect>> rect_results, std::vector<std::pair<Size, size_t>>&& padded_sizes_and_indices, RectOptions options)
    {
        _insert_impl(rect_results, std::move(padded_sizes_and_indices), options.allow_flip);
    }



    std::optional<Rect> AbstractFreeRectPacker::_insert_impl(Size padded_size, bool allow_flip)
    {
        auto best_scored_rect = _find_best_rect(padded_size, allow_flip);
        if (!best_scored_rect.rect_result.has_value())
            return {};

        _place_rect(best_scored_rect.free_rect_index, *best_scored_rect.rect_result);

        return best_scored_rect.rect_result;
    }

    void AbstractFreeRectPacker::_insert_impl(std::span<std::optional<Rect>> rect_results, std::vector<std::pair<Size, size_t>> padded_sizes_and_indices, bool allow_flip)
    {
        assert(rect_results.size() == padded_sizes_and_indices.size());

        while (!padded_sizes_and_indices.empty())
        {
            ScoredRect best_scored_rect;
            size_t best_item_index = 0;
            size_t best_tmp_index = 0;

            for (size_t i = 0; i < padded_sizes_and_indices.size(); ++i)
            {
                const Size padded_size = padded_sizes_and_indices[i].first;
                const size_t item_index = padded_sizes_and_indices[i].second;

                const auto scored_rect = _find_best_rect(padded_size, allow_flip);
                if (scored_rect.score_pair.is_better_than(best_scored_rect.score_pair))
                {
                    best_scored_rect = scored_rect;
                    best_item_index = item_index;
                    best_tmp_index = i;
                }
            }

            if (!best_scored_rect.rect_result.has_value())
                continue;
            assert(best_item_index < rect_results.size());
            assert(best_tmp_index < padded_sizes_and_indices.size());

            _place_rect(best_scored_rect.free_rect_index, *best_scored_rect.rect_result);

            assert(!rect_results[best_item_index].has_value());
            // Deploy item result.
            rect_results[best_item_index] = best_scored_rect.rect_result;
            // Erase item query by replacing it with the last entry.
            padded_sizes_and_indices[best_tmp_index] = padded_sizes_and_indices.back();
            padded_sizes_and_indices.pop_back();
        } // while
    }



    void AbstractFreeRectPacker::_place_rect(size_t free_rect_index, const Rect& rect)
    {
        _split_free_rects(_free_rects, free_rect_index, rect);

#ifdef _DEBUG
        for (const auto& r : _free_rects)
        {
            assert(r.size.area() > 0);
            assert(!r.overlaps(rect));
        }
        for (const auto& r1 : _free_rects)
            for (const auto& r2 : _free_rects)
                assert(r1.contains(r2) == (&r1 == &r2));
#endif // _DEBUG
    }

    AbstractFreeRectPacker::ScoredRect AbstractFreeRectPacker::_find_best_rect(Size padded_size, bool allow_flip) const
    {
        ScoredRect best_scored_rect;

        Size flipped_size = padded_size.flipped();

        for (size_t i = 0; i < _free_rects.size(); ++i)
        {
            const Rect& free_rect = _free_rects[i];

            if (free_rect.size.covers(padded_size))
            {
                const auto score_pair = _score_free_rect(free_rect, padded_size);
                if (score_pair.is_better_than(best_scored_rect.score_pair))
                {
                    best_scored_rect.score_pair = score_pair;
                    best_scored_rect.rect_result = {free_rect.position, padded_size};
                    best_scored_rect.free_rect_index = i;
                }
            }

            if (allow_flip && free_rect.size.covers(flipped_size))
            {
                const auto score_pair = _score_free_rect(free_rect, flipped_size);
                if (score_pair.is_better_than(best_scored_rect.score_pair))
                {
                    best_scored_rect.score_pair = score_pair;
                    best_scored_rect.rect_result = {free_rect.position, flipped_size};
                    best_scored_rect.free_rect_index = i;
                }
            }
        } // for each free rect

        return best_scored_rect;
    }



} // namespace al7::packing
