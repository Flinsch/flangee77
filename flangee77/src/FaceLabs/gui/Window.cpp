#include "Window.h"



namespace fl7::gui {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Window::Window()
        : _content_area(add_child<Container>())
    {
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Keeps the content area's size in sync with this window's own size.
     */
    void Window::_on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size)
    {
        _content_area.set_size(new_size);
    }



} // namespace fl7::gui