#ifndef FL7_GUI_CONTAINER_H
#define FL7_GUI_CONTAINER_H
#include "./Collection.h"

#include "./layout/Layout.h"

#include <concepts>
#include <memory>



namespace fl7::gui {



/**
 * A Collection with arbitrary, publicly addable children (e.g., `Panel`,
 * `GroupBox`), optionally arranged by a pluggable layout (see
 * `layout::Layout/set_layout()`). With none attached (the default), children just
 * keep whatever position/size they're explicitly given, shifted inward by this
 * container's own effective padding (see `get_scroll_offset()`). Once a layout is
 * attached, that padding shift no longer applies here (the layout itself would have
 * to account for it; none currently do, `layout::BoxLayout` has its own, separate
 * padding concept instead).
 */
class Container
    : public Collection
{

public:
    Container() = default;

    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;
    Container(Container&&) = delete;
    Container& operator=(Container&&) = delete;

    ~Container() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this container's layout, or nullptr if none is set; see set_layout(). */
    layout::Layout* get_layout() const { return _layout.get(); }

    /**
     * Constructs a new layout of the specified type in place, replacing any
     * previously set one, and performs it once immediately against this
     * container's current children. Thereafter kept in sync automatically:
     * see add_child()/remove_child() below and _on_size_changed().
     */
    template <class TLayout, class... Args>
        requires(std::derived_from<TLayout, layout::Layout>)
    TLayout& set_layout(Args&&... args)
    {
        auto new_layout = std::make_unique<TLayout>(std::forward<Args>(args)...);
        TLayout& ref = *new_layout;
        _layout = std::move(new_layout);
        _layout->perform(*this);
        return ref;
    }

    /**
     * Clears a previously set layout (see set_layout()): children keep
     * whatever position/size they currently have, unmanaged from here on.
     */
    void clear_layout() { _layout.reset(); }



    // #############################################################################
    // Collection Implementations
    // #############################################################################

    /**
     * While no layout is attached (see `get_layout()`), shifts every child inward
     * by this container's own effective padding's left/top (see
     * `get_effective_padding()`): a child's own `get_position()` at `{0, 0}` then
     * actually renders/is hit-tested at `{padding.left, padding.top}`, same as any
     * other `Collection::get_scroll_offset()` shift. `{0, 0}` (no shift) instead
     * once a layout is attached: see this class's own doc comment.
     */
    ml7::Vector2f get_scroll_offset() const override;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Constructs a new child Face of the specified type in place, adds it,
     * re-performs this container's layout (if any; see set_layout()), and
     * returns a reference to the new child.
     */
    template <class TFace, class... Args>
        requires(std::derived_from<TFace, Face>)
    TFace& add_child(Args&&... args)
    {
        TFace& ref = Collection::add_child<TFace>(std::forward<Args>(args)...);
        if (_layout)
            _layout->perform(*this);
        return ref;
    }

    /**
     * Removes the specified child (see Collection::remove_child()), then
     * re-performs this container's layout (if any; see set_layout()).
     */
    void remove_child(Face& child)
    {
        Collection::remove_child(child);
        if (_layout)
            _layout->perform(*this);
    }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Re-performs this container's layout (if any; see set_layout()). */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override
    {
        if (_layout)
            _layout->perform(*this);
    }

    /**
     * With no layout set (children just keep whatever position/size they're
     * explicitly given; see this class's own doc comment), returns the tightest
     * bounding box enclosing every child's own `get_position()` plus its own
     * `get_effective_preferred_size()`, recursively (a child's own effective
     * preferred size may itself come from this same calculation, if it's a
     * `Container` too). With a layout set, defers to the inherited default
     * (`{0, 0}`): a layout has no way yet to report what size it would actually
     * want, so there's nothing meaningful to calculate here instead.
     */
    ml7::Vector2f _calculate_preferred_size() const override;

    /**
     * With no layout set (children just keep whatever position/size they're
     * explicitly given; see this class's own doc comment), returns the tightest
     * bounding box enclosing every child's own `get_position()` plus its own
     * `get_effective_minimum_size()`, recursively (a child's own effective
     * minimum size may itself come from this same calculation, if it's a
     * `Container` too). With a layout set, defers to the inherited default
     * (`{0, 0}`): a layout has no way yet to report what size it would actually
     * want, so there's nothing meaningful to calculate here instead.
     */
    ml7::Vector2f _calculate_minimum_size() const override;



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    std::unique_ptr<layout::Layout> _layout;

}; // class Container



} // namespace fl7::gui

#endif // FL7_GUI_CONTAINER_H
