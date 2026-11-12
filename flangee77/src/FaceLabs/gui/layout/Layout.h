#ifndef FL7_GUI_LAYOUT_LAYOUT_H
#define FL7_GUI_LAYOUT_LAYOUT_H

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
 * change (see `Container::add_child()`/`remove_child()`/`set_size()`).
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
     * strategy. Called automatically by Container whenever it matters; never needs
     * to be called directly.
     */
    virtual void perform(Container& container) = 0;

    /**
     * Returns how large this layout's own strategy would ideally like container to
     * be, given its current children's own effective preferred sizes, queried by
     * `Container::_calculate_preferred_size()` while this layout is attached,
     * instead of the plain (no-layout) bounding-box calculation it otherwise falls
     * back to. Default: `{0, 0}` (no particular opinion; a layout that doesn't
     * actually resize/reposition children, e.g., `DefaultLayout`, has none to
     * report either).
     */
    virtual ml7::Vector2f calculate_preferred_size(const Container& container) const { return {}; }

    /**
     * Returns how large this layout's own strategy would ideally like container to
     * be, given its current children's own effective minimum sizes, queried by
     * `Container::_calculate_minimum_size()` while this layout is attached,
     * instead of the plain (no-layout) bounding-box calculation it otherwise falls
     * back to. Default: `{0, 0}` (no particular opinion; a layout that doesn't
     * actually resize/reposition children, e.g., `DefaultLayout`, has none to
     * report either).
     */
    virtual ml7::Vector2f calculate_minimum_size(const Container& container) const { return {}; }

}; // class Layout



} // namespace fl7::gui::layout

#endif // FL7_GUI_LAYOUT_LAYOUT_H
