#ifndef FL7_GUI_LAYOUT_BOXLAYOUT_H
#define FL7_GUI_LAYOUT_BOXLAYOUT_H
#include "./Layout.h"

#include <unordered_map>



namespace fl7::gui {
class Face;
} // namespace fl7::gui

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
 * `Orientation`), separated by a fixed `get_gap()`, within a fixed
 * `get_padding()` kept around the whole sequence. Along that main axis, each
 * child either takes exactly its own `get_preferred_size()` (the default: see
 * `get_stretch()`) or a share of whatever space is left over once every such
 * fixed-size child has been accounted for, proportional to its own stretch factor
 * relative to the other stretched children's.
 *
 * Along the perpendicular (cross) axis, a child is sized/aligned per its own
 * `Face::get_anchor()` (the same flags `DefaultLayout` reads for the other purpose
 * of reacting to a resize): `Left`/`Top` alone (whichever applies to that axis; the
 * default anchor) aligns it near, at its own `get_preferred_size()`; `Right`/`Bottom`
 * alone aligns it far instead, same size; neither aligns it centered; both together
 * fills the container's full extent along that axis, overriding its own preferred
 * size.
 *
 * Unlike `DefaultLayout`, this one does not react to the container being resized by
 * shifting/stretching children from some originally captured state: `perform()`
 * always recomputes every child's position/size from scratch, so there's nothing to
 * go stale.
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
     * Returns the given child's stretch factor; see `set_stretch()`. Default: 0
     * (not stretched: takes exactly its own `get_preferred_size()`).
     */
    float get_stretch(const Face& child) const;

    /**
     * Sets the given child's stretch factor: 0 (the default) or less means it's not
     * stretched at all, taking exactly its own `get_preferred_size()` along this
     * layout's own axis; a positive value means it instead takes that share of
     * whatever space is left over once every non-stretched child has taken its own
     * preferred size, relative to the other stretched children's own factors (e.g.,
     * two children both set to 1 split the leftover space evenly; one set to 2 and
     * another to 1 split it two-to-one).
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
    // Attributes
    // #############################################################################

    const Orientation _orientation;
    float _gap;
    Padding _padding;

    std::unordered_map<const Face*, float> _stretches;

}; // class BoxLayout



} // namespace fl7::gui::layout

#endif // FL7_GUI_LAYOUT_BOXLAYOUT_H
