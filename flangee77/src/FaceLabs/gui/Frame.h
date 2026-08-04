#ifndef FL7_GUI_FRAME_H
#define FL7_GUI_FRAME_H
#include "./Window.h"

#include "./Button.h"
#include "./Label.h"

#include <CoreLabs/behavioral/Signal.h>
#include <CoreLabs/string.h>



namespace fl7::gui {



/**
 * A Window with a fixed title bar (title text plus a close button) on top of its
 * content area. Chrome beyond that (dragging the title bar to move the frame,
 * resize handles, ...) is left for later.
 */
class Frame
    : public Window
{

public:
    explicit Frame(cl7::u8string title = {});

    Frame(const Frame&) = delete;
    Frame& operator=(const Frame&) = delete;
    Frame(Frame&&) = delete;
    Frame& operator=(Frame&&) = delete;

    ~Frame() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this frame's title text. */
    const cl7::u8string& get_title() const { return _title_label.get_text(); }

    /** Sets this frame's title text. */
    void set_title(cl7::u8string title) { _title_label.set_text(std::move(title)); }

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
     * Lays out the title bar (title label and close button) and the content area
     * (the remainder below the title bar).
     */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override;



private:

    // #############################################################################
    // Constants
    // #############################################################################

    /**
     * Fixed for now, should eventually come from the active theme/chrome
     * instead (e.g., the 9-slice title bar chrome's own native height).
     */
    static constexpr float TITLE_BAR_HEIGHT = 24.0f;



    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Label& _title_label;
    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Button& _close_button;

    cl7::behavioral::Signal<> _opened;
    cl7::behavioral::Signal<> _closed;

}; // class Frame



} // namespace fl7::gui

#endif // FL7_GUI_FRAME_H
