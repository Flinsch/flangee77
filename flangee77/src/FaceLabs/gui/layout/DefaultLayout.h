#ifndef FL7_GUI_LAYOUT_DEFAULTLAYOUT_H
#define FL7_GUI_LAYOUT_DEFAULTLAYOUT_H
#include "./Layout.h"

#include <unordered_map>



namespace fl7::gui {
class Face;
} // namespace fl7::gui

namespace fl7::gui::layout {



/**
 * Keeps each child anchored to whichever of its own parent-relative edges its
 * `Face::get_anchor()` flags (see `gui::Anchor`) say, as the container is resized:
 * Right without Left keeps its distance from the right edge (shifting as the
 * container grows/shrinks); Left and Right together instead stretches it to keep
 * its distance from both; same for `Top`/`Bottom` along the vertical axis,
 * independently. `Left` (or `Top`) alone, or no anchor at all, does nothing on
 * either axis, same as having no layout at all, which is also the default (see
 * `Face::get_anchor()`).
 *
 * Each child's own "distance from its anchored edge(s)" is captured once, from its
 * own position/size relative to the container's size as of the first `perform()`
 * call that sees it (typically right when it's added), and kept fixed from then on;
 * explicitly repositioning/resizing it afterwards is overwritten again on this
 * layout's next perform() call, same as any other anchor-based GUI layout.
 */
class DefaultLayout
    : public Layout
{

public:
    DefaultLayout() = default;

    DefaultLayout(const DefaultLayout&) = delete;
    DefaultLayout& operator=(const DefaultLayout&) = delete;
    DefaultLayout(DefaultLayout&&) = delete;
    DefaultLayout& operator=(DefaultLayout&&) = delete;

    ~DefaultLayout() override = default;



    // #############################################################################
    // Layout Implementation
    // #############################################################################

    /**
     * (Re-)arranges container's current children within it, per the anchoring rules
     * described on this class.
     */
    void perform(Container& container) override;



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * Each currently known child's distance from the container's own
     * left/top/right/bottom edges, as of when it was first seen; see `perform()`.
     * Pruned of any child no longer actually present on every `perform()` call.
     */
    struct Insets
    {
        float left = 0.0f;
        float top = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
    }; // struct Insets

    std::unordered_map<const Face*, Insets> _insets;

}; // class DefaultLayout



} // namespace fl7::gui::layout

#endif // FL7_GUI_LAYOUT_DEFAULTLAYOUT_H
