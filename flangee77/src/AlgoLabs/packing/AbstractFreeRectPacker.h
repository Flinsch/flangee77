#ifndef AL7_PACKING_ABSTRACTFREERECTPACKER_H
#define AL7_PACKING_ABSTRACTFREERECTPACKER_H
#include "./AbstractRectPacker.h"

#include <limits>



namespace al7::packing {



/**
 * Abstract intermediate class for 2D rectangle bin packers that are based on a
 * "list" free rectangles that describe the remaining available space.
 */
class AbstractFreeRectPacker
    : public AbstractRectPacker
{

public:
    /**
     * Constructs an abstract free rect packer with the specified initially available
     * total size of the rectangular bin.
     */
    AbstractFreeRectPacker(Size bin_size) noexcept;

    ~AbstractFreeRectPacker() noexcept override = default;



    /** Returns the "list" of rectangles that describe the remaining available space. */
    const std::vector<Rect>& free_rects() const { return _free_rects; }

    /**
     * Accesses the "list" of rectangles that describe the remaining available space,
     * to be manipulated manually. Use with caution to avoid creating nonsensical
     * configurations that do not conform to the algorithm.
     */
    std::vector<Rect>& access_free_rects() { return _free_rects; }



protected:
    struct ScorePair
    {
        /** The primary score value (lower is "better"). */
        int primary = std::numeric_limits<int>::max();
        /** The secondary score value, which comes into play when two primary values are equal (lower is "better"). */
        int secondary = std::numeric_limits<int>::max();

        ScorePair() noexcept = default;
        ScorePair(int primary, int secondary) noexcept : primary(primary), secondary(secondary) {};

        bool is_better_than(ScorePair other) const noexcept
        {
            return primary < other.primary || (primary == other.primary && secondary < other.secondary);
        }
    };

    struct ScoredRect
    {
        ScorePair score_pair;
        std::optional<Rect> rect_result;
        size_t free_rect_index = 0;
    };

    virtual ScorePair _score_free_rect(Rect free_rect, Size padded_size) const = 0;
    virtual void _split_free_rects(std::vector<Rect>& free_rects, size_t free_rect_index, const Rect& inserted_rect) = 0;

    void _on_reset(Size bin_size) override;
    bool _can_insert(Size padded_size, RectOptions options) const override;
    std::optional<Rect> _insert(Size padded_size, RectOptions options) override;
    void _insert(std::span<std::optional<Rect>> rect_results, std::vector<std::pair<Size, size_t>>&& padded_sizes_and_indices, RectOptions options) override;

    std::optional<Rect> _insert_impl(Size padded_size, bool allow_flip);
    void _insert_impl(std::span<std::optional<Rect>> rect_results, std::vector<std::pair<Size, size_t>> padded_sizes_and_indices, bool allow_flip);

    void _place_rect(size_t free_rect_index, const Rect& rect);

    ScoredRect _find_best_rect(Size padded_size, bool allow_flip) const;



private:
    /** The "list" of rectangles that describe the remaining available space. */
    std::vector<Rect> _free_rects;

}; // class AbstractFreeRectPacker



} // namespace al7::packing

#endif // AL7_PACKING_ABSTRACTFREERECTPACKER_H
