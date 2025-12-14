#ifndef AL7_PACKING_MAXRECTSPACKER_H
#define AL7_PACKING_MAXRECTSPACKER_H
#include "./AbstractFreeRectPacker.h"

#include <vector>



namespace al7::packing {



/**
 * A 2D rectangle bin packer that implements the Maximal Rectangles algorithm family.
 *
 * The Maximal Rectangles algorithm family is excellent for mixed rectangle sizes.
 * Fragmentation is handled reasonably due to robust free-rectangle splitting.
 * Maximal Rectangles typically beats Skyline/Shelf in final compactness.
 *
 * Use Maximal Rectangles when quality matters most, e.g. for:
 * - High-quality general-purpose packing
 * - Texture atlases (glyphs, sprites)
 * - Image packing
 * - Offline situations where order can be changed
 *
 * Best Short Side Fit (BSSF) is the typical "go-to" for tight, robust packing and
 * usually offers the best overall results, while Best Area Fit (BAF) is good
 * especially with mixed sizes.
 */
class MaxRectsPacker
    : public AbstractFreeRectPacker
{

public:
    enum struct Heuristic
    {
        BestShortSideFit,
        //BestLongSideFit,
        BestAreaFit,
        //BottomLeft,
        //ContactPointRule,
    };



    /**
     * Constructs a Maximal Rectangles packer with the specified initially available
     * total size of the rectangular bin and a heuristic for inserting new items.
     */
    MaxRectsPacker(Size bin_size, Heuristic heuristic = Heuristic::BestShortSideFit) noexcept;

    ~MaxRectsPacker() noexcept override = default;



    /** Returns the heuristic for inserting new items. */
    Heuristic get_heuristic() const { return _heuristic; }
    /** Sets the heuristic for inserting new items. */
    void set_heuristic(Heuristic heuristic) { _heuristic = heuristic; }



protected:
    ScorePair _score_free_rect(Rect free_rect, Size padded_size) const override;
    void _split_free_rects(std::vector<Rect>& free_rects, size_t free_rect_index, const Rect& inserted_rect) override;

    void _split_free_rects_impl(std::vector<Rect>& free_rects, const Rect& inserted_rect);
    void _merge_free_rects_helper(std::vector<Rect>& free_rects);



private:
    /** The heuristic for inserting new items. */
    Heuristic _heuristic;

    /** Temporary buffer for new free rectangles resulting form splitting previous ones. */
    std::vector<Rect> _new_free_rects;

}; // class MaxRectsPacker



} // namespace al7::packing

#endif // AL7_PACKING_MAXRECTSPACKER_H
