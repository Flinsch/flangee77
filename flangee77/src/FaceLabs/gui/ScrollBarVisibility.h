#ifndef FL7_GUI_SCROLLBARVISIBILITY_H
#define FL7_GUI_SCROLLBARVISIBILITY_H


namespace fl7::gui {



/**
 * A scroll bar's visibility, independent of whether its own axis (or, for a
 * single-axis scroller like faces::TextField, the only axis there is) is
 * actually scrollable/overflowing at all. Shared by every face that offers
 * this choice (see faces::ScrollPanel, faces::TextField), rather than each
 * redeclaring its own, identical enum.
 */
enum struct ScrollBarVisibility
{
    /** Shows the scroll bar unconditionally (even while its own axis doesn't actually overflow, in which case it just sits there inert, full-range, nothing to drag). */
    Always,
    /** Hides the scroll bar unconditionally. */
    Never,
    /** Shows the scroll bar only while the content actually overflows that axis's (viewport) bounds. */
    Auto,
}; // enum struct ScrollBarVisibility



} // namespace fl7::gui

#endif // FL7_GUI_SCROLLBARVISIBILITY_H
