#include "ScrollPanel.h"

#include <algorithm>



namespace fl7::gui::faces {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    ScrollPanel::ScrollPanel()
        : _content_area(add_child<ContentArea>())
        , _vertical_scroll_bar(add_child<ScrollBar>(ScrollBar::Orientation::Vertical))
        , _horizontal_scroll_bar(add_child<ScrollBar>(ScrollBar::Orientation::Horizontal))
    {
        _vertical_scroll_bar.get_changed().connect([this](float value) {
            _apply_scroll_offset({_scroll_offset.x, value});
        });
        _horizontal_scroll_bar.get_changed().connect([this](float value) {
            _apply_scroll_offset({value, _scroll_offset.y});
        });
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Sets this panel's content size, re-clamping (and, if that actually changes it, see get_scroll_changed()) the current scroll position to it.
     */
    void ScrollPanel::set_content_size(ml7::Vector2f content_size)
    {
        if (content_size == _content_size)
            return;

        _content_size = content_size;
        _update_scroll_bars();
        _apply_scroll_offset(_scroll_offset);
    }

    /**
     * Sets this panel's scroll position, clamped to [0, get_content_size() - (viewport size)] per axis (never negative), unless already at that (clamped) position.
     */
    void ScrollPanel::set_scroll_position(ml7::Vector2f scroll_position)
    {
        _apply_scroll_offset(scroll_position);
    }

    /**
     * Sets the amount the mouse wheel scrolls (vertically) per notch; also both scroll bars' own step.
     */
    void ScrollPanel::set_wheel_step(float wheel_step)
    {
        _wheel_step = wheel_step;
        _vertical_scroll_bar.set_step(_wheel_step);
        _horizontal_scroll_bar.set_step(_wheel_step);
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Lays out the content area and both scroll bars for this panel's own new size.
     */
    void ScrollPanel::_on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size)
    {
        _layout();
        _update_scroll_bars();
        _apply_scroll_offset(_scroll_offset);
    }

    /**
     * Scrolls vertically by get_wheel_step() per wheel notch. Always consumes.
     */
    bool ScrollPanel::_on_mouse_wheel(int delta)
    {
        // Wheel rotated away from the user (a positive delta/notch, scrolling
        // "up") moves the content up, i.e., decreases the offset.
        _apply_scroll_offset(_scroll_offset - ml7::Vector2f{0.0f, static_cast<float>(delta) * _wheel_step});
        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Positions/sizes the content area and both scroll bars for this panel's own current size.
     */
    void ScrollPanel::_layout()
    {
        const ml7::Vector2f size = get_size();

        const ml7::Vector2f viewport_size{
            std::max(0.0f, size.x - SCROLLBAR_THICKNESS),
            std::max(0.0f, size.y - SCROLLBAR_THICKNESS),
        };

        _content_area.set_position({0.0f, 0.0f});
        _content_area.set_size(viewport_size);

        _vertical_scroll_bar.set_position({viewport_size.x, 0.0f});
        _vertical_scroll_bar.set_size({SCROLLBAR_THICKNESS, viewport_size.y});

        _horizontal_scroll_bar.set_position({0.0f, viewport_size.y});
        _horizontal_scroll_bar.set_size({viewport_size.x, SCROLLBAR_THICKNESS});
    }

    /**
     * Refreshes both scroll bars' range/visible_fraction/step from the current content size vs. the content area's (viewport) size.
     */
    void ScrollPanel::_update_scroll_bars()
    {
        const ml7::Vector2f viewport_size = _content_area.get_size();

        _vertical_scroll_bar.set_range(0.0f, std::max(0.0f, _content_size.y - viewport_size.y));
        _vertical_scroll_bar.set_visible_fraction(_content_size.y > 0.0f ? viewport_size.y / _content_size.y : 1.0f);
        _vertical_scroll_bar.set_step(_wheel_step);

        _horizontal_scroll_bar.set_range(0.0f, std::max(0.0f, _content_size.x - viewport_size.x));
        _horizontal_scroll_bar.set_visible_fraction(_content_size.x > 0.0f ? viewport_size.x / _content_size.x : 1.0f);
        _horizontal_scroll_bar.set_step(_wheel_step);
    }

    /**
     * Clamps a scroll position to [0, get_content_size() - (viewport size)] per axis (never negative).
     */
    ml7::Vector2f ScrollPanel::_clamp_scroll_offset(ml7::Vector2f scroll_offset) const
    {
        const ml7::Vector2f viewport_size = _content_area.get_size();

        const ml7::Vector2f max_offset{
            std::max(0.0f, _content_size.x - viewport_size.x),
            std::max(0.0f, _content_size.y - viewport_size.y),
        };

        return {
            std::clamp(scroll_offset.x, 0.0f, max_offset.x),
            std::clamp(scroll_offset.y, 0.0f, max_offset.y),
        };
    }

    /**
     * Clamps and applies a new scroll position: syncs the content area and both scroll bars, emitting get_scroll_changed() only if it actually changed.
     */
    void ScrollPanel::_apply_scroll_offset(ml7::Vector2f scroll_offset)
    {
        scroll_offset = _clamp_scroll_offset(scroll_offset);
        if (scroll_offset == _scroll_offset)
            return;

        _scroll_offset = scroll_offset;
        _content_area._set_scroll_offset(_scroll_offset);
        _vertical_scroll_bar.set_value(_scroll_offset.y);
        _horizontal_scroll_bar.set_value(_scroll_offset.x);
        _scroll_changed.emit(_scroll_offset);
    }



} // namespace fl7::gui::faces
