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
        _relayout();
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

    /**
     * Sets whether this panel's content can be scrolled vertically at all.
     * While disabled, the vertical scroll position stays clamped to 0 regardless of
     * content size, and the vertical scroll bar is disabled (however its own
     * visibility is configured; see set_vertical_scroll_bar_visibility()), since
     * there would be nothing for it to do.
     */
    void ScrollPanel::set_vertical_scroll_enabled(bool enabled)
    {
        if (enabled == _vertical_scroll_enabled)
            return;

        _vertical_scroll_enabled = enabled;
        _relayout();
    }

    /**
     * Sets whether this panel's content can be scrolled horizontally at all.
     * While disabled, the horizontal scroll position stays clamped to 0 regardless of
     * content size, and the horizontal scroll bar is disabled (however its own
     * visibility is configured; see set_horizontal_scroll_bar_visibility()), since
     * there would be nothing for it to do.
     */
    void ScrollPanel::set_horizontal_scroll_enabled(bool enabled)
    {
        if (enabled == _horizontal_scroll_enabled)
            return;

        _horizontal_scroll_enabled = enabled;
        _relayout();
    }

    /**
     * Sets this panel's vertical scroll bar's visibility mode. Auto shows it only
     * while the content actually overflows vertically, accounting for whichever
     * other scroll bar ends up shown too, since showing one reduces the viewport
     * (and therefore how much room the other axis's content actually has) available
     * to the other axis.
     */
    void ScrollPanel::set_vertical_scroll_bar_visibility(ScrollBarVisibility visibility)
    {
        if (visibility == _vertical_scroll_bar_visibility)
            return;

        _vertical_scroll_bar_visibility = visibility;
        _relayout();
    }

    /**
     * Sets this panel's horizontal scroll bar's visibility mode. Auto shows it only
     * while the content actually overflows horizontally, accounting for whichever
     * other scroll bar ends up shown too, since showing one reduces the viewport
     * (and therefore how much room the other axis's content actually has) available
     * to the other axis.
     */
    void ScrollPanel::set_horizontal_scroll_bar_visibility(ScrollBarVisibility visibility)
    {
        if (visibility == _horizontal_scroll_bar_visibility)
            return;

        _horizontal_scroll_bar_visibility = visibility;
        _relayout();
    }



    // #############################################################################
    // Face Implementations
    // #############################################################################

    /**
     * Lays out the content area and both scroll bars for this panel's own new size.
     */
    void ScrollPanel::_on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size)
    {
        _relayout();
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
     * Positions/sizes the content area and both scroll bars for this panel's own
     * current size, and resolves each scroll bar's actual shown/hidden state (see
     * ScrollBarVisibility) and enabled state (shown and its own axis is enabled)
     * for this pass. Auto visibility is resolved against each axis's own full-size
     * overflow first, then re-checked once more for whichever axis wasn't needed on
     * its own, against the viewport as actually reduced by the other axis's scroll
     * bar (if that one did end up shown): sufficient to converge for exactly two
     * axes.
     */
    void ScrollPanel::_layout()
    {
        const ml7::Vector2f size = get_size();

        const bool vertical_overflows_full = _vertical_scroll_enabled && _content_size.y > size.y;
        const bool horizontal_overflows_full = _horizontal_scroll_enabled && _content_size.x > size.x;

        bool vertical_shown = _vertical_scroll_bar_visibility == ScrollBarVisibility::Always
            || (_vertical_scroll_enabled && _vertical_scroll_bar_visibility == ScrollBarVisibility::Auto && vertical_overflows_full);
        bool horizontal_shown = _horizontal_scroll_bar_visibility == ScrollBarVisibility::Always
            || (_horizontal_scroll_enabled && _horizontal_scroll_bar_visibility == ScrollBarVisibility::Auto && horizontal_overflows_full);

        // Second pass: an axis not yet needed on its own might still be needed
        // once the OTHER axis's (now known to be shown) scroll bar reduces the
        // viewport available to it. The other direction doesn't need a second
        // pass: an axis already shown only becomes even more clearly needed as
        // the viewport shrinks further, never less.
        if (_vertical_scroll_enabled && _vertical_scroll_bar_visibility == ScrollBarVisibility::Auto && !vertical_shown && horizontal_shown)
            vertical_shown = _content_size.y > (size.y - SCROLLBAR_THICKNESS);
        if (_horizontal_scroll_enabled && _horizontal_scroll_bar_visibility == ScrollBarVisibility::Auto && !horizontal_shown && vertical_shown)
            horizontal_shown = _content_size.x > (size.x - SCROLLBAR_THICKNESS);

        const ml7::Vector2f viewport_size{
            std::max(0.0f, size.x - (vertical_shown ? SCROLLBAR_THICKNESS : 0.0f)),
            std::max(0.0f, size.y - (horizontal_shown ? SCROLLBAR_THICKNESS : 0.0f)),
        };

        _content_area.set_position({0.0f, 0.0f});
        _content_area.set_size(viewport_size);

        _vertical_scroll_bar.set_visible(vertical_shown);
        _vertical_scroll_bar.set_enabled(vertical_shown && _vertical_scroll_enabled);
        _vertical_scroll_bar.set_position({viewport_size.x, 0.0f});
        _vertical_scroll_bar.set_size({SCROLLBAR_THICKNESS, viewport_size.y});

        _horizontal_scroll_bar.set_visible(horizontal_shown);
        _horizontal_scroll_bar.set_enabled(horizontal_shown && _horizontal_scroll_enabled);
        _horizontal_scroll_bar.set_position({0.0f, viewport_size.y});
        _horizontal_scroll_bar.set_size({viewport_size.x, SCROLLBAR_THICKNESS});
    }

    /**
     * Refreshes both scroll bars' range/visible_fraction/step from the current
     * content size vs. the content area's (viewport) size, clamping a disabled
     * axis's range to [0, 0].
     */
    void ScrollPanel::_update_scroll_bars()
    {
        const ml7::Vector2f viewport_size = _content_area.get_size();

        const float vertical_max = _vertical_scroll_enabled ? std::max(0.0f, _content_size.y - viewport_size.y) : 0.0f;
        _vertical_scroll_bar.set_range(0.0f, vertical_max);
        _vertical_scroll_bar.set_visible_fraction(_vertical_scroll_enabled && _content_size.y > 0.0f ? viewport_size.y / _content_size.y : 1.0f);
        _vertical_scroll_bar.set_step(_wheel_step);

        const float horizontal_max = _horizontal_scroll_enabled ? std::max(0.0f, _content_size.x - viewport_size.x) : 0.0f;
        _horizontal_scroll_bar.set_range(0.0f, horizontal_max);
        _horizontal_scroll_bar.set_visible_fraction(_horizontal_scroll_enabled && _content_size.x > 0.0f ? viewport_size.x / _content_size.x : 1.0f);
        _horizontal_scroll_bar.set_step(_wheel_step);
    }

    /**
     * Clamps a scroll position to [0, get_content_size() - (viewport size)] per
     * axis (never negative), or to [0, 0] on an axis that's currently disabled.
     */
    ml7::Vector2f ScrollPanel::_clamp_scroll_offset(ml7::Vector2f scroll_offset) const
    {
        const ml7::Vector2f viewport_size = _content_area.get_size();

        const ml7::Vector2f max_offset{
            _horizontal_scroll_enabled ? std::max(0.0f, _content_size.x - viewport_size.x) : 0.0f,
            _vertical_scroll_enabled ? std::max(0.0f, _content_size.y - viewport_size.y) : 0.0f,
        };

        return {
            std::clamp(scroll_offset.x, 0.0f, max_offset.x),
            std::clamp(scroll_offset.y, 0.0f, max_offset.y),
        };
    }

    /**
     * Clamps and applies a new scroll position: syncs the content area and both
     * scroll bars, emitting get_scroll_changed() only if it actually changed.
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

    /**
     * Re-runs _layout(), _update_scroll_bars(), and re-clamps/re-applies the
     * current scroll position to them, in that order. Needed whenever anything any
     * of them depend on changes: this panel's own size, its content size, either
     * axis's enabled state, or either scroll bar's visibility mode.
     */
    void ScrollPanel::_relayout()
    {
        _layout();
        _update_scroll_bars();
        _apply_scroll_offset(_scroll_offset);
    }



} // namespace fl7::gui::faces
