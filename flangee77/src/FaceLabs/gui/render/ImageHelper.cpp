#include "ImageHelper.h"

#include "./AbstractRenderer.h"

#include <algorithm>



namespace fl7::gui::render {



    /**
     * Draws image within the box at box_position/box_size, per scale_mode
     * (see HasImage::get_image_scale_mode()).
     */
    void ImageHelper::draw(AbstractRenderer* renderer, const Image& image, ml7::Vector2f box_position, ml7::Vector2f box_size, ImageScaleMode scale_mode)
    {
        // Stretch (the default) simply fills the whole box, same as Zoom/Center
        // fall back to doing while there's no native_size to scale/center against
        // (an image set up without one, e.g., in a test).
        ml7::Vector2f dest_size = box_size;
        if (scale_mode == ImageScaleMode::Zoom && image.native_size.x > 0.0f && image.native_size.y > 0.0f)
        {
            const float scale = std::min(box_size.x / image.native_size.x, box_size.y / image.native_size.y);
            dest_size = image.native_size * scale;
        }
        else if (scale_mode == ImageScaleMode::Center && image.native_size.x > 0.0f && image.native_size.y > 0.0f)
        {
            dest_size = image.native_size;
        }

        const ml7::Vector2f dest_position = box_position + (box_size - dest_size) * 0.5f;

        renderer->draw_textured_rect(dest_position, dest_position + dest_size, image.texture_id, image.uv_min, image.uv_max, {1.0f, 1.0f, 1.0f, 1.0f});
    }



} // namespace fl7::gui::render
