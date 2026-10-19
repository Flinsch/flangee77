#include "ListBox.h"

#include <cassert>
#include <cstddef>



namespace fl7::gui::faces {



    // #############################################################################
    // ListBox::Panel
    // #############################################################################

    /**
     * A plain ScrollPanel themed "list_box" instead of "scroll_panel", so a list
     * box's chrome can be styled independently of a plain ScrollPanel's.
     */
    class ListBox::Panel
        : public ScrollPanel
    {
    protected:
        cl7::u8string_view _get_theme_key() const override { return u8"list_box"; }
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    ListBox::ListBox()
        : _panel(add_child<Panel>())
        , _row_area(_panel.get_content_area().add_child<RowArea>(*this))
    {
        _panel.set_horizontal_scroll_enabled(false);
        _panel.set_horizontal_scroll_bar_visibility(ScrollPanel::ScrollBarVisibility::Never);
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Sets the currently selected item by index, or -1 to select none, unless
     * already at that index. An out-of-range index (besides -1) is ignored. Emits
     * get_selection_changed() (with the new index) when actually changed.
     */
    void ListBox::set_selected_index(int index)
    {
        if (index < -1 || index >= get_item_count())
            return;

        if (index == _selected_index)
            return;

        _selected_index = index;
        _selection_changed.emit(_selected_index);
    }

    /**
     * Sets the height of a single row (must be positive), re-stacking every row to it.
     */
    void ListBox::set_row_height(float row_height)
    {
        assert(row_height > 0.0f);

        if (row_height == _row_height)
            return;

        _row_height = row_height;
        _relayout_rows();
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Keeps the internal panel's size, and every row's width, in sync with this
     * list's own size.
     */
    void ListBox::_on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size)
    {
        _panel.set_size(new_size);
        _relayout_rows();
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Re-stacks every row (position/size) and re-syncs the internal panel's content
     * size and this list's row area to its (possibly scroll-bar-narrowed) current
     * viewport width. Needed whenever this list's own size, its row height, or its
     * item count changes.
     */
    void ListBox::_relayout_rows()
    {
        const float total_height = static_cast<float>(_rows.size()) * _row_height;

        // The panel's content width is irrelevant to it (horizontal scrolling is
        // permanently disabled/hidden, see the constructor): reuses whatever the
        // content area's current (possibly stale) width happens to be, purely as
        // an inert placeholder.
        _panel.set_content_size({_panel.get_content_area().get_size().x, total_height});

        // Only now, after set_content_size() above has possibly changed the
        // vertical scroll bar's shown/hidden state (and, with it, the content
        // area's own viewport width), is that width actually final for this pass.
        const float width = _panel.get_content_area().get_size().x;

        _row_area.set_size({width, total_height});

        for (std::size_t i = 0; i < _rows.size(); ++i)
        {
            _rows[i]->set_position({0.0f, static_cast<float>(i) * _row_height});
            _rows[i]->set_size({width, _row_height});
        }
    }



} // namespace fl7::gui::faces
