#ifndef AL7_PACKING_SKYLINEPACKER_H
#define AL7_PACKING_SKYLINEPACKER_H
#include "./AbstractRectPacker.h"

#include "./GuillotinePacker.h"

#include <vector>



namespace al7::packing {



/**
 * A 2D rectangle bin packer that implements the Skyline algorithm family.
 *
 * This Tetris-like algorithm family has not quite the same quality as Maximal
 * Rectangles, but much lower complexity. Generates less fragmentation and more
 * predictable shapes. Excellent compromise between speed and packing quality.
 *
 * Use Skyline when you need fast, good packing with incremental updates, e.g. for:
 * - Dynamic/online packing (adding rectangles over time)
 * - Texture atlases that update or pack at runtime
 * - Cases where a "shelf-like but smarter" method is desired
 *
 * Best Fit (BF) with an additional Waste Map is the best overall Skyline variant,
 * while BF alone is also not too bad, but faster.
 */
class SkylinePacker
    : public AbstractRectPacker
{

public:
    enum struct Heuristic
    {
        BottomLeft,
        BestFit,
    };



    /**
     * Constructs a Skyline packer with the specified initially available total size
     * of the rectangular bin and a heuristic for inserting new items.
     */
    explicit SkylinePacker(Size bin_size, Heuristic heuristic = Heuristic::BestFit, bool use_waste_map = false);

    ~SkylinePacker() noexcept override = default;



    /** Returns the heuristic for inserting new items. */
    Heuristic get_heuristic() const { return _heuristic; }
    /** Sets the heuristic for inserting new items. */
    void set_heuristic(Heuristic heuristic) { _heuristic = heuristic; }



protected:
    struct SkylineLevel
    {
        /** The left position of the skyline level line. */
        unsigned start_x;
        /** The width of the skyline level line. */
        unsigned width;
        /** The vertical position of the skyline level line. */
        unsigned y;
    };

    struct PlacementRange
    {
        /** The index of the leftmost involved skyline level. */
        size_t start_index;
        /** The excess width, which can be seen as room for horizontal shifts of the item "within" the involved skyline levels. */
        unsigned leftover_width;
        /** The bottom position of the item's placement. */
        unsigned y;
    };

    struct ScorePair
    {
        /** The primary score value (lower is "better"). */
        unsigned primary = std::numeric_limits<unsigned>::max();
        /** The secondary score value, which comes into play when two primary values are equal (lower is "better"). */
        unsigned secondary = std::numeric_limits<unsigned>::max();

        ScorePair() noexcept = default;
        ScorePair(unsigned primary, unsigned secondary) noexcept : primary(primary), secondary(secondary) {};

        bool is_better_than(ScorePair other) const noexcept
        {
            return primary < other.primary || (primary == other.primary && secondary < other.secondary);
        }
    };

    struct ScoredRect
    {
        ScorePair score_pair;
        std::optional<Rect> rect_result;
        size_t start_index = 0;
    };

    void _on_reset(Size bin_size) override;
    bool _can_insert(Size padded_size, RectOptions options) const override;
    std::optional<Rect> _insert(Size padded_size, RectOptions options) override;

    void _place_rect(size_t start_index, const Rect& rect);

    ScoredRect _find_best_rect(Size padded_size, bool allow_flip) const;
    void _try_update_best_rect(ScoredRect& best_scored_rect, size_t start_index, Size padded_size) const;

    std::optional<PlacementRange> _fits_on_level(size_t start_index, Size padded_size) const;

    ScorePair _score_rect(size_t start_index, const Rect& rect) const;
    size_t _calculate_waste_area(size_t start_index, Position position, unsigned width) const;



private:
    /** The heuristic for inserting new items. */
    Heuristic _heuristic;
    /** Whether to use an additional waste map. */
    bool _use_waste_map;

    /** The waste map in the form of a Guillotine packer. */
    GuillotinePacker _waste_map;

    /** The "list" of skyline levels that describe the skyline horizon/envelope. */
    std::vector<SkylineLevel> _skyline_levels;

}; // class SkylinePacker



} // namespace al7::packing

#endif // AL7_PACKING_SKYLINEPACKER_H
