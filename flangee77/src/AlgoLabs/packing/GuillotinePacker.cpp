#include "GuillotinePacker.h"



namespace al7::packing {



    GuillotinePacker::GuillotinePacker(Size bin_size, SelectionHeuristic selection_heuristic, SplitHeuristic split_heuristic)
        : AbstractFreeRectPacker(bin_size)
        , _selection_heuristic(selection_heuristic)
        , _split_heuristic(split_heuristic)
    {
    }



    GuillotinePacker::ScorePair GuillotinePacker::_score_free_rect(Rect free_rect, Size padded_size) const
    {
        switch (_selection_heuristic)
        {

        case SelectionHeuristic::BestAreaFit:
        {
            const int leftover_width = std::abs(static_cast<int>(free_rect.size.width) - static_cast<int>(padded_size.width));
            const int leftover_height = std::abs(static_cast<int>(free_rect.size.height) - static_cast<int>(padded_size.height));
            const int leftover_area = static_cast<int>(static_cast<ptrdiff_t>(free_rect.size.area()) - static_cast<ptrdiff_t>(padded_size.area()));
            const int area_fit = leftover_area;
            const int short_side_fit = std::min(leftover_width, leftover_height);
            return {area_fit, short_side_fit};
        }

        case SelectionHeuristic::BestShortSideFit:
        {
            const int leftover_width = std::abs(static_cast<int>(free_rect.size.width) - static_cast<int>(padded_size.width));
            const int leftover_height = std::abs(static_cast<int>(free_rect.size.height) - static_cast<int>(padded_size.height));
            const int short_side_fit = std::min(leftover_width, leftover_height);
            const int long_side_fit = std::max(leftover_width, leftover_height);
            return {short_side_fit, long_side_fit};
        }

        default:
            assert(false);
        } // switch selection heuristic

        return {};
    }

    void GuillotinePacker::_split_free_rects(std::vector<Rect>& free_rects, size_t free_rect_index, const Rect& inserted_rect)
    {
        assert(free_rect_index < free_rects.size());

        const Rect free_rect = free_rects[free_rect_index];
        assert(free_rect.contains(inserted_rect));

        // Remove the free rect and "replace" it
        // with up to two newly subdivided rects.
        free_rects.erase(free_rects.begin() + static_cast<ptrdiff_t>(free_rect_index));

        // Split free rect into up to two new rects.
        const auto [top_left, bottom_right] = _split_free_rect(free_rect, inserted_rect);
        if (top_left.size.width > 0 && top_left.size.height > 0)
            free_rects.push_back(top_left);
        if (bottom_right.size.width > 0 && bottom_right.size.height > 0)
            free_rects.push_back(bottom_right);

#ifdef _DEBUG
        for (const auto& r1 : free_rects)
            for (const auto& r2 : free_rects)
                assert(r1.overlaps(r2) == (&r1 == &r2));
#endif // _DEBUG
    }



    std::pair<Rect, Rect> GuillotinePacker::_split_free_rect(const Rect& free_rect, const Rect& inserted_rect) const
    {
        assert(inserted_rect.position == free_rect.position);
        assert(free_rect.size.covers(inserted_rect.size));

        Rect top_left;
        top_left.position.x = free_rect.position.x;
        top_left.position.y = free_rect.position.y + inserted_rect.size.height;
        top_left.size.height = free_rect.size.height - inserted_rect.size.height;

        Rect bottom_right;
        bottom_right.position.x = free_rect.position.x + inserted_rect.size.width;
        bottom_right.position.y = free_rect.position.y;
        bottom_right.size.width = free_rect.size.width - inserted_rect.size.width;

        const auto cut_orientation = _cut_orientation(free_rect, inserted_rect);
        if (cut_orientation == CutOrientation::Vertical)
        {
            top_left.size.width = inserted_rect.size.width;
            bottom_right.size.height = free_rect.size.height;
        }
        else
        {
            top_left.size.width = free_rect.size.width;
            bottom_right.size.height = inserted_rect.size.height;
        }

        return {top_left, bottom_right};
    }

    GuillotinePacker::CutOrientation GuillotinePacker::_cut_orientation(const Rect& free_rect, const Rect& inserted_rect) const
    {
        const int leftover_width = std::abs(static_cast<int>(free_rect.size.width) - static_cast<int>(inserted_rect.size.width));
        const int leftover_height = std::abs(static_cast<int>(free_rect.size.height) - static_cast<int>(inserted_rect.size.height));

        bool cut_vertically = false;

        switch (_split_heuristic)
        {

        case SplitHeuristic::ShorterLeftoverAxis:
        {
            // Split along the shorter leftover axis.
            cut_vertically = leftover_height < leftover_width;
            break;
        }

        case SplitHeuristic::MinimizeArea:
        {
            // Minimize smaller area => maximize larger area.
            // Prefer two different-sized rects over two more even-sized rects.
            cut_vertically = inserted_rect.size.width * leftover_height < leftover_width * inserted_rect.size.height;
            break;
        }

        default:
            assert(false);
        } // switch selection heuristic

        return cut_vertically ? CutOrientation::Vertical : CutOrientation::Horizontal;
    }



} // namespace al7::packing
