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
     * everything but chrome). The key's own level's checked/hovered/pressed/focused
     * overlays (if any) whose state is currently active in `state` are then applied
     * on top, in that fixed priority order (see ThemeLevel).
     */
    Style Theme::resolve(cl7::u8string_view key, const State& state) const
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
        style.selection_color = (level && level->selection_color) ? *level->selection_color : _default_level.selection_color.value_or(style.selection_color);

        if (level)
        {
            if (state.checked && level->checked)
                _apply_overlay(style, *level->checked);
            if (state.hovered && level->hovered)
                _apply_overlay(style, *level->hovered);
            if (state.pressed && level->pressed)
                _apply_overlay(style, *level->pressed);
            if (state.focused && level->focused)
                _apply_overlay(style, *level->focused);
        }

        return style;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Overwrites style's fields with whichever ones overlay actually has set,
     * leaving the rest of style untouched.
     */
    void Theme::_apply_overlay(Style& style, const ThemeLevel& overlay)
    {
        if (overlay.font)
            style.font = overlay.font;
        if (overlay.chrome)
            style.chrome = overlay.chrome;
        if (overlay.text_style)
            style.text_style = *overlay.text_style;
        if (overlay.background_color)
            style.background_color = *overlay.background_color;
        if (overlay.border_color)
            style.border_color = *overlay.border_color;
        if (overlay.border_width)
            style.border_width = *overlay.border_width;
        if (overlay.selection_color)
            style.selection_color = *overlay.selection_color;
    }



} // namespace fl7::gui
