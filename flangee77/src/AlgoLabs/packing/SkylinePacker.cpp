#include "SkylinePacker.h"

#include <CoreLabs/static_vector.h>



namespace al7::packing {



    SkylinePacker::SkylinePacker(Size bin_size, Heuristic heuristic, bool use_waste_map) noexcept
        : AbstractRectPacker(bin_size)
        , _heuristic(heuristic)
        , _use_waste_map(use_waste_map)
        , _waste_map(bin_size)
    {
        _waste_map.access_free_rects().clear();

        _skyline_levels.emplace_back(0, bin_size.width, 0);
    }



    void SkylinePacker::_on_reset(Size bin_size)
    {
        _waste_map.reset(bin_size);
        _waste_map.access_free_rects().clear();

        _skyline_levels.clear();
        _skyline_levels.emplace_back(0, bin_size.width, 0);
    }

    bool SkylinePacker::_can_insert(Size padded_size, RectOptions options) const
    {
        if (_use_waste_map && _waste_map.can_insert(padded_size, options))
            return true;

        Size flipped_size = padded_size.flipped();

        for (size_t i = 0; i < _skyline_levels.size(); ++i)
        {
            if (_fits_on_level(i, padded_size))
                return true;
            if (options.allow_flip && _fits_on_level(i, flipped_size))
                return true;
        }

        return false;
    }

    std::optional<Rect> SkylinePacker::_insert(Size padded_size, RectOptions options)
    {
        if (_use_waste_map)
        {
            auto rect_result = _waste_map.insert(padded_size, {.allow_flip = options.allow_flip});
            if (rect_result.has_value())
                return rect_result;
        }

        auto best_scored_rect = _find_best_rect(padded_size, options.allow_flip);
        if (!best_scored_rect.rect_result.has_value())
            return {};

        _place_rect(best_scored_rect.start_index, *best_scored_rect.rect_result);

        return best_scored_rect.rect_result;
    }



    void SkylinePacker::_place_rect(size_t start_index, const Rect& rect)
    {
        assert(start_index < _skyline_levels.size());
        assert(_skyline_levels[start_index].start_x <= rect.position.x);
        assert(_skyline_levels[start_index].start_x + rect.size.width <= bin_size().width);
        assert(rect.position.x + rect.size.width <= bin_size().width);
        assert(rect.position.y + rect.size.height <= bin_size().height);

        const unsigned x0 = rect.position.x;
        const unsigned x1 = rect.position.x + rect.size.width;
        const unsigned y = rect.position.y;

        for (size_t level_index = start_index; ; ++level_index)
        {
            assert(level_index < _skyline_levels.size());
            // Check for skipped skyline levels due to a right-aligned position.
            if (x0 >= _skyline_levels[level_index].start_x + _skyline_levels[level_index].width)
                continue;

            const unsigned x0_ = std::max(x0, _skyline_levels[level_index].start_x);
            const unsigned x1_ = std::min(x1, _skyline_levels[level_index].start_x + _skyline_levels[level_index].width);
            const unsigned width_ = x1_ - x0_;
            assert(width_ > 0 && width_ <= _skyline_levels[level_index].width);

            if (_use_waste_map)
            {
                assert(y >= _skyline_levels[level_index].y);
                const unsigned waste_height = y - _skyline_levels[level_index].y;

                if (waste_height > 0)
                    _waste_map.access_free_rects().emplace_back(x0_, _skyline_levels[level_index].y, width_, waste_height);
            }

            if (x0_ > _skyline_levels[level_index].start_x)
            {
                // The rect only covers the right part of the skyline level.
                // We shorten the width of the existing skyline level
                // and add a new one to its right.
                assert(x1_ == _skyline_levels[level_index].start_x + _skyline_levels[level_index].width);
                assert(width_ < _skyline_levels[level_index].width);
                _skyline_levels[level_index].width -= width_;
                _skyline_levels.insert(_skyline_levels.begin() + static_cast<ptrdiff_t>(level_index + 1), {
                    .start_x = x0_,
                    .width = width_,
                    .y = y + rect.size.height,
                });
            }
            else if (x1_ < _skyline_levels[level_index].start_x + _skyline_levels[level_index].width)
            {
                // The rect only covers the left part of the skyline level.
                // We move it to the right, shorten its width accordingly,
                // and insert a new skyline level to its left.
                assert(x0_ == _skyline_levels[level_index].start_x);
                assert(width_ < _skyline_levels[level_index].width);
                _skyline_levels.insert(_skyline_levels.begin() + static_cast<ptrdiff_t>(level_index), {
                    .start_x = x0_,
                    .width = width_,
                    .y = y + rect.size.height,
                });
                _skyline_levels[level_index + 1].start_x += width_;
                _skyline_levels[level_index + 1].width -= width_;
            }
            else
            {
                // The rect spans the entire skyline level.
                // We simply update the vertical position
                // and leave the horizontal info unchanged.
                assert(x0_ == _skyline_levels[level_index].start_x);
                assert(x1_ == _skyline_levels[level_index].start_x + _skyline_levels[level_index].width);
                assert(width_ == _skyline_levels[level_index].width);
                _skyline_levels[level_index].y = y + rect.size.height;
            }

            if (x1_ == x1)
                break;
            assert(x1_ < x1);
        } // for "each" skyline level

        // Try to merge consecutive skyline levels.
        for (size_t i = 1; i < _skyline_levels.size(); )
        {
            if (_skyline_levels[i - 1].y != _skyline_levels[i].y)
            {
                ++i;
                continue;
            }

            _skyline_levels[i - 1].width += _skyline_levels[i].width;
            _skyline_levels.erase(_skyline_levels.begin() + static_cast<ptrdiff_t>(i));
        }

#ifdef _DEBUG
        assert(!_skyline_levels.empty());
        assert(_skyline_levels.front().start_x == 0);
        assert(_skyline_levels.back().start_x + _skyline_levels.back().width == bin_size().width);
        for (const auto& skyline_level : _skyline_levels)
        {
            assert(skyline_level.width > 0);
            assert(skyline_level.start_x + skyline_level.width <= bin_size().width);
            assert(skyline_level.y <= bin_size().height);
        }
        for (size_t i = 1; i < _skyline_levels.size(); ++i)
        {
            assert(_skyline_levels[i].start_x == _skyline_levels[i - 1].start_x + _skyline_levels[i - 1].width);
            assert(_skyline_levels[i].y != _skyline_levels[i - 1].y);
        }
#endif // _DEBUG
    }



    SkylinePacker::ScoredRect SkylinePacker::_find_best_rect(Size padded_size, bool allow_flip) const
    {
        ScoredRect best_scored_rect;

        Size flipped_size = padded_size.flipped();

        for (size_t i = 0; i < _skyline_levels.size(); ++i)
        {
            _try_update_best_rect(best_scored_rect, i, padded_size);
            if (allow_flip)
                _try_update_best_rect(best_scored_rect, i, flipped_size);
        } // for each skyline level

        return best_scored_rect;
    }

    void SkylinePacker::_try_update_best_rect(ScoredRect& best_scored_rect, size_t start_index, Size padded_size) const
    {
        const auto placement_range_result = _fits_on_level(start_index, padded_size);
        if (!placement_range_result.has_value())
            return;

        Rect rect{_skyline_levels[start_index].start_x, placement_range_result->y, padded_size};
        auto score_pair = _score_rect(placement_range_result->start_index, rect);

        // In the case of Best-Fit, we also try to place the item right-aligned
        // to see if we can achieve a smaller waste area this way.
        if (_heuristic == Heuristic::BestFit && placement_range_result->leftover_width > 0)
        {
            const Rect rect2{rect.position.x + placement_range_result->leftover_width, rect.position.y, rect.size};
            const auto score_pair2 = _score_rect(placement_range_result->start_index, rect2);

            if (score_pair2.is_better_than(score_pair))
            {
                rect = rect2;
                score_pair = score_pair2;
            }
        }

        if (!score_pair.is_better_than(best_scored_rect.score_pair))
            return;

        best_scored_rect.score_pair = score_pair;
        best_scored_rect.rect_result = rect;
        best_scored_rect.start_index = start_index;
    }



    std::optional<SkylinePacker::PlacementRange> SkylinePacker::_fits_on_level(size_t start_index, Size padded_size) const
    {
        assert(start_index < _skyline_levels.size());
        if (_skyline_levels[start_index].start_x + padded_size.width > bin_size().width)
            return {};

        unsigned y = _skyline_levels[start_index].y;
        unsigned width_left = padded_size.width;

        size_t level_index = start_index;
        while (width_left > _skyline_levels[level_index].width)
        {
            width_left -= _skyline_levels[level_index].width;
            ++level_index;
            assert(level_index < _skyline_levels.size());
            assert(_skyline_levels[level_index].start_x == _skyline_levels[level_index - 1].start_x + _skyline_levels[level_index - 1].width);
            y = std::max(y, _skyline_levels[level_index].y);
        }

        if (y + padded_size.height > bin_size().height)
            return {};

        return PlacementRange{
            .start_index = start_index,
            .leftover_width = _skyline_levels[level_index].width - width_left,
            .y = y,
        };
    }



    SkylinePacker::ScorePair SkylinePacker::_score_rect(size_t start_index, const Rect& rect) const
    {
        assert(start_index < _skyline_levels.size());

        switch (_heuristic)
        {

        case Heuristic::BottomLeft:
        {
            const unsigned top = rect.position.y + rect.size.height;
            const unsigned level_width = _skyline_levels[start_index].width;
            return {top, level_width};
        }

        case Heuristic::BestFit:
        {
            const size_t waste_area = _calculate_waste_area(start_index, rect.position, rect.size.width);
            const unsigned top = rect.position.y + rect.size.height;
            assert(waste_area <= static_cast<size_t>(std::numeric_limits<unsigned>::max()));
            return {static_cast<unsigned>(waste_area), top};
        }

        default:
            assert(false);
        } // switch selection heuristic

        return {};
    }

    size_t SkylinePacker::_calculate_waste_area(size_t start_index, Position position, unsigned width) const
    {
        assert(start_index < _skyline_levels.size());
        assert(_skyline_levels[start_index].start_x <= position.x);
        assert(_skyline_levels[start_index].start_x + width <= bin_size().width);
        assert(position.x + width <= bin_size().width);

        size_t waste_area = 0;

        const unsigned x0 = position.x;
        const unsigned x1 = position.x + width;
        const unsigned y = position.y;

        for (size_t level_index = start_index; ; ++level_index)
        {
            assert(level_index < _skyline_levels.size());
            // Check for skipped skyline levels due to a right-aligned position.
            if (x0 >= _skyline_levels[level_index].start_x + _skyline_levels[level_index].width)
                continue;

            const unsigned x0_ = std::max(x0, _skyline_levels[level_index].start_x);
            const unsigned x1_ = std::min(x1, _skyline_levels[level_index].start_x + _skyline_levels[level_index].width);
            const unsigned width_ = x1_ - x0_;
            assert(width_ > 0 && width_ <= _skyline_levels[level_index].width);

            assert(y >= _skyline_levels[level_index].y);
            const unsigned waste_height = y - _skyline_levels[level_index].y;

            waste_area += static_cast<size_t>(width_) * static_cast<size_t>(waste_height);

            if (x1_ == x1)
                break;
            assert(x1_ < x1);
        } // for "each" skyline level

        return waste_area;
    }



} // namespace al7::packing
