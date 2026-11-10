#ifndef FL7_GUI_LAYOUT_BOXLAYOUT_H
#define FL7_GUI_LAYOUT_BOXLAYOUT_H
#include "./Layout.h"

#include "../Face.h"

#include <MathLabs/Vector2.h>

#include <unordered_map>



namespace fl7::gui::layout {



/**
 * A fixed space kept on each of a container's four edges; see
 * `BoxLayout::get_padding()`/`set_padding()`.
 */
struct Padding
{
    Padding() = default;

    /** The same padding on every side. */
    Padding(float uniform) : left(uniform), top(uniform), right(uniform), bottom(uniform) {} // Deliberately not explicit: the whole point is to allow a bare float as padding.
    Padding(float left, float top, float right, float bottom) : left(left), top(top), right(right), bottom(bottom) {}

    float left = 0.0f;
    float top = 0.0f;
    float right = 0.0f;
    float bottom = 0.0f;
}; // struct Padding



/**
 * Arranges a container's children one after another along a single axis (see
 * `Orientation`), separated by a fixed `get_gap()`, within a fixed `get_padding()`
 * kept around the whole sequence. This divides the container into a sequence of
 * cells, one per child: along the main axis, a cell is either exactly as big as its
 * own child's `get_effective_preferred_size()` (a stretch factor of 0 or less; see
 * `get_stretch()`) or a share of whatever space is left over once every such
 * fixed-size child's cell has been accounted for, proportional to its own stretch
 * factor relative to the other stretched children's (every child's default: see
 * `get_stretch()`, every cell then just splits the main axis evenly, unless some
 * are explicitly pinned to their own preferred size via 0 instead); along the
 * perpendicular (cross) axis, every cell is simply the container's own full extent
 * there (minus padding).
 *
 * How a child actually sits within its own cell (possibly larger than its own
 * preferred size: filled, shrunk-and-aligned near/far, or shrunk-and-centered) is
 * then up to its own `Face::get_anchor()` (the same flags `DefaultLayout` reads for
 * the other purpose of reacting to a resize), independently per axis: `Left`/`Top`
 * alone (whichever applies to that axis; the default anchor) aligns it near, at its
 * own `get_effective_preferred_size()`; `Right`/`Bottom` alone aligns it far
 * instead, same size; neither aligns it centered; both together fills the cell,
 * overriding its own preferred size. A stretched child (the default; see
 * `get_stretch()`) whose anchor doesn't fill the main axis this way just sits
 * somewhere within its own, larger cell instead of actually growing: stretch and
 * anchor are independent, exactly as they already are for the cross axis. A plain
 * stack of fixed-size children, each kept at its own preferred size regardless of
 * this layout's own size, therefore needs 0 explicitly set on every one of them
 * (see `set_stretch()`), not just left at the default.
 *
 * Each child's own `get_effective_preferred_size()` is re-queried live on every
 * `perform()` call, not cached here: it can genuinely change over time (e.g.,
 * `faces::Label`'s, after its text changes). Any face type whose own calculation is
 * actually expensive to repeat is expected to cache that internally instead (see
 * `faces::Label::_calculate_text_size()`), where the relevant inputs (and when
 * they've changed) are actually known.
 */
class BoxLayout
    : public Layout
{

public:
    enum struct Orientation
    {
        Horizontal,
        Vertical,
    }; // enum struct Orientation



    explicit BoxLayout(Orientation orientation, float gap = 0.0f, Padding padding = {})
        : _orientation(orientation)
        , _gap(gap)
        , _padding(padding)
    {
    }

    BoxLayout(const BoxLayout&) = delete;
    BoxLayout& operator=(const BoxLayout&) = delete;
    BoxLayout(BoxLayout&&) = delete;
    BoxLayout& operator=(BoxLayout&&) = delete;

    ~BoxLayout() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this box layout's orientation (fixed at construction). */
    Orientation get_orientation() const { return _orientation; }

    /** Returns the fixed gap kept between consecutive children. Default: 0. */
    float get_gap() const { return _gap; }

    /** Sets the fixed gap kept between consecutive children. */
    void set_gap(float gap) { _gap = gap; }

    /** Returns the fixed space kept around the whole sequence of children, on every side. Default: none. */
    Padding get_padding() const { return _padding; }

    /** Sets the fixed space kept around the whole sequence of children, independently on each side. */
    void set_padding(Padding padding) { _padding = padding; }

    /** Sets the fixed space kept around the whole sequence of children, the same on every side; see `set_padding(Padding)`. */
    void set_padding(float padding) { set_padding(Padding(padding)); }

    /**
     * Returns the given child's stretch factor; see `set_stretch()`. Default: 1
     * (every child, until told otherwise, participates evenly in distributing the
     * main-axis space).
     */
    float get_stretch(const Face& child) const;

    /**
     * Sets the given child's stretch factor: 0 or less means its cell is exactly
     * its own `get_effective_preferred_size()` along this layout's own axis, not
     * participating in the distribution at all; a positive value (1, the default
     * for every child until told otherwise) means its cell instead takes that
     * share of whatever space is left over once every non-stretched (stretch <= 0)
     * child's cell has taken its own preferred size, relative to the other
     * stretched children's own factors (e.g., two children both left at the
     * default 1 split the leftover space evenly; one set to 2 and another left at
     * 1 split it two-to-one). Either way, purely about the cell's own size,
     * independent of whether the child actually fills it (see this class's own
     * doc comment: that's its anchor's call, not this one's).
     */
    void set_stretch(const Face& child, float stretch);



    // #############################################################################
    // Layout Implementation
    // #############################################################################

    /**
     * (Re-)arranges container's current children within it, per the
     * sequencing/stretching/cross-axis-alignment rules described on this class.
     */
    void perform(Container& container) override;



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Fits a child's own preferred size within its cell, along a single axis,
     * per anchor's near_flag/far_flag; see this class's own doc comment and
     * this method's own declaration (BoxLayout.h) for the exact rules.
     */
    static void _fit_in_cell(Anchor anchor, Anchor near_flag, Anchor far_flag, float cell_position, float cell_size, float preferred, float& out_position, float& out_size);



    // #############################################################################
    // Attributes
    // #############################################################################

    const Orientation _orientation;
    float _gap;
    Padding _padding;

    std::unordered_map<const Face*, float> _stretches;

}; // class BoxLayout



} // namespace fl7::gui::layout

#endif // FL7_GUI_LAYOUT_BOXLAYOUT_H
