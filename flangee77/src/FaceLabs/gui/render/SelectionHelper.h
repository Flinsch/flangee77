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
 * Knows how to draw a HasSelection face's selection highlight: a flat-colored rect,
 * spanning the face's full height, from the pixel x-offset of its selection's begin
 * code point index to its end index within `text`. No-op if the face isn't a
 * HasSelection, or it currently has no selection. Talks only to AbstractRenderer's
 * public (backend-agnostic) drawing primitives, so this is shared by every
 * AbstractRenderer implementation, not reimplemented per backend.
 */
class SelectionHelper
{

public:
    /**
     * Draws face's selection highlight (if any) against `text`, at
     * absolute_position, sized/styled per face's effective style, via renderer.
     */
    void draw(AbstractRenderer* renderer, cl7::u32string_view text, const Face& face, ml7::Vector2f absolute_position);

}; // class SelectionHelper



} // namespace fl7::gui::render

#endif // FL7_GUI_RENDER_SELECTIONHELPER_H
