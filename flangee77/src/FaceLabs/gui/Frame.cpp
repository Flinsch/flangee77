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
        // Purely decorative: disabled so title bar clicks (including on the
        // text itself) fall through to this frame's own mouse-down/drag
        // handling instead of being swallowed by the label.
        _title_label.set_enabled(false);

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

    /**
     * Arms dragging if the press landed within the title bar strip.
     */
    void Frame::_on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position)
    {
        _dragging = local_position.y < TITLE_BAR_HEIGHT;
    }

    /**
     * Disarms dragging.
     */
    void Frame::_on_mouse_up(xl7::input::MouseButton button)
    {
        _dragging = false;
    }

    /**
     * Moves this frame by delta, while dragging is armed.
     */
    void Frame::_on_mouse_drag(ml7::Vector2f delta)
    {
        if (_dragging)
            set_position(get_position() + delta);
    }



} // namespace fl7::gui
