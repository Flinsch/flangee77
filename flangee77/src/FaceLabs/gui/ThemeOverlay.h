#ifndef FL7_GUI_THEMEOVERLAY_H
#define FL7_GUI_THEMEOVERLAY_H

#include "./NineSliceChrome.h"
#include "./Padding.h"

#include <FaceLabs/fonts/Font.h>
#include <FaceLabs/fonts/TextStyle.h>

#include <XiaoLabs/graphics/Color.h>

#include <optional>



namespace fl7::gui {



/**
 * The fields a theme level (or one of its own checked/hovered/pressed/focused
 * overlays, see `ThemeLevel`) can set, each independently optional so resolution
 * can tell "not set at this level" from any particular value (see
 * `Theme::resolve()`). Factored out of `ThemeLevel` so a
 * checked/hovered/pressed/focused overlay can be one of these without `ThemeLevel`
 * containing itself: an overlay only ever has these direct fields, never nested
 * overlays of its own.
 */
struct ThemeOverlay
{
    /**
     * Non-owning: whoever builds the theme keeps the 9-slice chrome alive.
     * Unlike the other fields, this never falls through to the theme-wide default
     * level during resolution: a chrome sprite is tied to a specific texture/9-slice
     * layout, so a "just any chrome" theme-wide fallback wouldn't be meaningful.
     */
    const NineSliceChrome* chrome = nullptr;

    /** Non-owning: whoever builds the theme keeps the font alive. */
    fonts::Font* font = nullptr;

    std::optional<fonts::TextStyle> text_style;

    std::optional<xl7::graphics::Color> background_color;
    std::optional<xl7::graphics::Color> border_color;
    /** 0 = no border. */
    std::optional<float> border_width;
    std::optional<xl7::graphics::Color> selection_color;
    std::optional<Padding> padding;
    std::optional<float> scroll_bar_thickness;
    std::optional<float> title_bar_height;
    std::optional<float> resize_handle_size;

}; // struct ThemeOverlay



} // namespace fl7::gui

#endif // FL7_GUI_THEMEOVERLAY_H
