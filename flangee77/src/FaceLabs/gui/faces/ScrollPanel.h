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
 * Both scroll bars are always instantiated and always shown for now; making each
 * individually enable-able/hide-able (or shown only when actually needed) is
 * planned as a follow-up, not implemented yet.
 */
class ScrollPanel
    : public Compound
    , public HasBackground
{

public:
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

    /** Positions/sizes the content area and both scroll bars for this panel's own current size. */
    void _layout();

    /** Refreshes both scroll bars' range/visible_fraction/step from the current content size vs. the content area's (viewport) size. */
    void _update_scroll_bars();

    /** Clamps a scroll position to [0, get_content_size() - (viewport size)] per axis (never negative). */
    ml7::Vector2f _clamp_scroll_offset(ml7::Vector2f scroll_offset) const;

    /** Clamps and applies a new scroll position: syncs the content area and both scroll bars, emitting get_scroll_changed() only if it actually changed. */
    void _apply_scroll_offset(ml7::Vector2f scroll_offset);



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

    cl7::behavioral::Signal<ml7::Vector2f> _scroll_changed;

}; // class ScrollPanel



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_SCROLLPANEL_H
