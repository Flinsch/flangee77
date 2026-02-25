#ifndef AL7_PACKING_ABSTRACTRECTPACKER_H
#define AL7_PACKING_ABSTRACTRECTPACKER_H

#include "./Rect.h"
#include "./RectOptions.h"

#include <vector>
#include <span>



/**
 * The implementations of the 2D rectangle bin packing algorithms are heavily
 * inspired by "A Thousand Ways to Pack the Bin - A Practical Approach to
 * Two-Dimensional Rectangle Bin Packing" by Jukka Jylänki.
 * https://github.com/juj/RectangleBinPack/blob/master/RectangleBinPack.pdf
 */



namespace al7::packing {



/**
 * Abstract base class for 2D bin packers for rectangular items.
 */
class AbstractRectPacker
{

public:
    /**
     * Constructs an abstract rect packer with the specified initially available
     * total size of the rectangular bin.
     */
    explicit AbstractRectPacker(Size bin_size) noexcept;

    virtual ~AbstractRectPacker() noexcept = default;



    /** Checks if an item of the specified size could be inserted. */
    bool can_insert(Size item_size, RectOptions options = {}) const;

    /** Tries to insert an item of the specified size. */
    std::optional<Rect> insert(Size item_size, RectOptions options = {});

    /**
     * Tries to insert items of the specified sizes in an offline batch mode.
     * For each requested item size, an optional rectangle is returned, in the
     * corresponding order. However, it is (deliberately) not guaranteed that
     * "earlier" (or even all) rectangles will be considered before later rectangles.
     * In case of doubt, the decision rests with the specific algorithm used.
     */
    std::vector<std::optional<Rect>> insert(const std::vector<Size>& item_sizes, RectOptions options = {});



    /** Resets the packer by essentially clearing the bin. */
    void reset();
    /** Resets the packer by essentially clearing the bin and adjusts the bin size. */
    void reset(Size bin_size);

    /** Returns the initially available total size of the rectangular bin. */
    Size bin_size() const { return _bin_size; }

    /** Returns the number of inserted items. */
    size_t item_count() const { return _count; }

    /**
     * Returns the area of the occupied space. This value simply corresponds to the
     * cumulative size of the emitted rectangles. Inherited classes may override
     * this function if a different calculation method is more appropriate due to
     * algorithm-specific circumstances.
     */
    virtual size_t used_area() const { return _used_area; }

    /** Calculates the occupancy as the used area divided by the total area. */
    float occupancy() const;



protected:
    virtual void _on_reset(Size bin_size) = 0;
    virtual bool _can_insert(Size padded_size, RectOptions options) const = 0;
    virtual std::optional<Rect> _insert(Size padded_size, RectOptions options) = 0;

    /** Override this function to implement offline batch logic that is "smarter" than the "naive" default logic that simply tries to add the items one after the other. */
    virtual void _insert(std::span<std::optional<Rect>> rect_results, std::vector<std::pair<Size, size_t>>&& padded_sizes_and_indices, RectOptions options);

    static Size _pad_item_size(Size item_size, unsigned padding);
    static Rect _unpad_item_rect(Rect padded_rect, unsigned padding);



private:
    /** The initially available total size of the rectangular bin. */
    Size _bin_size;

    /** The number of inserted items. */
    size_t _count = 0;
    /** The area of the occupied space. */
    size_t _used_area = 0;

}; // class AbstractRectPacker



} // namespace al7::packing

#endif // AL7_PACKING_ABSTRACTRECTPACKER_H
