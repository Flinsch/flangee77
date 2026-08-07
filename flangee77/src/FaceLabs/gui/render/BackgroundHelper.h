#ifndef FL7_GUI_RENDER_BACKGROUNDHELPER_H
#define FL7_GUI_RENDER_BACKGROUNDHELPER_H

#include <MathLabs/Vector2.h>



namespace fl7::gui {
class Face;
struct NineSliceChrome;
} // namespace fl7::gui

namespace fl7::gui::render {



class AbstractRenderer;



/**
 * Knows how to draw a HasBackground face's background: a 9-sliced NineSliceChrome
 * sprite if Style::chrome is set, else a flat Style::background_color rect, over
 * the face's full bounds. Talks only to AbstractRenderer's public
 * (backend-agnostic) drawing primitives, so this is shared by every
 * AbstractRenderer implementation, not reimplemented per backend.
 */
class BackgroundHelper
{

public:
    /**
     * Draws face's background at absolute_position via renderer.
     */
    void draw(AbstractRenderer* renderer, const Face& face, ml7::Vector2f absolute_position);



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Draws the given chrome sprite 9-sliced over a destination rect at
     * absolute_position/size: corners at native size, edges stretched along their
     * one free axis, the center stretched along both.
     */
    void _draw_chrome(AbstractRenderer* renderer, const NineSliceChrome& chrome, ml7::Vector2f absolute_position, ml7::Vector2f size);

}; // class BackgroundHelper



} // namespace fl7::gui::render

#endif // FL7_GUI_RENDER_BACKGROUNDHELPER_H
