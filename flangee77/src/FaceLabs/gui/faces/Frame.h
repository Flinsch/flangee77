#ifndef FL7_GUI_FACES_FRAME_H
#define FL7_GUI_FACES_FRAME_H
#include "../Compound.h"
#include "../HasBackground.h"

#include "../Container.h"

#include "./Button.h"
#include "./Label.h"

#include <CoreLabs/behavioral/Signal.h>
#include <CoreLabs/string.h>



namespace fl7::gui::faces {



/**
 * A top-level window: a background/border chrome (see `Style`) around an internal
 * content area, exposed via `get_content_area()` (the only place a caller can
 * actually add children, since this is a `Compound`: fixed internal composition,
 * not arbitrary public children), with a fixed title bar (title text plus a close
 * button) on top of it, draggable (by that title bar) to move it and, by a small
 * grip in its bottom-right corner, to resize it (never below this frame's own
 * effective minimum size, which already aggregates the content area's own children's
 * minimum sizes recursively). The content area fills whatever is left below the
 * title bar and is kept in sync as the frame is resized.
 */
class Frame
    : public Compound
    , public HasBackground
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

    /** Returns this frame's content area: the (only) place a caller can actually add children. */
    Container& get_content_area() { return _content_area; }
    /** Returns this frame's content area: the (only) place a caller can actually add children. */
    const Container& get_content_area() const { return _content_area; }

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

    /** Returns this frame's theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"frame"; }

    /**
     * Lays out the title bar (title label and close button), the resize handle
     * (bottom-right corner), and the content area (the remainder below the title
     * bar).
     */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override;

    /**
     * Returns the content area's own effective preferred size, plus this
     * frame's own title bar height on top; width is otherwise unaffected, the
     * title bar never needs more of it than the content area itself already
     * does.
     */
    ml7::Vector2f _calculate_preferred_size() const override;

    /**
     * Returns the content area's own effective minimum size, plus this
     * frame's own title bar height on top (width otherwise unaffected: the
     * title bar never needs more of it than the content area itself already
     * does), floored to `MIN_WIDTH`/`_min_height()` either way, so this
     * frame's own chrome always has enough room regardless of how small the
     * content area's own minimum size is. What actually keeps the resize
     * handle (see `ResizeHandle`) from ever shrinking this frame below it.
     */
    ml7::Vector2f _calculate_minimum_size() const override;

    /**
     * `false`: a frame's own `Style::padding` would currently have no actual
     * effect (no text of its own, and its content area isn't inset by it the
     * way `Container`'s own children are, since this is a `Compound`, not a
     * `Container`; see `Container`'s own doc comment), so reporting a size
     * padded for it would be misleading.
     */
    bool _applies_own_padding() const override { return false; }

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
     * This frame's own effective minimum width (see `_calculate_minimum_size()`)
     * never goes below this, regardless of how small the content area's own
     * aggregated minimum width is (e.g., an empty content area's: 0): a floor
     * for the chrome itself (title bar/close button), not the content.
     */
    static constexpr float MIN_WIDTH = 80.0f;



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Returns the minimum height dragging the resize handle (see `ResizeHandle`)
     * never shrinks this frame below: tall enough for the title bar and the resize
     * handle to both still fit.
     */
    float _min_height() const;



    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning: actually owned (as a child) by the inherited `Collection`. */
    Container& _content_area;
    /** Non-owning: actually owned (as a child) by the inherited `Collection`. */
    Label& _title_label;
    /** Non-owning: actually owned (as a child) by the inherited `Collection`. */
    Button& _close_button;
    /** Non-owning: actually owned (as a child) by the inherited `Collection`. */
    ResizeHandle& _resize_handle;

    cl7::behavioral::Signal<> _opened;
    cl7::behavioral::Signal<> _closed;

    /** Whether the title bar is currently being dragged. */
    bool _dragging = false;

}; // class Frame



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_FRAME_H
