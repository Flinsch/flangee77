#ifndef FL7_GUI_FACES_SCROLLPANEL_H
#define FL7_GUI_FACES_SCROLLPANEL_H
#include "../Container.h"
#include "../HasBackground.h"



namespace fl7::gui::faces {



/**
 * A Panel-like background/border chrome around arbitrary, publicly addable
 * children, except its content is allowed to be larger than its own (viewport)
 * bounds: scrolled into view via get_scroll_offset()/set_scroll_offset(), clamped
 * to get_content_size() (set explicitly by the caller: there's no layout system yet
 * to compute a content size from the children themselves, see Container).
 * Overflowing content is still clipped to this face's own bounds regardless (see
 * Collection::clips_children()), same as any other Collection's. This face has no
 * opinion on how get_scroll_offset() actually gets driven: pair it with a ScrollBar
 * (or two, one per axis) yourself, forwarding its get_changed() into
 * set_scroll_offset().
 */
class ScrollPanel
    : public Container
    , public HasBackground
{

public:
    ScrollPanel() = default;

    ScrollPanel(const ScrollPanel&) = delete;
    ScrollPanel& operator=(const ScrollPanel&) = delete;
    ScrollPanel(ScrollPanel&&) = delete;
    ScrollPanel& operator=(ScrollPanel&&) = delete;

    ~ScrollPanel() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this panel's content size: how large its (scrollable) content is, regardless of its own (viewport) size. */
    ml7::Vector2f get_content_size() const { return _content_size; }

    /** Sets this panel's content size. Re-clamps the current scroll offset to it. */
    void set_content_size(ml7::Vector2f content_size);

    /** Returns this panel's current scroll offset (see Collection::get_scroll_offset()). */
    ml7::Vector2f get_scroll_offset() const override { return _scroll_offset; }

    /** Sets this panel's scroll offset, clamped to [0, get_content_size() - get_size()] per axis (never negative). */
    void set_scroll_offset(ml7::Vector2f scroll_offset);



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this panel's theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"scroll_panel"; }

    /** Re-clamps the current scroll offset whenever this panel's own (viewport) size changes. */
    void _on_size_changed(ml7::Vector2f old_size, ml7::Vector2f new_size) override;



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /** Clamps a scroll offset to [0, get_content_size() - get_size()] per axis (never negative). */
    ml7::Vector2f _clamp_scroll_offset(ml7::Vector2f scroll_offset) const;



    // #############################################################################
    // Attributes
    // #############################################################################

    ml7::Vector2f _content_size;
    ml7::Vector2f _scroll_offset;

}; // class ScrollPanel



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_SCROLLPANEL_H
