#ifndef FL7_GUI_IMAGE_H
#define FL7_GUI_IMAGE_H

#include <XiaoLabs/graphics/textures/Texture2D.h>

#include <MathLabs/Vector2.h>



namespace fl7::gui {



/**
 * A texture region to be displayed as-is (see HasImage/faces::PictureBox).
 * Same basic shape as fonts::Icon, minus the glyph-layout-specific fields
 * (baseline offset, layout advance) that don't apply outside of text.
 */
struct Image
{
    /** The (not owned) texture to sample the image from. */
    xl7::graphics::textures::Texture2D::Id texture_id;

    /** The source rectangle within the texture, in normalized (0..1) UV coordinates. */
    ml7::Vector2f uv_min{0.0f, 0.0f};
    ml7::Vector2f uv_max{1.0f, 1.0f};

    /**
     * The image's native (unscaled) size, in pixels. Needed by
     * ImageScaleMode::Zoom/Center to know the image's own aspect ratio/size;
     * ignored by Stretch.
     */
    ml7::Vector2f native_size{0.0f, 0.0f};

}; // struct Image



} // namespace fl7::gui

#endif // FL7_GUI_IMAGE_H
