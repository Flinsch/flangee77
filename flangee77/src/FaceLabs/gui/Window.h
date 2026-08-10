#ifndef FL7_GUI_WINDOW_H
#define FL7_GUI_WINDOW_H
#include "./Compound.h"
#include "./HasBackground.h"

#include "./Container.h"



namespace fl7::gui {



/**
 * A bare top-level window: a background/border chrome (see Style) around an
 * internal content area, exposed via get_content_area() rather than a direct
 * add_child() on the Window itself (Window is a Compound: fixed internal
 * composition, not arbitrary public children). The content area fills the
 * entire window and is kept in sync as the window is resized. Chrome beyond
 * the plain background (title bar, close button, resize handles, ...) is left
 * to a future Frame built on top of this.
 */
class Window
    : public Compound
    , public HasBackground
{

public:
    Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    ~Window() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this window's content area: the (only) place a caller can actually add children. */
    Container& get_content_area() { return _content_area; }
    /** Returns this window's content area: the (only) place a caller can actually add children. */
    const Container& get_content_area() const { return _content_area; }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this window's theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"window"; }

    /**
     * Keeps the content area's size in sync with this window's own size.
     */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override;



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    Container& _content_area;

}; // class Window



} // namespace fl7::gui

#endif // FL7_GUI_WINDOW_H
