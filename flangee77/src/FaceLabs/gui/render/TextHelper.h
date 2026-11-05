#ifndef FL7_GUI_RENDER_TEXTHELPER_H
#define FL7_GUI_RENDER_TEXTHELPER_H

#include <MathLabs/Vector2.h>

#include <CoreLabs/string.h>



namespace fl7::gui {
class Face;
} // namespace fl7::gui

namespace fl7::gui::render {



class AbstractRenderer;



/**
 * Knows how to draw a HasText face's text over its full bounds. Talks only to
 * AbstractRenderer's public (backend-agnostic) drawing primitives, so this is
 * shared by every AbstractRenderer implementation, not reimplemented per backend.
 */
class TextHelper
{

public:
    /**
     * Draws text at absolute_position - text_scroll_offset, laid out against
     * text_box_size, styled per face's effective style, via renderer (see
     * HasText::get_text_scroll_offset()/get_text_box_size()).
     */
    void draw(AbstractRenderer* renderer, cl7::u32string_view text, const Face& face, ml7::Vector2f absolute_position, ml7::Vector2f text_scroll_offset, ml7::Vector2f text_box_size);

}; // class TextHelper



} // namespace fl7::gui::render

#endif // FL7_GUI_RENDER_TEXTHELPER_H
