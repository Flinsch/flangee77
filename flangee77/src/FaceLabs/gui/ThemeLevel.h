#ifndef FL7_GUI_THEMELEVEL_H
#define FL7_GUI_THEMELEVEL_H

#include "./NineSliceChrome.h"

#include <FaceLabs/fonts/Font.h>
#include <FaceLabs/fonts/TextStyle.h>

#include <XiaoLabs/graphics/Color.h>

#include <memory>
#include <optional>



namespace fl7::gui {



/**
 * One cascading level of a theme: either the theme-wide default level, or a single
 * face-type/role level. Unlike style, every value-typed field here is optional,
 * "not set at this level" is a real, distinct state, needed so resolution can fall
 * through to the next level field by field.
 */
struct ThemeLevel
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

    /**
     * Independent, not mutually exclusive (a face can be checked and hovered and
     * pressed and focused all at once) per-state overlays, applied on top of the
     * fields above once this level's fields have already been resolved (see
     * Theme::resolve()), in this fixed priority order: checked, then hovered, then
     * pressed, then focused (each later one's set fields winning over the earlier
     * ones'). checked comes first (rather than alongside the transient
     * hovered/pressed/focused trio) because it's a persistent, semantic value (see
     * HasCheckedState), not a passing interaction: momentary feedback should tint
     * whichever (checked or unchecked) look is currently in effect, not replace it.
     * Only the fields directly above are ever consulted from an overlay: an
     * overlay's own nested checked/hovered/pressed/focused pointers, if it had any,
     * are never applied. One level of nesting is all that's meaningful. A
     * unique_ptr (not optional) only because ThemeLevel refers to itself here.
     */
    std::unique_ptr<ThemeLevel> checked;
    std::unique_ptr<ThemeLevel> hovered;
    std::unique_ptr<ThemeLevel> pressed;
    std::unique_ptr<ThemeLevel> focused;

}; // struct ThemeLevel



} // namespace fl7::gui

#endif // FL7_GUI_THEMELEVEL_H
