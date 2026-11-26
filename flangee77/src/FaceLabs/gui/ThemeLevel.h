#ifndef FL7_GUI_THEMELEVEL_H
#define FL7_GUI_THEMELEVEL_H
#include "./ThemeOverlay.h"

#include <optional>



namespace fl7::gui {



/**
 * One cascading level of a theme: either the theme-wide default level, or a single
 * face-type/role level. Inherits `ThemeOverlay`'s own fields (chrome, font, text
 * style, colors, padding, etc.), plus its own independent, not mutually exclusive
 * checked/hovered/pressed/focused overlays, each a plain `ThemeOverlay`. See
 * `Theme::resolve()` for how a level's fields cascade and its overlays combine.
 */
struct ThemeLevel
    : ThemeOverlay
{
    std::optional<ThemeOverlay> checked;
    std::optional<ThemeOverlay> hovered;
    std::optional<ThemeOverlay> pressed;
    std::optional<ThemeOverlay> focused;

}; // struct ThemeLevel



} // namespace fl7::gui

#endif // FL7_GUI_THEMELEVEL_H
