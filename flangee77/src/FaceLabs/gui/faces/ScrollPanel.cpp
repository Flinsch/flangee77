#include "ScrollPanel.h"

#include <algorithm>



namespace fl7::gui::faces {



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Sets this panel's content size. Re-clamps the current scroll offset to it.
     */
    void ScrollPanel::set_content_size(ml7::Vector2f content_size)
    {
        _content_size = content_size;
        _scroll_offset = _clamp_scroll_offset(_scroll_offset);
    }

    /**
     * Sets this panel's scroll offset, clamped to [0, get_content_size() - get_size()] per axis (never negative).
     */
    void ScrollPanel::set_scroll_offset(ml7::Vector2f scroll_offset)
    {
        _scroll_offset = _clamp_scroll_offset(scroll_offset);
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Re-clamps the current scroll offset whenever this panel's own (viewport) size changes.
     */
    void ScrollPanel::_on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size)
    {
        _scroll_offset = _clamp_scroll_offset(_scroll_offset);
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Clamps a scroll offset to [0, get_content_size() - get_size()] per axis (never negative).
     */
    ml7::Vector2f ScrollPanel::_clamp_scroll_offset(ml7::Vector2f scroll_offset) const
    {
        const ml7::Vector2f max_offset{
            std::max(0.0f, _content_size.x - get_size().x),
            std::max(0.0f, _content_size.y - get_size().y),
        };

        return {
            std::clamp(scroll_offset.x, 0.0f, max_offset.x),
            std::clamp(scroll_offset.y, 0.0f, max_offset.y),
        };
    }



} // namespace fl7::gui::faces
