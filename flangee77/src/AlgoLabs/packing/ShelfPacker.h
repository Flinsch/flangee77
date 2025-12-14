#ifndef AL7_PACKING_SHELFPACKER_H
#define AL7_PACKING_SHELFPACKER_H
#include "./AbstractRectPacker.h"

#include <vector>



namespace al7::packing {



/**
 * A 2D rectangle bin packer that implements the Shelf algorithm family.
 *
 * Fastest algorithm family, aside from trivial "sorted placement" or naive
 * approaches. May waste space when sizes vary widely. Very online-friendly.
 * If in doubt, use Maximum Rectangles or Skyline for better quality results.
 *
 * Use Shelf when speed and simplicity matter more than perfect tightness, e.g. for:
 * - Font glyph atlases (classic use case)
 * - Tall/narrow or similarly sized items
 * - Simple or runtime packing where deterministic shelves are desired
 *
 * Best Height Fit (BHF) is the best practical Shelf variant, while First Fit (FF)
 * is also not too bad, but even faster.
 */
class ShelfPacker
    : public AbstractRectPacker
{

public:
    enum struct Heuristic
    {
        //NextFit,
        FirstFit,
        BestHeightFit,
        //BestWidthFit,
        //BestAreaFit,
        //WorstWidthFit,
        //WorstAreaFit,
    };



    /**
     * Constructs a Shelf packer with the specified initially available total size
     * of the rectangular bin and a heuristic for inserting new items.
     */
    ShelfPacker(Size bin_size, Heuristic heuristic = Heuristic::BestHeightFit) noexcept;

    ~ShelfPacker() noexcept override = default;



    /** Returns the heuristic for inserting new items. */
    Heuristic get_heuristic() const { return _heuristic; }
    /** Sets the heuristic for inserting new items. */
    void set_heuristic(Heuristic heuristic) { _heuristic = heuristic; }



protected:
    struct Shelf
    {
        /** The current horizontal position where the next element would be inserted. */
        unsigned current_x;
        /** The bottom position of the shelf. */
        unsigned start_y;
        /** The (current/potential) height of the shelf. */
        unsigned height;
        /** The height of the tallest item on the shelf (or 0). */
        unsigned item_height;
    };

    void _on_reset(Size bin_size) override;
    bool _can_insert(Size padded_size, RectOptions options) const override;
    std::optional<Rect> _insert(Size padded_size, RectOptions options) override;

    std::optional<Rect> _insert_impl(Size padded_size, bool allow_flip);

    bool _try_fit_on_shelf(const Shelf& shelf, Size& padded_size, bool allow_flip) const;
    Rect _add_to_shelf(Shelf& shelf, Size padded_size);



private:
    /** The heuristic for inserting new items. */
    Heuristic _heuristic;

    /** The "list" of the shelves. */
    std::vector<Shelf> _shelves;

}; // class ShelfPacker



} // namespace al7::packing

#endif // AL7_PACKING_SHELFPACKER_H
