#ifndef AL7_PACKING_GUILLOTINEPACKER_H
#define AL7_PACKING_GUILLOTINEPACKER_H
#include "./AbstractFreeRectPacker.h"



namespace al7::packing {



/**
 * A 2D rectangle bin packer that implements the Guillotine algorithm family.
 *
 * The Guillotine algorithm family produces straight orthogonal cuts going from one
 * edge to the opposite edge. Fairly fast with predictable results, but less tight
 * than Maximal Rectangles.
 *
 * Guillotine is classically used to produce cuttable layouts, e.g. for woodworking,
 * glass cutting, sheet-metal cutting, CNC, microelectronics, printing, etc. These
 * are not really of interest within the scope of this (graphics) framework.
 * However, Guillotine is sometimes also used in bin-providers that should produce
 * bisecting end-to-end cuts. For example, Skyline can be configured to use
 * Guillotine as a fallback waste map, in order to utilize any remaining free spaces
 * that would otherwise be lost as "offcuts".
 *
 * Best Area Fit (BAF) together with Shorter Leftover Axis (SLA) is the standard
 * high-quality combo, while Best Short Side Fit (BSSF) and/or Minimize Area (MINA)
 * are good as well.
 */
class GuillotinePacker
    : public AbstractFreeRectPacker
{

public:
    enum struct SelectionHeuristic
    {
        BestAreaFit,
        BestShortSideFit,
        //BestLongSideFit,
        //WorstAreaFit,
        //WorstShortSideFit,
        //WorstLongSideFit,
    };

    enum struct SplitHeuristic
    {
        ShorterLeftoverAxis,
        //LongerLeftoverAxis,
        MinimizeArea,
        //MaximizeArea,
        //ShorterAxis,
        //LongerAxis,
    };



    /**
     * Constructs a Guillotine packer with the specified initially available total size
     * of the rectangular bin and selection/split heuristics for inserting new items.
     */
    GuillotinePacker(Size bin_size, SelectionHeuristic selection_heuristic = SelectionHeuristic::BestAreaFit, SplitHeuristic split_heuristic = SplitHeuristic::ShorterLeftoverAxis) noexcept;

    ~GuillotinePacker() noexcept override = default;



    /** Returns the heuristic for choosing which free rectangle to place an item in. */
    SelectionHeuristic get_selection_heuristic() const { return _selection_heuristic; }
    /** Sets the heuristic for choosing which free rectangle to place an item in. */
    void set_selection_heuristic(SelectionHeuristic selection_heuristic) { _selection_heuristic = selection_heuristic; }

    /** Returns the heuristic for choosing how to split a free rectangle after placing an item. */
    SplitHeuristic get_split_heuristic() const { return _split_heuristic; }
    /** Sets the heuristic for choosing how to split a free rectangle after placing an item. */
    void set_split_heuristic(SplitHeuristic split_heuristic) { _split_heuristic = split_heuristic; }



protected:
    enum struct CutOrientation
    {
        Horizontal,
        Vertical,
    };

    ScorePair _score_free_rect(Rect free_rect, Size padded_size) const override;
    void _split_free_rects(std::vector<Rect>& free_rects, size_t free_rect_index, const Rect& inserted_rect) override;

    std::pair<Rect, Rect> _split_free_rect(const Rect& free_rect, const Rect& inserted_rect) const;

    CutOrientation _cut_orientation(const Rect& free_rect, const Rect& inserted_rect) const;



private:
    /** The heuristic for choosing which free rectangle to place an item in. */
    SelectionHeuristic _selection_heuristic;
    /** The heuristic for choosing how to split a free rectangle after placing an item. */
    SplitHeuristic _split_heuristic;

}; // class GuillotinePacker



} // namespace al7::packing

#endif // AL7_PACKING_GUILLOTINEPACKER_H
