
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/NineSliceChrome.h>
#include <FaceLabs/gui/Theme.h>

#include <XiaoLabs/graphics/Color.h>

#include <memory>



namespace tl7::internals {
    inline
    cl7::u8string to_string(const xl7::graphics::Color& color) { return u8"rgba(" + cl7::to_string(color.get_r()) + u8", " + cl7::to_string(color.get_g()) + u8", " + cl7::to_string(color.get_b()) + u8", " + cl7::to_string(color.get_a()) + u8")"; }
}



TESTLABS_CASE( u8"FaceLabs:  gui:  Theme:  resolve() on an unknown key falls through to the theme-wide default level, then to Style's own hard-coded defaults, field by field" )
{
    fl7::gui::Theme theme;

    const fl7::gui::Style defaults{};
    const fl7::gui::Style resolved_empty = theme.resolve(u8"unknown", {});
    TESTLABS_CHECK( resolved_empty.chrome == nullptr );
    TESTLABS_CHECK( resolved_empty.font == nullptr );
    TESTLABS_CHECK_EQ( resolved_empty.background_color, defaults.background_color );
    TESTLABS_CHECK_EQ( resolved_empty.border_color, defaults.border_color );
    TESTLABS_CHECK_EQ( resolved_empty.border_width, defaults.border_width );

    const xl7::graphics::Color custom_background{1.0f, 0.0f, 0.0f, 1.0f};
    theme.get_default_level().background_color = custom_background;

    const fl7::gui::Style resolved = theme.resolve(u8"unknown", {});
    TESTLABS_CHECK_EQ( resolved.background_color, custom_background ); // falls through to the theme-wide default
    TESTLABS_CHECK_EQ( resolved.border_color, defaults.border_color ); // still untouched: Style's own hard-coded default
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Theme:  a key's own level overrides the theme-wide default field by field; chrome only ever comes from the key's own level, never the default level" )
{
    fl7::gui::Theme theme;

    const fl7::gui::NineSliceChrome default_chrome{};
    const fl7::gui::NineSliceChrome button_chrome{};

    const xl7::graphics::Color default_background{0.1f, 0.1f, 0.1f, 1.0f};
    const xl7::graphics::Color button_background{0.9f, 0.9f, 0.9f, 1.0f};
    const xl7::graphics::Color default_border{0.5f, 0.5f, 0.5f, 1.0f};

    fl7::gui::ThemeLevel& default_level = theme.get_default_level();
    default_level.chrome = &default_chrome;
    default_level.background_color = default_background;
    default_level.border_color = default_border;

    fl7::gui::ThemeLevel& button_level = theme.get_level(u8"button");
    button_level.background_color = button_background; // overrides the default level's background_color
    // border_color left unset: must fall through to the default level's.

    fl7::gui::Style resolved = theme.resolve(u8"button", {});
    TESTLABS_CHECK_EQ( resolved.background_color, button_background );
    TESTLABS_CHECK_EQ( resolved.border_color, default_border ); // fell through, since the button level doesn't set it
    TESTLABS_CHECK( resolved.chrome == nullptr ); // NOT the default level's chrome: a key's own level is the only source

    button_level.chrome = &button_chrome;
    resolved = theme.resolve(u8"button", {});
    TESTLABS_CHECK( resolved.chrome == &button_chrome );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Theme:  checked/hovered/pressed/focused overlays apply only while their state flag is active, each overwriting only the fields it actually sets, in that fixed priority order" )
{
    fl7::gui::Theme theme;

    const xl7::graphics::Color base_background{0.2f, 0.2f, 0.2f, 1.0f};
    const xl7::graphics::Color checked_background{0.3f, 0.3f, 0.3f, 1.0f};
    const xl7::graphics::Color hovered_background{0.4f, 0.4f, 0.4f, 1.0f};
    const xl7::graphics::Color pressed_background{0.5f, 0.5f, 0.5f, 1.0f};
    const xl7::graphics::Color focused_background{0.6f, 0.6f, 0.6f, 1.0f};
    const xl7::graphics::Color hovered_border{0.0f, 1.0f, 0.0f, 1.0f};

    fl7::gui::ThemeLevel& level = theme.get_level(u8"checkbox");
    level.background_color = base_background;

    level.checked = std::make_unique<fl7::gui::ThemeLevel>();
    level.checked->background_color = checked_background;

    level.hovered = std::make_unique<fl7::gui::ThemeLevel>();
    level.hovered->background_color = hovered_background;
    level.hovered->border_color = hovered_border; // a field the other overlays don't touch

    level.pressed = std::make_unique<fl7::gui::ThemeLevel>();
    level.pressed->background_color = pressed_background;

    level.focused = std::make_unique<fl7::gui::ThemeLevel>();
    level.focused->background_color = focused_background;

    // Nothing active: plain base level.
    fl7::gui::Style resolved = theme.resolve(u8"checkbox", {});
    TESTLABS_CHECK_EQ( resolved.background_color, base_background );

    // Only checked.
    resolved = theme.resolve(u8"checkbox", { .checked = true });
    TESTLABS_CHECK_EQ( resolved.background_color, checked_background );

    // Only hovered: its own field changes, but it doesn't clobber background_color
    // back to the base either way (irrelevant here since checked is off), and its
    // border_color DOES show up since nothing else sets that field.
    resolved = theme.resolve(u8"checkbox", { .hovered = true });
    TESTLABS_CHECK_EQ( resolved.background_color, hovered_background );
    TESTLABS_CHECK_EQ( resolved.border_color, hovered_border );

    // checked + hovered: hovered comes later in the fixed order, so it wins the
    // field they both set (background_color), while its border_color still shows
    // up untouched by the (background_color-only) checked overlay.
    resolved = theme.resolve(u8"checkbox", { .checked = true, .hovered = true });
    TESTLABS_CHECK_EQ( resolved.background_color, hovered_background );
    TESTLABS_CHECK_EQ( resolved.border_color, hovered_border );

    // + pressed: later still, wins background_color over hovered; border_color
    // (which pressed doesn't set) is left as hovered's.
    resolved = theme.resolve(u8"checkbox", { .checked = true, .hovered = true, .pressed = true });
    TESTLABS_CHECK_EQ( resolved.background_color, pressed_background );
    TESTLABS_CHECK_EQ( resolved.border_color, hovered_border );

    // + focused: last in the order, wins over all the others for background_color.
    resolved = theme.resolve(u8"checkbox", { .checked = true, .hovered = true, .pressed = true, .focused = true });
    TESTLABS_CHECK_EQ( resolved.background_color, focused_background );
}
