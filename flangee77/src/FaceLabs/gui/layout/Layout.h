#ifndef FL7_GUI_LAYOUT_LAYOUT_H
#define FL7_GUI_LAYOUT_LAYOUT_H

#include <CoreLabs/root.h>



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

}; // class Layout



} // namespace fl7::gui::layout

#endif // FL7_GUI_LAYOUT_LAYOUT_H
