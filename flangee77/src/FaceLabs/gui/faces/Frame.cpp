#include "Frame.h"

#include <algorithm>



namespace fl7::gui::faces {



    /**
     * A small, actively interactive drag grip in the frame's own bottom-right
     * corner (see `Frame::_on_size_changed()`'s own repositioning), letting the
     * user resize it. Deliberately NOT a disabled/passive decoration, unlike
     * `Frame`'s own `_title_label` (or `TrackControl`'s handle): it overlaps the
     * frame's content area, which, unlike the title bar's own otherwise-empty
     * strip, is both enabled and (being added first, in Window's own
     * constructor) further back in z-order, so it would otherwise swallow the click
     * before it ever reached Frame's own mouse hooks. Handles dragging entirely on
     * its own instead, resizing its owning frame directly. Not a Control either:
     * drag-only, no keyboard equivalent (yet), so no Tab stop.
     */
    class Frame::ResizeHandle
        : public Face
        , public HasBackground
    {

    public:
        explicit ResizeHandle(Frame& frame) : _frame(frame) {}

        ResizeHandle(const ResizeHandle&) = delete;
        ResizeHandle& operator=(const ResizeHandle&) = delete;
        ResizeHandle(ResizeHandle&&) = delete;
        ResizeHandle& operator=(ResizeHandle&&) = delete;

        ~ResizeHandle() override = default;

    protected:

        // #########################################################################
        // Face Implementations
        // #########################################################################

        /** Returns this resize handle's theme role. */
        cl7::u8string_view _get_theme_key() const override { return u8"frame_resize_handle"; }

        /** Arms dragging. */
        void _on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position) override
        {
            _dragging = true;
        }

        /** Disarms dragging. */
        void _on_mouse_up(xl7::input::MouseButton button) override
        {
            _dragging = false;
        }

        /** Resizes the owning frame by delta, clamped to Frame::MIN_WIDTH/MIN_HEIGHT, while dragging is armed. */
        void _on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position) override
        {
            if (!_dragging)
                return;

            const ml7::Vector2f size = _frame.get_size();
            _frame.set_size({
                std::max(Frame::MIN_WIDTH, size.x + delta.x),
                std::max(Frame::MIN_HEIGHT, size.y + delta.y),
            });
        }

    private:

        // #########################################################################
        // Attributes
        // #########################################################################

        Frame& _frame;

        /** Whether this handle is currently being dragged. */
        bool _dragging = false;

    }; // class Frame::ResizeHandle



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Frame::Frame(cl7::u32string title)
        : _title_label(add_child<Label>(std::move(title)))
        , _close_button(add_child<Button>(U"X"))
        , _resize_handle(add_child<ResizeHandle>(*this))
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
     * Lays out the title bar (title label and close button), the resize handle
     * (bottom-right corner), and the content area (the remainder below the title
     * bar).
     */
    void Frame::_on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size)
    {
        _close_button.set_position({std::max(0.0f, new_size.x - TITLE_BAR_HEIGHT), 0.0f});
        _close_button.set_size({TITLE_BAR_HEIGHT, TITLE_BAR_HEIGHT});

        _title_label.set_position({0.0f, 0.0f});
        _title_label.set_size({std::max(0.0f, new_size.x - TITLE_BAR_HEIGHT), TITLE_BAR_HEIGHT});

        _resize_handle.set_position({std::max(0.0f, new_size.x - RESIZE_HANDLE_SIZE), std::max(0.0f, new_size.y - RESIZE_HANDLE_SIZE)});
        _resize_handle.set_size({RESIZE_HANDLE_SIZE, RESIZE_HANDLE_SIZE});

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
    void Frame::_on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position)
    {
        if (_dragging)
            set_position(get_position() + delta);
    }



} // namespace fl7::gui::faces
