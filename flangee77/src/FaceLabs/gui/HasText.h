#ifndef FL7_GUI_HASTEXT_H
#define FL7_GUI_HASTEXT_H

#include <MathLabs/Vector2.h>

#include <CoreLabs/string.h>

#include <optional>



namespace fl7::gui {



/**
 * Interface: opts a face into having a piece of text drawn by the renderer.
 */
class HasText
{

public:
    virtual ~HasText() = default;

    /**
     * Returns the text to display.
     */
    virtual const cl7::u32string& get_display_text() const = 0;

    /**
     * Returns how far the displayed text is currently scrolled within its own
     * box: subtracted from the box's own position before laying out/drawing/
     * hit-testing it, same sign convention as Collection::get_scroll_offset().
     * Default: none; only faces::TextField's multiline mode actually scrolls.
     */
    virtual ml7::Vector2f get_text_scroll_offset() const { return {}; }

    /**
     * Returns the box size to actually lay the text out against, if different
     * from the face's own get_size() narrowed by its own effective padding (see
     * Face::get_effective_padding()); e.g., narrower still, to leave room for a
     * scroll bar (see faces::TextField's multiline mode). Overriding this means
     * taking over accounting for that padding yourself (see faces::TextField's
     * own _effective_text_box_size()); render::AbstractRenderer::_draw_face()
     * only applies it on your behalf while this stays nullopt, the default.
     */
    virtual std::optional<ml7::Vector2f> get_text_box_size() const { return std::nullopt; }

}; // class HasText



} // namespace fl7::gui

#endif // FL7_GUI_HASTEXT_H
