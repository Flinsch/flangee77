#include "ImageHelper.h"

#include "./AbstractRenderer.h"

#include "../Face.h"

#include <algorithm>



namespace fl7::gui::render {



    /**
     * Draws image within face's own bounds at absolute_position, per scale_mode
     * (see HasImage::get_image_scale_mode()).
     */
    void ImageHelper::draw(AbstractRenderer* renderer, const Image& image, const Face& face, ml7::Vector2f absolute_position, ImageScaleMode scale_mode)
    {
        const ml7::Vector2f size = face.get_size();

        // Stretch (the default) simply fills the whole face, same as Zoom/Center
        // fall back to doing while there's no native_size to scale/center against
        // (an image set up without one, e.g., in a test).
        ml7::Vector2f dest_size = size;
        if (scale_mode == ImageScaleMode::Zoom && image.native_size.x > 0.0f && image.native_size.y > 0.0f)
        {
            const float scale = std::min(size.x / image.native_size.x, size.y / image.native_size.y);
            dest_size = image.native_size * scale;
        }
        else if (scale_mode == ImageScaleMode::Center && image.native_size.x > 0.0f && image.native_size.y > 0.0f)
        {
            dest_size = image.native_size;
        }

        const ml7::Vector2f dest_position = absolute_position + (size - dest_size) * 0.5f;

        renderer->draw_textured_rect(dest_position, dest_position + dest_size, image.texture_id, image.uv_min, image.uv_max, {1.0f, 1.0f, 1.0f, 1.0f});
    }



} // namespace fl7::gui::render
