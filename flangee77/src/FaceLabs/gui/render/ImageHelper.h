#ifndef FL7_GUI_RENDER_IMAGEHELPER_H
#define FL7_GUI_RENDER_IMAGEHELPER_H

#include "../HasImage.h"

#include <MathLabs/Vector2.h>



namespace fl7::gui::render {



class AbstractRenderer;



/**
 * Knows how to draw a HasImage face's image within a given box, per
 * `ImageScaleMode`: stretched to fill it exactly, uniformly scaled (preserving its
 * own aspect ratio) to fit within it, or drawn at its own native size, centered.
 * Talks only to `AbstractRenderer`'s public (backend-agnostic) drawing primitives,
 * so this is shared by every `AbstractRenderer` implementation, not reimplemented
 * per backend.
 */
class ImageHelper
{

public:
    /**
     * Draws image within the box at box_position/box_size, per scale_mode
     * (see HasImage::get_image_scale_mode()).
     */
    void draw(AbstractRenderer* renderer, const Image& image, ml7::Vector2f box_position, ml7::Vector2f box_size, ImageScaleMode scale_mode);

}; // class ImageHelper



} // namespace fl7::gui::render

#endif // FL7_GUI_RENDER_IMAGEHELPER_H
