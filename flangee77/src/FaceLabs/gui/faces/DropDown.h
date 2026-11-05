#ifndef FL7_GUI_FACES_DROPDOWN_H
#define FL7_GUI_FACES_DROPDOWN_H
#include "../Compound.h"
#include "../HasBackground.h"

#include <CoreLabs/behavioral/Signal.h>
#include <CoreLabs/string.h>

#include <vector>



namespace fl7::gui::faces {



class Button;  // Used as this drop-down's own trailing arrow button; defined in Button.h, not needed here.
class Label;   // Used as this drop-down's own (disabled) text child; defined in Label.h, not needed here.
class ListBox; // Used purely as a popup; defined in ListBox.h, not needed here.



/**
 * A closed, non-editable selection widget (like an HTML <select>): shows the
 * currently selected item's text plus a trailing arrow glyph, and, while clicked/
 * Enter/Space-activated open, a popup list of all items to choose from (see
 * add_item()). Unlike an editable combo box, there's no free-text entry, so the
 * selection is always exactly one of the added items, or none.
 *
 * Internally a small fixed composition (see Compound), same as ScrollBar's own
 * track + buttons: a disabled Label for the current text (so a click/hover over
 * it resolves to this DropDown as a whole, same trick as CheckBox's caption) and
 * a genuinely interactive trailing Button of its own (an arrow glyph), which
 * opens/closes the popup just like clicking the text part does, but keeps its
 * own distinct hovered/pressed look (and its own Tab stop/Enter-Space
 * activation) while actually being clicked/focused specifically.
 *
 * The popup is a separate top-level Shell face (see Collection::clips_children()'s
 * doc comment on why: it must be free to escape this DropDown's own logical
 * owner's bounds), created lazily on first open and simply hidden/shown again
 * afterwards, never destroyed again (Shell has no way to remove a top-level face
 * at all, same as, say, a Window); see Shell::open_popup()/close_popup() for how
 * its Z-order/auto-close-on-focus-elsewhere is kept in order regardless.
 * Note: _arrow_button being its own genuine Tab stop (same as any other
 * Control) means it, not just this DropDown itself, can end up focused. Only
 * while THIS DropDown itself is the focused face do Escape/Up/Down actually
 * move the popup's selection (see _on_key_down()/_consumes_arrow()); while
 * _arrow_button specifically holds focus instead, only Enter/Space (toggling
 * it via its own _on_click(), same as any Button) does anything.
 *
 * Keyboard focus never moves onto the popup itself, though: arrow keys always
 * move its selection from whichever of the two above actually holds focus
 * instead, so a stray click into the popup list (which Shell's normal mouse
 * handling cannot help but focus, see _on_popup_selection_changed()) hands
 * focus right back here afterwards, never leaving it on the popup.
 */
class DropDown
    : public Compound
    , public HasBackground
{

public:
    DropDown();

    DropDown(const DropDown&) = delete;
    DropDown& operator=(const DropDown&) = delete;
    DropDown(DropDown&&) = delete;
    DropDown& operator=(DropDown&&) = delete;

    ~DropDown() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Adds a new item at the end of this drop-down's list, with the specified text.
     */
    void add_item(cl7::u32string text);

    /** Returns the number of items currently in this drop-down's list. */
    int get_item_count() const { return static_cast<int>(_items.size()); }

    /** Returns the index of the currently selected item, or -1 if none is. */
    int get_selected_index() const { return _selected_index; }

    /**
     * Sets the currently selected item by index, or -1 to select none, unless
     * already at that index. An out-of-range index (besides -1) is ignored. Emits
     * get_selection_changed() (with the new index) when actually changed.
     */
    void set_selected_index(int index);

    /** Returns the signal emitted (with the new index, or -1) whenever this drop-down's selected item actually changes. */
    cl7::behavioral::Signal<int>& get_selection_changed() { return _selection_changed; }

    /**
     * Returns the currently selected item's text, or an empty string if none is
     * selected.
     */
    const cl7::u32string& get_display_text() const;



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this drop-down's theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"drop_down"; }

    /**
     * Keeps the label/arrow layout in sync with this drop-down's own size.
     */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override;

    /**
     * Toggles the popup list open/closed. Also what Enter/Space triggers while this
     * drop-down is focused.
     */
    void _on_click(xl7::input::MouseButton button) override;

    /**
     * Closes the popup list (if open) without changing the selection, on Escape.
     * Otherwise, while the popup is open, moves the selection by one with Up/Down.
     */
    void _on_key_down(xl7::input::Key key) override;

    /** A DropDown as a whole is an interactive leaf control, so it's a Tab stop (Compound itself doesn't opt into that by default). */
    bool _is_focusable() const override { return true; }

    /** A DropDown's _on_click() meaningfully does something, so Enter/Space (or a gamepad's "confirm" button) can activate it while focused. */
    bool _is_clickable() const override { return true; }

    /**
     * Consumes Up/Down (hard) while the popup is open and the selection still has
     * somewhere to go in that direction; always free otherwise.
     */
    ArrowResponse _consumes_arrow(ArrowDirection dir) const override;



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Keeps _label's text in sync with get_display_text(), whenever the selection
     * changes.
     */
    void _sync_label();

    /**
     * Opens the popup if closed, or closes it if open; what both a click on the
     * text part and a click on _arrow_button actually trigger.
     */
    void _toggle_popup();

    /**
     * Returns whether the popup list is currently open (created and visible).
     */
    bool _is_popup_open() const;

    /**
     * Opens the popup list (creating it first if needed), positioned right below
     * this drop-down, unless there are no items at all.
     */
    void _open_popup();

    /**
     * Closes the popup list, if it's actually open (a no-op otherwise).
     */
    void _close_popup();

    /**
     * Creates the popup list (a separate top-level Shell face, see the class doc
     * comment), populated from _items, unless it already exists.
     */
    void _ensure_popup();

    /**
     * Reacts to the popup list's own get_selection_changed(): adopts the new
     * index, closes the popup, and hands focus back to this DropDown (clicking a
     * popup row unavoidably focuses the popup list itself first instead, see
     * Shell::update()'s mouse handling), unless this change was actually just this
     * DropDown syncing the popup's own selection to its own (see
     * set_selected_index()/_open_popup()), not an actual row click, in which case
     * none of that applies (and nothing is emitted a second time either).
     */
    void _on_popup_selection_changed(int index);



    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Label& _label;
    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Button& _arrow_button;

    std::vector<cl7::u32string> _items;

    int _selected_index = -1;

    /** Non-owning: owned by the Shell as a separate top-level face, see _ensure_popup(). */
    ListBox* _popup_list = nullptr;

    /** Suppresses _on_popup_selection_changed()'s row-click side effects while this DropDown itself is the one pushing _selected_index onto _popup_list. */
    bool _syncing_popup_selection = false;

    cl7::behavioral::Signal<int> _selection_changed;

}; // class DropDown



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_DROPDOWN_H
