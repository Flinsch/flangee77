#ifndef FL7_GUI_FACES_FRAME_H
#define FL7_GUI_FACES_FRAME_H
#include "./Window.h"

#include "./Button.h"
#include "./Label.h"

#include <CoreLabs/behavioral/Signal.h>
#include <CoreLabs/string.h>



namespace fl7::gui::faces {



/**
 * A Window with a fixed title bar (title text plus a close button) on top of its
 * content area, draggable (by that title bar) to move it and, by a small grip in
 * its bottom-right corner, to resize it (clamped to a sane minimum size; see
 * `MIN_WIDTH`/`MIN_HEIGHT`).
 */
class Frame
    : public Window
{

public:
    explicit Frame(cl7::u32string title = {});

    Frame(const Frame&) = delete;
    Frame& operator=(const Frame&) = delete;
    Frame(Frame&&) = delete;
    Frame& operator=(Frame&&) = delete;

    ~Frame() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this frame's title text. */
    const cl7::u32string& get_title() const { return _title_label.get_text(); }

    /** Sets this frame's title text. */
    void set_title(cl7::u32string title) { _title_label.set_text(std::move(title)); }

    /** Returns the signal emitted whenever this frame is opened (see `open()`). */
    cl7::behavioral::Signal<>& get_opened() { return _opened; }

    /** Returns the signal emitted whenever this frame is closed (see `close()`). */
    cl7::behavioral::Signal<>& get_closed() { return _closed; }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Opens this frame (makes it visible again), unless already open. Emits the
     * "opened" signal when actually opened.
     */
    void open();

    /**
     * Closes this frame (hides it), unless already closed. Emits the "closed"
     * signal when actually closed.
     */
    void close();



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Lays out the title bar (title label and close button), the resize handle
     * (bottom-right corner), and the content area (the remainder below the title
     * bar).
     */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override;

    /**
     * Arms dragging if the press landed within the title bar strip.
     */
    void _on_mouse_down(xl7::input::MouseButton button, ml7::Vector2f local_position) override;

    /**
     * Disarms dragging.
     */
    void _on_mouse_up(xl7::input::MouseButton button) override;

    /**
     * Moves this frame by delta, while dragging is armed.
     */
    void _on_mouse_drag(ml7::Vector2f delta, ml7::Vector2f local_position) override;



private:
    class ResizeHandle; // Face + HasBackground only, defined in Frame.cpp; see its own doc comment.

    // #############################################################################
    // Constants
    // #############################################################################

    /**
     * Fixed for now, should eventually come from the active theme/chrome
     * instead (e.g., the 9-slice title bar chrome's own native height).
     */
    static constexpr float TITLE_BAR_HEIGHT = 24.0f;

    /** Fixed for now, same caveat as TITLE_BAR_HEIGHT. */
    static constexpr float RESIZE_HANDLE_SIZE = 16.0f;

    /** Dragging the resize handle (see ResizeHandle) never shrinks this frame below this size. */
    static constexpr float MIN_WIDTH = 80.0f;
    /** See MIN_WIDTH; tall enough for the title bar and the resize handle to both still fit. */
    static constexpr float MIN_HEIGHT = TITLE_BAR_HEIGHT + RESIZE_HANDLE_SIZE;



    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Label& _title_label;
    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Button& _close_button;
    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    ResizeHandle& _resize_handle;

    cl7::behavioral::Signal<> _opened;
    cl7::behavioral::Signal<> _closed;

    /** Whether the title bar is currently being dragged. */
    bool _dragging = false;

}; // class Frame



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_FRAME_H
