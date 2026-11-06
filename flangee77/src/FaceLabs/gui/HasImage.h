#ifndef FL7_GUI_HASIMAGE_H
#define FL7_GUI_HASIMAGE_H

#include "./Image.h"



namespace fl7::gui {



/**
 * How a HasImage face's image (see HasImage::get_image()) is fit into its own
 * bounds.
 */
enum struct ImageScaleMode
{
    /** Stretches the image to exactly fill the face's own bounds, distorting its aspect ratio if necessary. */
    Stretch,
    /** Scales the image uniformly (preserving its own aspect ratio) to fit entirely within the face's own bounds, centered; letterboxed on whichever axis has room left over. */
    Zoom,
    /** Draws the image at its own native size, centered: clipped (see AbstractRenderer::_draw_face()) if larger than the face's own bounds, letterboxed if smaller. */
    Center,
}; // enum struct ImageScaleMode



/**
 * Interface: opts a face into having an image drawn by the renderer.
 */
class HasImage
{

public:
    virtual ~HasImage() = default;

    /** Returns the image to display, or nullptr if none is currently set. */
    virtual const Image* get_image() const = 0;

    /** Returns how get_image() (if any) is fit into this face's own bounds; see ImageScaleMode. Default: Stretch. */
    virtual ImageScaleMode get_image_scale_mode() const { return ImageScaleMode::Stretch; }

}; // class HasImage



} // namespace fl7::gui

#endif // FL7_GUI_HASIMAGE_H
