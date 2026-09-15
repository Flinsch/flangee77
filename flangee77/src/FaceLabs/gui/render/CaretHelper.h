#ifndef FL7_GUI_RENDER_CARETHELPER_H
#define FL7_GUI_RENDER_CARETHELPER_H

#include <MathLabs/Vector2.h>

#include <CoreLabs/string.h>



namespace fl7::gui {
class Face;
} // namespace fl7::gui

namespace fl7::gui::render {



class AbstractRenderer;



/**
 * Knows how to draw a HasCaret face's caret: a thin vertical line, spanning the
 * face's full height, at the pixel x-offset corresponding to its current code point
 * index within `text`. No-op if the face isn't a HasCaret, or its caret isn't
 * currently visible. Talks only to AbstractRenderer's public (backend-agnostic)
 * drawing primitives, so this is shared by every AbstractRenderer implementation,
 * not reimplemented per backend.
 */
class CaretHelper
{

public:
    /**
     * Draws face's caret (if any, and currently visible) against `text`, at
     * absolute_position, sized/styled per face's effective style, via renderer.
     */
    void draw(AbstractRenderer* renderer, cl7::u32string_view text, const Face& face, ml7::Vector2f absolute_position);

}; // class CaretHelper



} // namespace fl7::gui::render

#endif // FL7_GUI_RENDER_CARETHELPER_H
