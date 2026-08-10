#include "Theme.h"



namespace fl7::gui {



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the level for the specified face-type/role key, creating an initially
     * empty one if it doesn't exist yet (mutable, for building up a theme).
     */
    ThemeLevel& Theme::get_level(cl7::u8string_view key)
    {
        auto it = _levels.find(key);
        if (it == _levels.end())
            it = _levels.emplace(cl7::u8string(key), ThemeLevel{}).first;
        return it->second;
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Resolves the specified face-type/role key into a complete style: each field
     * individually falls back from that key's own level to the theme-wide default
     * level, then to style's own hard-coded default, except chrome, which only ever
     * comes from the key's own level. Unknown keys resolve as if their level were
     * empty (falling through to the theme-wide default/style's own defaults for
     * everything but chrome).
     */
    Style Theme::resolve(cl7::u8string_view key) const
    {
        Style style;

        const auto it = _levels.find(key);
        const ThemeLevel* level = it != _levels.end() ? &it->second : nullptr;

        style.font = (level && level->font) ? level->font : _default_level.font;
        style.chrome = level ? level->chrome : nullptr;

        style.text_style = (level && level->text_style) ? *level->text_style : _default_level.text_style.value_or(style.text_style);
        style.background_color = (level && level->background_color) ? *level->background_color : _default_level.background_color.value_or(style.background_color);
        style.border_color = (level && level->border_color) ? *level->border_color : _default_level.border_color.value_or(style.border_color);
        style.border_width = (level && level->border_width) ? *level->border_width : _default_level.border_width.value_or(style.border_width);

        return style;
    }



} // namespace fl7::gui
