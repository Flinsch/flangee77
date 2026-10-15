#ifndef FL7_GUI_FACES_SCROLLPANEL_H
#define FL7_GUI_FACES_SCROLLPANEL_H
#include "../Compound.h"
#include "../Container.h"
#include "../HasBackground.h"

#include "./ScrollBar.h"

#include <CoreLabs/behavioral/Signal.h>



namespace fl7::gui::faces {



/**
 * A Panel-like background/border chrome around an internal, always-present vertical
 * and horizontal ScrollBar plus an arbitrary, publicly addable content area (see
 * get_content_area(), the only place a caller can actually add children, same as
 * Window's), whose content is allowed to be larger than its own (viewport) bounds:
 * scrolled into view via get_scroll_position()/set_scroll_position(), clamped to
 * get_content_size() (set explicitly by the caller: there's no layout system yet to
 * compute a content size from the children themselves, see Container). Overflowing
 * content is still clipped to the content area's own bounds regardless (see
 * Collection::clips_children()). Also scrolls vertically on its own, by
 * get_wheel_step() per wheel notch, while hovered (or bubbled up to from a child
 * that doesn't itself want the wheel (e.g., a Label), but not one that does (e.g.,
 * a Slider); see Face::_on_mouse_wheel()).
 *
 * Both scroll bars are always instantiated, but each can be independently
 * disabled (see set_vertical_scroll_enabled()/set_horizontal_scroll_enabled())
 * and each has its own, independent visibility mode (see ScrollBarVisibility).
 */
class ScrollPanel
    : public Compound
    , public HasBackground
{

public:
    /**
     * A scroll bar's visibility, independent of whether its own axis is actually
     * scrollable at all.
     */
    enum struct ScrollBarVisibility
    {
        /** Shows the scroll bar unconditionally (even while its own axis is disabled, in which case it just sits there inert, full-range, nothing to drag). */
        Always,
        /** Hides the scroll bar unconditionally. */
        Never,
        /** Shows the scroll bar only while the content actually overflows that axis's (viewport) bounds. */
        Auto,
    };



    ScrollPanel();

    ScrollPanel(const ScrollPanel&) = delete;
    ScrollPanel& operator=(const ScrollPanel&) = delete;
    ScrollPanel(ScrollPanel&&) = delete;
    ScrollPanel& operator=(ScrollPanel&&) = delete;

    ~ScrollPanel() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this panel's content area: the (only) place a caller can actually add children. */
    Container& get_content_area() { return _content_area; }
    /** Returns this panel's content area: the (only) place a caller can actually add children. */
    const Container& get_content_area() const { return _content_area; }

    /** Returns this panel's content size: how large its (scrollable) content is, regardless of its own (viewport) size. */
    ml7::Vector2f get_content_size() const { return _content_size; }

    /** Sets this panel's content size, re-clamping (and, if that actually changes it, see get_scroll_changed()) the current scroll position to it. */
    void set_content_size(ml7::Vector2f content_size);

    /** Returns this panel's current scroll position (how far its content area's content is currently scrolled, in content pixels). */
    ml7::Vector2f get_scroll_position() const { return _scroll_offset; }

    /** Sets this panel's scroll position, clamped to [0, get_content_size() - (viewport size)] per axis (never negative), unless already at that (clamped) position. */
    void set_scroll_position(ml7::Vector2f scroll_position);

    /** Returns the amount the mouse wheel scrolls (vertically) per notch, while hovered or bubbled up to (see _on_mouse_wheel()); also both scroll bars' own step. */
    float get_wheel_step() const { return _wheel_step; }

    /** Sets the amount the mouse wheel scrolls (vertically) per notch; also both scroll bars' own step. */
    void set_wheel_step(float wheel_step);

    /** Returns the signal emitted (with the new position) whenever this panel's scroll position actually changes, regardless of what changed it (set_scroll_position(), set_content_size() re-clamping it, a resize, the mouse wheel, or either scroll bar). */
    cl7::behavioral::Signal<ml7::Vector2f>& get_scroll_changed() { return _scroll_changed; }

    /** Returns whether this panel's content can currently be scrolled vertically at all; see set_vertical_scroll_enabled(). */
    bool get_vertical_scroll_enabled() const { return _vertical_scroll_enabled; }

    /**
     * Sets whether this panel's content can be scrolled vertically at all.
     * While disabled, the vertical scroll position stays clamped to 0 regardless of
     * content size, and the vertical scroll bar is disabled (however its own
     * visibility is configured; see set_vertical_scroll_bar_visibility()), since
     * there would be nothing for it to do.
     */
    void set_vertical_scroll_enabled(bool enabled);

    /** Returns whether this panel's content can currently be scrolled horizontally at all; see set_horizontal_scroll_enabled(). */
    bool get_horizontal_scroll_enabled() const { return _horizontal_scroll_enabled; }

    /**
     * Sets whether this panel's content can be scrolled horizontally at all.
     * While disabled, the horizontal scroll position stays clamped to 0 regardless of
     * content size, and the horizontal scroll bar is disabled (however its own
     * visibility is configured; see set_horizontal_scroll_bar_visibility()), since
     * there would be nothing for it to do.
     */
    void set_horizontal_scroll_enabled(bool enabled);

    /** Returns this panel's vertical scroll bar's visibility mode; see set_vertical_scroll_bar_visibility(). */
    ScrollBarVisibility get_vertical_scroll_bar_visibility() const { return _vertical_scroll_bar_visibility; }

    /**
     * Sets this panel's vertical scroll bar's visibility mode. Auto shows it only
     * while the content actually overflows vertically, accounting for whichever
     * other scroll bar ends up shown too, since showing one reduces the viewport
     * (and therefore how much room the other axis's content actually has) available
     * to the other axis.
     */
    void set_vertical_scroll_bar_visibility(ScrollBarVisibility visibility);

    /** Returns this panel's horizontal scroll bar's visibility mode; see set_horizontal_scroll_bar_visibility(). */
    ScrollBarVisibility get_horizontal_scroll_bar_visibility() const { return _horizontal_scroll_bar_visibility; }

    /**
     * Sets this panel's horizontal scroll bar's visibility mode. Auto shows it only
     * while the content actually overflows horizontally, accounting for whichever
     * other scroll bar ends up shown too, since showing one reduces the viewport
     * (and therefore how much room the other axis's content actually has) available
     * to the other axis.
     */
    void set_horizontal_scroll_bar_visibility(ScrollBarVisibility visibility);



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this panel's theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"scroll_panel"; }

    /** Lays out the content area and both scroll bars for this panel's own new size. */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override;

    /** Scrolls vertically by get_wheel_step() per wheel notch. Always consumes. */
    bool _on_mouse_wheel(int delta) override;



private:

    // #############################################################################
    // Types
    // #############################################################################

    /**
     * The actual (arbitrary, publicly addable) content viewport: a plain
     * Container except its children are shifted by the owning ScrollPanel's
     * current scroll position (see Collection::get_scroll_offset()). The
     * mutator is deliberately not public: scrolling an owning ScrollPanel must
     * always go through its own set_scroll_position() instead, which clamps
     * the position and keeps both scroll bars in sync with it -- setting it
     * here directly would desync them.
     */
    class ContentArea
        : public Container
    {
    public:
        /** Returns this content area's current scroll offset, as set by the owning ScrollPanel (see Collection::get_scroll_offset()). */
        ml7::Vector2f get_scroll_offset() const override { return _scroll_offset; }

    private:
        friend class ScrollPanel;

        /** Sets this content area's scroll offset; only the owning ScrollPanel may call this (see class doc comment above). */
        void _set_scroll_offset(ml7::Vector2f scroll_offset) { _scroll_offset = scroll_offset; }

        ml7::Vector2f _scroll_offset;

    }; // class ContentArea



    // #############################################################################
    // Constants
    // #############################################################################

    /**
     * Fixed for now, should eventually come from the active theme/chrome
     * instead (e.g., the 9-slice chrome's own native scroll bar thickness).
     */
    static constexpr float SCROLLBAR_THICKNESS = 16.0f;



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
    void _layout();

    /**
     * Refreshes both scroll bars' range/visible_fraction/step from the current
     * content size vs. the content area's (viewport) size, clamping a disabled
     * axis's range to [0, 0].
     */
    void _update_scroll_bars();

    /**
     * Clamps a scroll position to [0, get_content_size() - (viewport size)] per
     * axis (never negative), or to [0, 0] on an axis that's currently disabled.
     */
    ml7::Vector2f _clamp_scroll_offset(ml7::Vector2f scroll_offset) const;

    /**
     * Clamps and applies a new scroll position: syncs the content area and both
     * scroll bars, emitting get_scroll_changed() only if it actually changed.
     */
    void _apply_scroll_offset(ml7::Vector2f scroll_offset);

    /**
     * Re-runs _layout(), _update_scroll_bars(), and re-clamps/re-applies the
     * current scroll position to them, in that order. Needed whenever anything any
     * of them depend on changes: this panel's own size, its content size, either
     * axis's enabled state, or either scroll bar's visibility mode.
     */
    void _relayout();



    // #############################################################################
    // Attributes
    // #############################################################################

    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    ContentArea& _content_area;
    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    ScrollBar& _vertical_scroll_bar;
    /** Non-owning: actually owned (as a child) by the inherited Collection. */
    ScrollBar& _horizontal_scroll_bar;

    ml7::Vector2f _content_size;
    ml7::Vector2f _scroll_offset;

    /** The amount the mouse wheel scrolls (vertically) per notch; also both scroll bars' own step. */
    float _wheel_step = 40.0f;

    bool _vertical_scroll_enabled = true;
    bool _horizontal_scroll_enabled = true;

    ScrollBarVisibility _vertical_scroll_bar_visibility = ScrollBarVisibility::Always;
    ScrollBarVisibility _horizontal_scroll_bar_visibility = ScrollBarVisibility::Auto;

    cl7::behavioral::Signal<ml7::Vector2f> _scroll_changed;

}; // class ScrollPanel



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_SCROLLPANEL_H
