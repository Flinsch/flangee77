#ifndef FL7_GUI_NINESLICECHROME_H
#define FL7_GUI_NINESLICECHROME_H
#include <XiaoLabs/graphics/textures/Texture2D.h>

#include <MathLabs/Vector2.h>



namespace fl7::gui {



/**
 * A 9-slice-able chrome sprite: a rect within a texture (uv_min/uv_max, its native
 * pixel size given by sprite_size), subdivided into a 3x3 grid via four insets (in
 * the sprite's own pixel space, not normalized UV, for easier authoring). When
 * drawn (e.g., see render::DefaultRenderer::_draw_chrome) onto an arbitrarily sized
 * destination rect, corners are placed at native size, edges stretch along their
 * one free axis, and the center stretches along both: the classic 9-slice/9-patch
 * technique for scaling chrome (window/panel backgrounds, button skins, etc.)
 * without distorting borders or corners.
 */
struct NineSliceChrome
{
    xl7::graphics::textures::Texture2D::Id texture_id;

    /** The top-left corner of the sprite's rect within the texture, in normalized UV space (0..1). */
    ml7::Vector2f uv_min{0.0f, 0.0f};
    /** The bottom-right corner of the sprite's rect within the texture, in normalized UV space (0..1). */
    ml7::Vector2f uv_max{1.0f, 1.0f};

    /**
     * The sprite's native (unscaled) size, in pixels. Needed to convert the
     * pixel-space insets below into fractions of uv_min/uv_max, and to size the
     * corners/edges on screen at native (unstretched) resolution.
     */
    ml7::Vector2f sprite_size{0.0f, 0.0f};

    /** The left inset (in the sprite's own pixel space) that carves the sprite into a 3x3 grid. */
    float inset_left = 0.0f;
    /** The top inset (in the sprite's own pixel space) that carves the sprite into a 3x3 grid. */
    float inset_top = 0.0f;
    /** The right inset (in the sprite's own pixel space) that carves the sprite into a 3x3 grid. */
    float inset_right = 0.0f;
    /** The bottom inset (in the sprite's own pixel space) that carves the sprite into a 3x3 grid. */
    float inset_bottom = 0.0f;

}; // struct NineSliceChrome



} // namespace fl7::gui

#endif // FL7_GUI_NINESLICECHROME_H
