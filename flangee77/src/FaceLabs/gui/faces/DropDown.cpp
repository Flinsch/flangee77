#include "DropDown.h"

#include "./Button.h"
#include "./Label.h"
#include "./ListBox.h"

#include "../Shell.h"

#include <algorithm>



namespace fl7::gui::faces {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    DropDown::DropDown()
        : _label(add_child<Label>())
        , _arrow_button(add_child<Button>(U"▼")) // down-pointing triangle
    {
        _label.set_enabled(false);
        _arrow_button.get_clicked().connect([this]() { _toggle_popup(); });
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Adds a new item at the end of this drop-down's list, with the specified text.
     */
    void DropDown::add_item(cl7::u32string text)
    {
        _items.push_back(text);
        if (_popup_list)
            _popup_list->add_item<Label>(std::move(text));
    }

    /**
     * Sets the currently selected item by index, or -1 to select none, unless
     * already at that index. An out-of-range index (besides -1) is ignored. Emits
     * get_selection_changed() (with the new index) when actually changed.
     */
    void DropDown::set_selected_index(int index)
    {
        if (index < -1 || index >= static_cast<int>(_items.size()) || index == _selected_index)
            return;

        _selected_index = index;
        _sync_label();

        // Keeps the popup's own highlighted row in sync, so it shows the right
        // one next time it's opened, even if it was never opened at all yet (in
        // which case this is simply skipped, see _open_popup() instead). The sync
        // flag keeps this from being mistaken for an actual row click.
        if (_popup_list)
        {
            _syncing_popup_selection = true;
            _popup_list->set_selected_index(index);
            _syncing_popup_selection = false;
        }

        _selection_changed.emit(index);
    }

    /**
     * Returns the currently selected item's text, or an empty string if none is
     * selected.
     */
    const cl7::u32string& DropDown::get_display_text() const
    {
        static const cl7::u32string empty;
        return (_selected_index >= 0 && _selected_index < static_cast<int>(_items.size())) ? _items[_selected_index] : empty;
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Keeps the label/arrow layout in sync with this drop-down's own size.
     */
    void DropDown::_on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size)
    {
        const float arrow_side = std::min(new_size.x, new_size.y);

        _label.set_position({0.0f, 0.0f});
        _label.set_size({std::max(0.0f, new_size.x - arrow_side), new_size.y});

        _arrow_button.set_position({new_size.x - arrow_side, 0.0f});
        _arrow_button.set_size({arrow_side, new_size.y});
    }

    /**
     * Toggles the popup list open/closed. Also what Enter/Space triggers while this
     * drop-down is focused.
     */
    void DropDown::_on_click(xl7::input::MouseButton button)
    {
        _toggle_popup();
    }

    /**
     * Closes the popup list (if open) without changing the selection, on Escape.
     * Otherwise, while the popup is open, moves the selection by one with Up/Down.
     */
    void DropDown::_on_key_down(xl7::input::Key key)
    {
        using xl7::input::Key;

        if (key == Key::Escape)
        {
            _close_popup();
            return;
        }

        if (!_is_popup_open())
            return;

        if (key == Key::Up && _selected_index > 0)
            set_selected_index(_selected_index - 1);
        else if (key == Key::Down && _selected_index + 1 < static_cast<int>(_items.size()))
            set_selected_index(_selected_index + 1);
    }

    /**
     * Consumes Up/Down (hard) while the popup is open and the selection still has
     * somewhere to go in that direction; always free otherwise.
     */
    ArrowResponse DropDown::_consumes_arrow(ArrowDirection dir) const
    {
        if (!_is_popup_open())
            return ArrowResponse::Ignore;

        if (dir == ArrowDirection::Up && _selected_index > 0)
            return ArrowResponse::Hard;
        if (dir == ArrowDirection::Down && _selected_index + 1 < static_cast<int>(_items.size()))
            return ArrowResponse::Hard;

        return ArrowResponse::Ignore;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Keeps _label's text in sync with get_display_text(), whenever the selection
     * changes.
     */
    void DropDown::_sync_label()
    {
        _label.set_text(get_display_text());
    }

    /**
     * Opens the popup if closed, or closes it if open; what both a click on the
     * text part and a click on _arrow_button actually trigger.
     */
    void DropDown::_toggle_popup()
    {
        if (_is_popup_open())
            _close_popup();
        else
            _open_popup();
    }

    /**
     * Returns whether the popup list is currently open (created and visible).
     */
    bool DropDown::_is_popup_open() const
    {
        return _popup_list && _popup_list->is_visible();
    }

    /**
     * Opens the popup list (creating it first if needed), positioned right below
     * this drop-down, unless there are no items at all.
     */
    void DropDown::_open_popup()
    {
        if (_items.empty())
            return;

        _ensure_popup();

        if (_popup_list->get_selected_index() != _selected_index)
        {
            _syncing_popup_selection = true;
            _popup_list->set_selected_index(_selected_index);
            _syncing_popup_selection = false;
        }

        constexpr int max_visible_items = 8;
        const int visible_items = std::min(static_cast<int>(_items.size()), max_visible_items);

        _popup_list->set_position(get_absolute_position() + ml7::Vector2f{0.0f, get_size().y});
        _popup_list->set_size({get_size().x, static_cast<float>(visible_items) * _popup_list->get_row_height()});
        _popup_list->set_visible(true);

        // Keeps the popup raised to the front of the Z-order (not just the first
        // time it's ever created, see Shell::open_popup()), and has the shell
        // close it again on its own once focus moves to anything that's neither
        // it nor this DropDown.
        if (Shell* const shell = get_shell())
            shell->open_popup(_popup_list, this);
    }

    /**
     * Closes the popup list, if it's actually open (a no-op otherwise).
     */
    void DropDown::_close_popup()
    {
        if (!_is_popup_open())
            return;

        if (Shell* const shell = get_shell())
            shell->close_popup(_popup_list);
        else
            _popup_list->set_visible(false);
    }

    /**
     * Creates the popup list (a separate top-level Shell face, see the class doc
     * comment), populated from _items, unless it already exists.
     */
    void DropDown::_ensure_popup()
    {
        if (_popup_list)
            return;

        Shell* const shell = get_shell();
        assert(shell);

        ListBox& popup = shell->add_face<ListBox>();
        popup.set_visible(false);
        for (const cl7::u32string& item : _items)
            popup.add_item<Label>(item);

        // This DropDown's own lifetime is NOT tied to the popup's: it's a separate
        // top-level Shell face (see the class doc comment), and Shell has no way to
        // ever remove a top-level face again (same as, say, a Window). If this
        // DropDown is destroyed first, this popup, and this connection's `this`
        // capture with it, just stays around, permanently hidden and unreachable,
        // for the shell's remaining lifetime: a known, pre-existing limitation of
        // Shell itself, not something new here.
        popup.get_selection_changed().connect([this](int index) { _on_popup_selection_changed(index); });

        _popup_list = &popup;
    }

    /**
     * Reacts to the popup list's own get_selection_changed(): adopts the new
     * index, closes the popup, and hands focus back to this DropDown (clicking a
     * popup row unavoidably focuses the popup list itself first instead, see
     * Shell::update()'s mouse handling), unless this change was actually just this
     * DropDown syncing the popup's own selection to its own (see
     * set_selected_index()/_open_popup()), not an actual row click, in which case
     * none of that applies (and nothing is emitted a second time either).
     */
    void DropDown::_on_popup_selection_changed(int index)
    {
        if (_syncing_popup_selection)
            return;

        _selected_index = index;
        _sync_label();
        _close_popup();

        if (Shell* const shell = get_shell())
            shell->set_focus(this);

        _selection_changed.emit(index);
    }



} // namespace fl7::gui::faces
