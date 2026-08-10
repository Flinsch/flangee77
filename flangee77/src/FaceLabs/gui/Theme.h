#ifndef FL7_GUI_THEME_H
#define FL7_GUI_THEME_H

#include "./Style.h"
#include "./ThemeLevel.h"

#include <CoreLabs/string.h>

#include <unordered_map>



namespace fl7::gui {



/**
 * A named, cascading bundle of visual defaults: one theme-wide default level, plus
 * any number of face-type/role levels, keyed by the faces' own theme keys (e.g.,
 * "window", "button", etc.). A third, face-state level (e.g., "hover", "pressed",
 * etc.) is anticipated but not implemented yet.
 */
class Theme
{

public:
    Theme() = default;

    Theme(const Theme&) = delete;
    Theme& operator=(const Theme&) = delete;
    Theme(Theme&&) = delete;
    Theme& operator=(Theme&&) = delete;

    ~Theme() = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns the theme-wide default level (mutable, for building up a theme). */
    ThemeLevel& get_default_level() { return _default_level; }
    /** Returns the theme-wide default level. */
    const ThemeLevel& get_default_level() const { return _default_level; }

    /**
     * Returns the level for the specified face-type/role key, creating an initially
     * empty one if it doesn't exist yet (mutable, for building up a theme).
     */
    ThemeLevel& get_level(cl7::u8string_view key);



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
    Style resolve(cl7::u8string_view key) const;



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    ThemeLevel _default_level;

    std::unordered_map<cl7::u8string, ThemeLevel, cl7::string_hash<>, std::equal_to<>> _levels;

}; // class Theme



} // namespace fl7::gui

#endif // FL7_GUI_THEME_H
