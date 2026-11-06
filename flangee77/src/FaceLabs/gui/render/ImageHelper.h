#ifndef FL7_GUI_RENDER_IMAGEHELPER_H
#define FL7_GUI_RENDER_IMAGEHELPER_H

#include "../HasImage.h"

#include <MathLabs/Vector2.h>



namespace fl7::gui {
class Face;
} // namespace fl7::gui

namespace fl7::gui::render {



class AbstractRenderer;



/**
 * Knows how to draw a HasImage face's image within its own bounds, per
 * ImageScaleMode (see HasImage::get_image_scale_mode()): stretched to fill them
 * exactly, uniformly scaled (preserving its own aspect ratio) to fit within them,
 * or drawn at its own native size, centered. Talks only to AbstractRenderer's
 * public (backend-agnostic) drawing primitives, so this is shared by every
 * AbstractRenderer implementation, not reimplemented per backend.
 */
class ImageHelper
{

public:
    /**
     * Draws image within face's own bounds at absolute_position, per scale_mode
     * (see HasImage::get_image_scale_mode()).
     */
    void draw(AbstractRenderer* renderer, const Image& image, const Face& face, ml7::Vector2f absolute_position, ImageScaleMode scale_mode);

}; // class ImageHelper



} // namespace fl7::gui::render

#endif // FL7_GUI_RENDER_IMAGEHELPER_H
