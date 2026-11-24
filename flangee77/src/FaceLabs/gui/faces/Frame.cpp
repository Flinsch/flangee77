#include "Frame.h"

#include <algorithm>



namespace fl7::gui::faces {



    /**
     * A small, actively interactive drag grip in the frame's own bottom-right
     * corner (see `Frame::_on_size_changed()`'s own repositioning), letting the
     * user resize it. Deliberately NOT a disabled/passive decoration, unlike
     * `Frame`'s own `_title_label` (or `TrackControl`'s handle): it overlaps the
     * frame's content area, which, unlike the title bar's own otherwise-empty
     * strip, is both enabled and (being added first, in `Frame`'s own
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

        /** Arms dragging, seeding `_drag_size` from the frame's current size. */
        void _on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position) override
        {
            _dragging = true;
            _drag_size = _frame.get_size();
        }

        /** Disarms dragging. */
        void _on_mouse_up(xl7::input::MouseButton button) override
        {
            _dragging = false;
        }

        /**
         * Resizes the owning frame by delta, clamped to its own effective
         * minimum size (see `Frame::_calculate_minimum_size()`, which already
         * aggregates the content area's own minimum size, recursively, on top
         * of `Frame::MIN_WIDTH`/`_min_height()`), while dragging is armed; see
         * `_drag_size`.
         */
        void _on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position) override
        {
            if (!_dragging)
                return;

            _drag_size += delta;
            const ml7::Vector2f minimum_size = _frame.get_effective_minimum_size();
            _frame.set_size({
                std::max(minimum_size.x, _drag_size.x),
                std::max(minimum_size.y, _drag_size.y),
            });
        }

    private:

        // #########################################################################
        // Attributes
        // #########################################################################

        Frame& _frame;

        /** Whether this handle is currently being dragged. */
        bool _dragging = false;

        /**
         * This drag gesture's own running size, accumulating every `delta` since
         * `_on_mouse_down()` unclamped, unlike the frame's own (always clamped to
         * `Frame::MIN_WIDTH`/`_min_height()`) actual size: dragging past the
         * minimum and back again this way lines the handle back up with the
         * cursor right where it left off, instead of the cursor and the frame's
         * actual size drifting apart for the rest of the gesture.
         */
        ml7::Vector2f _drag_size;

    }; // class Frame::ResizeHandle



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Frame::Frame(cl7::u32string title)
        : _content_area(add_child<Container>())
        , _title_label(add_child<Label>(std::move(title)))
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
        const Style style = get_effective_style();
        const float title_bar_height = style.title_bar_height;
        const float resize_handle_size = style.resize_handle_size;

        _close_button.set_position({std::max(0.0f, new_size.x - title_bar_height), 0.0f});
        _close_button.set_size({title_bar_height, title_bar_height});

        _title_label.set_position({0.0f, 0.0f});
        _title_label.set_size({std::max(0.0f, new_size.x - title_bar_height), title_bar_height});

        _resize_handle.set_position({
            std::max(0.0f, new_size.x - resize_handle_size),
            std::max(0.0f, new_size.y - resize_handle_size),
        });
        _resize_handle.set_size({resize_handle_size, resize_handle_size});

        _content_area.set_position({0.0f, title_bar_height});
        _content_area.set_size({new_size.x, std::max(0.0f, new_size.y - title_bar_height)});
    }

    /**
     * Returns the content area's own effective preferred size, plus this
     * frame's own title bar height on top; width is otherwise unaffected, the
     * title bar never needs more of it than the content area itself already
     * does.
     */
    ml7::Vector2f Frame::_calculate_preferred_size() const
    {
        return _content_area.get_effective_preferred_size()
            + ml7::Vector2f{0.0f, get_effective_style().title_bar_height};
    }

    /**
     * Returns the content area's own effective minimum size, plus this
     * frame's own title bar height on top (width otherwise unaffected: the
     * title bar never needs more of it than the content area itself already
     * does), floored to `MIN_WIDTH`/`_min_height()` either way, so this
     * frame's own chrome always has enough room regardless of how small the
     * content area's own minimum size is. What actually keeps the resize
     * handle (see `ResizeHandle`) from ever shrinking this frame below it.
     */
    ml7::Vector2f Frame::_calculate_minimum_size() const
    {
        const ml7::Vector2f content_minimum = _content_area.get_effective_minimum_size()
            + ml7::Vector2f{0.0f, get_effective_style().title_bar_height};
        return {std::max(MIN_WIDTH, content_minimum.x), std::max(_min_height(), content_minimum.y)};
    }

    /**
     * Arms dragging if the press landed within the title bar strip.
     */
    void Frame::_on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position)
    {
        _dragging = local_position.y < get_effective_style().title_bar_height;
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



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Returns the minimum height dragging the resize handle (see `ResizeHandle`)
     * never shrinks this frame below: tall enough for the title bar and the resize
     * handle to both still fit.
     */
    float Frame::_min_height() const
    {
        const Style style = get_effective_style();
        return style.title_bar_height + style.resize_handle_size;
    }



} // namespace fl7::gui::faces
