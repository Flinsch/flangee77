#include "Frame.h"

#include <algorithm>



namespace fl7::gui {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Frame::Frame(cl7::u8string title)
        : _title_label(add_child<Label>(std::move(title)))
        , _close_button(add_child<Button>(u8"X"))
    {
        _close_button.get_clicked().connect([this]() { close(); });
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Opens this frame (makes it visible again), unless already open. Emits the
     * "opened" signal when actually opened.
     */
    void Frame::open()
    {
        if (is_visible())
            return;

        set_visible(true);
        _opened.emit();
    }

    /**
     * Closes this frame (hides it), unless already closed. Emits the "closed"
     * signal when actually closed.
     */
    void Frame::close()
    {
        if (!is_visible())
            return;

        set_visible(false);
        _closed.emit();
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Lays out the title bar (title label and close button) and the content area
     * (the remainder below the title bar).
     */
    void Frame::_on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size)
    {
        _close_button.set_position({std::max(0.0f, new_size.x - TITLE_BAR_HEIGHT), 0.0f});
        _close_button.set_size({TITLE_BAR_HEIGHT, TITLE_BAR_HEIGHT});

        _title_label.set_position({0.0f, 0.0f});
        _title_label.set_size({std::max(0.0f, new_size.x - TITLE_BAR_HEIGHT), TITLE_BAR_HEIGHT});

        get_content_area().set_position({0.0f, TITLE_BAR_HEIGHT});
        get_content_area().set_size({new_size.x, std::max(0.0f, new_size.y - TITLE_BAR_HEIGHT)});
    }



} // namespace fl7::gui
