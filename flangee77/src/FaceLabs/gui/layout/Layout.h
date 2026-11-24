#ifndef FL7_GUI_LAYOUT_LAYOUT_H
#define FL7_GUI_LAYOUT_LAYOUT_H

#include "../Face.h"

#include <MathLabs/Vector2.h>



namespace fl7::gui {
class Container;
} // namespace fl7::gui

namespace fl7::gui::layout {



/**
 * Base class for a pluggable strategy that arranges a Container's current children
 * within it (see `Container::set_layout()`). A Container with no layout attached
 * (the default) leaves its children exactly wherever they've been explicitly
 * positioned/sized, entirely unmanaged; attaching one of these takes over that job
 * instead, kept in sync automatically as the container's children or own size
 * change (see `Container::add_child()`, `remove_child()`, or `set_size()`).
 *
 * Every concrete layout also offers its own `add_child()` (and, where its own
 * strategy actually has a notion of "where", e.g.,`BoxLayout::insert_child()`,
 * further placement methods besides), each with whatever parameters that
 * strategy actually needs to place a new child; adding one through
 * `Container::add_child()` directly instead bypasses that, leaving it
 * unarranged by this layout.
 */
class Layout
{

public:
    virtual ~Layout() = default;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * (Re-)arranges container's current children within it, per this layout's own
     * strategy. Called automatically by `Container` whenever it matters; never
     * needs to be called directly.
     */
    virtual void perform(Container& container) = 0;

    /**
     * Returns how large this layout's own strategy would ideally like container to
     * be, given its current children's own effective preferred sizes, while this
     * layout is attached, instead of the plain (no-layout) bounding-box
     * calculation a `Container` otherwise falls back to. Default: `{0, 0}` (no
     * particular opinion; a layout that doesn't actually resize/reposition
     * children, e.g., `DefaultLayout`, has none to report either).
     */
    virtual ml7::Vector2f calculate_preferred_size(const Container& container) const { return {}; }

    /**
     * Returns how large this layout's own strategy would ideally like container to
     * be, given its current children's own effective minimum sizes, while this
     * layout is attached, instead of the plain (no-layout) bounding-box
     * calculation a `Container` otherwise falls back to. Default: `{0, 0}` (no
     * particular opinion; a layout that doesn't actually resize/reposition
     * children, e.g., `DefaultLayout`, has none to report either).
     */
    virtual ml7::Vector2f calculate_minimum_size(const Container& container) const { return {}; }



protected:

    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Returns this layout's own container, set once by `Container::set_layout()`
     * right after construction: the one a concrete layout's own `add_child()`
     * (see, e.g., `BoxLayout::add_child()`) actually adds a new child through.
     * Read-only from here on purpose: only `Container::set_layout()` itself
     * ever gets to set this, not a subclass.
     */
    Container* _get_container() const { return _container; }

    /**
     * Fits a child's own preferred size within its cell, along a single axis,
     * never below `minimum`, per anchor's `near_flag` and `far_flag`. Shared by
     * every concrete layout that divides its container into anchor-fitted
     * cells (see `BoxLayout::perform()` or `GridLayout::perform()`).
     */
    static void _fit_in_cell(Anchor anchor, Anchor near_flag, Anchor far_flag, float cell_position, float cell_size, float preferred, float minimum, float& out_position, float& out_size);



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    Container* _container = nullptr;

    friend class gui::Container;

}; // class Layout



} // namespace fl7::gui::layout

#endif // FL7_GUI_LAYOUT_LAYOUT_H
