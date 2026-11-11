#ifndef FL7_GUI_PADDING_H
#define FL7_GUI_PADDING_H



namespace fl7::gui {



/**
 * A fixed space kept on each of a face's (or, within `layout::BoxLayout`, a
 * container's) four edges.
 */
struct Padding
{
    Padding() = default;

    /** The same padding on every side. */
    Padding(float uniform) : left(uniform), top(uniform), right(uniform), bottom(uniform) {} // Deliberately not explicit: the whole point is to allow a bare float as padding.
    Padding(float left, float top, float right, float bottom) : left(left), top(top), right(right), bottom(bottom) {}

    float left = 0.0f;
    float top = 0.0f;
    float right = 0.0f;
    float bottom = 0.0f;
}; // struct Padding



} // namespace fl7::gui

#endif // FL7_GUI_PADDING_H
