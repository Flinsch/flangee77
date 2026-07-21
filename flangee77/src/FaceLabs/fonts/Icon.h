#ifndef FL7_FONTS_ICON_H
#define FL7_FONTS_ICON_H

#include <MathLabs/Vector2.h>

#include <XiaoLabs/graphics/textures/Texture2D.h>



namespace fl7::fonts {



/**
 * Describes an inline icon: an arbitrary (caller-supplied, not owned) texture drawn
 * as if it were a glyph, via an IconRun. Unlike glyphs, icons keep their own
 * texture colors (they are not tinted/coverage-masked by GlyphStyle::text_color)
 * and are ignored by italic/weight.
 */
struct Icon
{
    /** The (not owned) texture to sample the icon image from. */
    xl7::graphics::textures::Texture2D::Id texture_id;

    /** The source rectangle within the texture, in normalized (0..1) UV coordinates. */
    ml7::Vector2f uv_min = {0.0f, 0.0f};
    ml7::Vector2f uv_max = {1.0f, 1.0f};

    /** The display size, in scaled pixels; also used as-is for the layout advance width. */
    ml7::Vector2f size = {16.0f, 16.0f};

    /**
     * The top-left corner, relative to the cursor/baseline origin at the
     * icon's code point position. Same convention as raster::PixelOffset for
     * glyphs: offset.y is typically negative, to sit above the baseline
     * (e.g., offset = {0, -size.y} bottom-aligns the icon to the baseline).
     */
    ml7::Vector2f offset = {0.0f, 0.0f};
};



} // namespace fl7::fonts

#endif // FL7_FONTS_ICON_H
