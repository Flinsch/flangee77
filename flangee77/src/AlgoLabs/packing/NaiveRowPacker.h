#ifndef AL7_PACKING_NAIVEROWPACKER_H
#define AL7_PACKING_NAIVEROWPACKER_H
#include "./AbstractRectPacker.h"



namespace al7::packing {



/**
 * A simple 2D bin packer that places rectangular items sequentially in horizontal
 * rows. Each new item is placed at the current row's cursor position (x, y). The
 * cursor advances to the right by the item's width. If the item does not fit within
 * the current row, a new row is started at x = 0 and y is advanced by the maximum
 * item height of the previous row.
 *
 * This naive approach is essentially the same as the Next-Fit (NF) heuristic of a
 * Shelf packer.
 *
 * Pretty fast, as it doesn't perform any searching or backtracking, but may produce
 * significant unused space with items of varying heights and/or with wide items
 * that start a new row.
 */
class NaiveRowPacker
    : public AbstractRectPacker
{
public:
    /**
     * Constructs a naive row packer with the specified initially available total
     * size of the rectangular bin.
     */
    explicit NaiveRowPacker(Size bin_size) noexcept;

    ~NaiveRowPacker() noexcept override = default;



protected:
    void _on_reset(Size bin_size) override;
    bool _can_insert(Size padded_size, RectOptions options) const override;
    std::optional<Rect> _insert(Size padded_size, RectOptions options) override;

    std::optional<Rect> _find_insert_rect(Size padded_size, bool allow_flip) const;



private:
    /** The position where the next item will be inserted, provided it still fits in the current row. */
    Position _position = {0, 0};
    /** The height of the tallest item within the current row. */
    unsigned _row_height = 0;

}; // class NaiveRowPacker



} // namespace al7::packing

#endif // AL7_PACKING_NAIVEROWPACKER_H
