#include "Window.h"



namespace fl7::gui::faces {



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

    /**
     * Returns this window's content area's own effective preferred size: a window
     * has no content of its own beyond that (the content area fills it entirely).
     */
    ml7::Vector2f Window::_calculate_preferred_size() const
    {
        return get_content_area().get_effective_preferred_size();
    }

    /**
     * Returns this window's content area's own effective minimum size: a window
     * has no content of its own beyond that (the content area fills it entirely).
     */
    ml7::Vector2f Window::_calculate_minimum_size() const
    {
        return get_content_area().get_effective_minimum_size();
    }



} // namespace fl7::gui::faces
