#ifndef FL7_GUI_FACES_LISTBOX_H
#define FL7_GUI_FACES_LISTBOX_H
#include "../Compound.h"
#include "../Container.h"
#include "../HasBackground.h"
#include "../HasCheckedState.h"

#include "./ScrollPanel.h"

#include <CoreLabs/behavioral/Signal.h>

#include <algorithm>
#include <concepts>
#include <vector>



namespace fl7::gui::faces {



/**
 * A scrollable, vertically stacked list of arbitrary rows (see add_item()), each
 * wrapping one caller-supplied Face, with single-select-by-click/arrow-keys built
 * in (see get_selected_index()/set_selected_index()). Internally just a themed
 * ("list_box") ScrollPanel (see Panel) with its vertical scrolling reused wholesale
 * and its horizontal scrolling permanently disabled/hidden (rows always span the
 * full available width). Each row (see Row, themed "list_box_row") is purely a
 * disabled decoration around its own content Face; hit-testing/selection/keyboard
 * navigation is instead handled by a single RowArea spanning all rows at once (see
 * RowArea), both on the content Face's behalf (clicking anywhere in a row, its
 * disabled content included, selects that row) and so the whole list has one
 * coherent focused face for arrow-key navigation, rather than each (otherwise
 * disabled) row needing to be its own focusable Control.
 *
 * No removal is supported yet (the underlying Collection itself doesn't support
 * removing a child either, see Collection).
 */
class ListBox
    : public Compound
{

public:
    ListBox();

    ListBox(const ListBox&) = delete;
    ListBox& operator=(const ListBox&) = delete;
    ListBox(ListBox&&) = delete;
    ListBox& operator=(ListBox&&) = delete;

    ~ListBox() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Constructs a new item (an arbitrary Face, owned by a new row of its own) in
     * place, adds it at the end of this list, and returns a reference to it (not to
     * the row wrapping it, which stays purely an internal implementation detail).
     * Disabled (same as the row wrapping it): clicking/hovering anywhere in the row
     * always resolves to the row as a whole, never to the item's own content
     * specifically.
     */
    template <class TFace, class... Args>
        requires(std::derived_from<TFace, Face>)
    TFace& add_item(Args&&... args);

    /**
     * Removes (and destroys, along with its content Face) the item at the
     * specified index; an out-of-range index is ignored. Every later item's own
     * index shifts down by one to match. If the removed item was the currently
     * selected one, the selection is cleared (-1) rather than jumping to whatever
     * item now happens to occupy its old position; a selected item at a later
     * index just keeps tracking the same (still selected) item, re-numbered down
     * by one. Either way, get_selection_changed() only fires if the index this
     * list reports actually changes as a result.
     */
    void remove_item(int index);

    /** Returns the number of items currently in this list. */
    int get_item_count() const { return static_cast<int>(_rows.size()); }

    /** Returns the index of the currently selected item, or -1 if none is. */
    int get_selected_index() const { return _selected_index; }

    /**
     * Sets the currently selected item by index, or -1 to select none, unless
     * already at that index. An out-of-range index (besides -1) is ignored. Emits
     * get_selection_changed() (with the new index) when actually changed.
     */
    void set_selected_index(int index);

    /** Returns the signal emitted (with the new index, or -1) whenever this list's selected item actually changes. */
    cl7::behavioral::Signal<int>& get_selection_changed() { return _selection_changed; }

    /** Returns the height of a single row. */
    float get_row_height() const { return _row_height; }

    /**
     * Sets the height of a single row (must be positive), re-stacking every row to it.
     */
    void set_row_height(float row_height);



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Keeps the internal panel's size, and every row's width, in sync with this
     * list's own size.
     */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override;



private:
    class Panel;   // A ScrollPanel themed "list_box" instead of "scroll_panel"; defined in ListBox.cpp.
    class Row;     // One selectable row, wrapping one caller-supplied item Face; defined below (needed inline here, see add_item()).
    class RowArea; // Hit-tests/selects/navigates all rows at once; defined below (needed inline here, see add_item()).



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Re-stacks every row (position/size) and re-syncs the internal panel's content
     * size and this list's row area to its (possibly scroll-bar-narrowed) current
     * viewport width. Needed whenever this list's own size, its row height, or its
     * item count changes.
     */
    void _relayout_rows();



    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Panel& _panel;
    /** Non-owning: actually owned (as a child) by the panel's content area. */
    RowArea& _row_area;

    /** Non-owning: actually owned (as children) by _row_area, in insertion order. */
    std::vector<Row*> _rows;

    int _selected_index = -1;

    float _row_height = 24.0f;

    cl7::behavioral::Signal<int> _selection_changed;

}; // class ListBox



// #############################################################################
// ListBox::Row
// #############################################################################

/**
 * A single selectable row, purely a disabled decoration (themed "list_box_row",
 * see HasCheckedState) around its own caller-supplied content Face (see
 * set_content()): never itself hit-tested, hovered, pressed, or focused (that's
 * RowArea's job, for the whole list at once; see ListBox::RowArea), only visually
 * reflects whether it's currently the owning list's selected item.
 */
class ListBox::Row
    : public Compound
    , public HasBackground
    , public HasCheckedState
{
    friend class ListBox;

public:
    Row(const ListBox& owner, int index)
        : _owner(owner)
        , _index(index)
    {
        set_enabled(false);
    }



    // #############################################################################
    // HasCheckedState Implementations
    // #############################################################################

    /** Returns whether this row is currently the owning list's selected item. */
    bool is_checked() const override { return _owner.get_selected_index() == _index; }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Constructs this row's content Face in place, adds it as this row's (only)
     * child, and returns a reference to it. Disabled, and kept filling this row's
     * entire size (see _on_size_changed()).
     */
    template <class TFace, class... Args>
        requires(std::derived_from<TFace, Face>)
    TFace& set_content(Args&&... args)
    {
        TFace& content = add_child<TFace>(std::forward<Args>(args)...);
        content.set_enabled(false);
        content.set_position({0.0f, 0.0f});
        content.set_size(get_size());
        _content = &content;
        return content;
    }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this row's theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"list_box_row"; }

    /** Keeps this row's content Face filling its entire new size. */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override
    {
        if (!_content)
            return;

        _content->set_position({0.0f, 0.0f});
        _content->set_size(new_size);
    }



private:

    // #############################################################################
    // Backend Interface (ListBox)
    // #############################################################################

    /**
     * Re-numbers this row after some earlier row was removed (see
     * ListBox::remove_item()): only the owning list ever has a reason to shift a
     * row's index like this, after the fact, behind its own back.
     */
    void _set_index(int index) { _index = index; }



    // #############################################################################
    // Attributes
    // #############################################################################

    const ListBox& _owner;
    int _index;

    /** Non-owning: actually owned (as the only child) by the inherited Collection, once set_content() is called. */
    Face* _content = nullptr;

}; // class ListBox::Row



// #############################################################################
// ListBox::RowArea
// #############################################################################

/**
 * Spans every row at once (see ListBox::_relayout_rows()): the one face that's
 * actually hit-tested/focused for the whole list, so a click anywhere in a row
 * (its disabled content Face included) selects that row, and arrow-key navigation
 * only needs this single face to hold the focus, rather than each (otherwise
 * disabled) row needing to be its own focusable Control.
 */
class ListBox::RowArea
    : public Container
{

public:
    explicit RowArea(ListBox& owner)
        : _owner(owner)
    {
    }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Selects whichever row the click's (local) vertical position falls into, if
     * any (an out-of-range index, e.g. a click below the last row, is silently
     * ignored, see set_selected_index()).
     */
    void _on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position) override
    {
        _owner.set_selected_index(static_cast<int>(local_position.y / _owner.get_row_height()));
    }

    /**
     * Up/Down move the selection by one row; Home/End jump to the first/last row.
     */
    void _on_key_down(xl7::input::Key key) override
    {
        using xl7::input::Key;

        const int count = _owner.get_item_count();
        if (count == 0)
            return;

        switch (key)
        {
        case Key::Up:
            _owner.set_selected_index(std::max(0, _owner.get_selected_index() - 1));
            break;

        case Key::Down:
            _owner.set_selected_index(std::min(count - 1, _owner.get_selected_index() + 1));
            break;

        case Key::Home:
            _owner.set_selected_index(0);
            break;

        case Key::End:
            _owner.set_selected_index(count - 1);
            break;

        default:
            break;
        }
    }



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    ListBox& _owner;

}; // class ListBox::RowArea



// #############################################################################
// ListBox (template definitions)
// #############################################################################

/**
 * Constructs a new item (an arbitrary Face, owned by a new row of its own) in
 * place, adds it at the end of this list, and returns a reference to it (not to
 * the row wrapping it, which stays purely an internal implementation detail).
 * Disabled (same as the row wrapping it): clicking/hovering anywhere in the row
 * always resolves to the row as a whole, never to the item's own content
 * specifically.
 */
template <class TFace, class... Args>
    requires(std::derived_from<TFace, Face>)
TFace& ListBox::add_item(Args&&... args)
{
    Row& row = _row_area.add_child<Row>(*this, static_cast<int>(_rows.size()));
    TFace& content = row.template set_content<TFace>(std::forward<Args>(args)...);

    _rows.push_back(&row);
    _relayout_rows();

    return content;
}



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_LISTBOX_H
