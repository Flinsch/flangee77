#ifndef FL7_GUI_RENDER_SELECTIONHELPER_H
#define FL7_GUI_RENDER_SELECTIONHELPER_H

#include <MathLabs/Vector2.h>

#include <CoreLabs/string.h>



namespace fl7::gui {
class Face;
} // namespace fl7::gui

namespace fl7::gui::render {



class AbstractRenderer;



/**
 * Knows how to draw a HasSelection face's selection highlight: one flat-colored
 * rect per line the selection actually spans (see fonts::TextLayout::
 * lay_out_box(): text may have grown to more than one line, see gui::faces::
 * TextField's multiline support), each from the pixel x-offset of its
 * selection's begin code point index to its end index within `text`, clipped
 * to that line's own range. No-op if the face isn't a HasSelection, or it
 * currently has no selection. Talks only to AbstractRenderer's public
 * (backend-agnostic) drawing primitives, so this is shared by every
 * AbstractRenderer implementation, not reimplemented per backend.
 */
class SelectionHelper
{

public:
    /**
     * Draws face's selection highlight (if any) against `text`, at
     * absolute_position - text_scroll_offset, laid out against text_box_size,
     * styled per face's effective style, via renderer (see
     * HasText::get_text_scroll_offset()/get_text_box_size()). One rect per line
     * the selection actually spans (see fonts::TextLayout::lay_out_box()),
     * rather than assuming there's only ever the one, now that text may have
     * grown to more than one line (see gui::faces::TextField's multiline
     * support).
     */
    void draw(AbstractRenderer* renderer, cl7::u32string_view text, const Face& face, ml7::Vector2f absolute_position, ml7::Vector2f text_scroll_offset, ml7::Vector2f text_box_size);

}; // class SelectionHelper



} // namespace fl7::gui::render

#endif // FL7_GUI_RENDER_SELECTIONHELPER_H
