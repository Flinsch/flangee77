
#include <TestLabs/TestSuite.h>

#include <FaceLabs/gui/Container.h>
#include <FaceLabs/gui/Shell.h>
#include <FaceLabs/gui/Theme.h>
#include <FaceLabs/gui/faces/Button.h>
#include <FaceLabs/gui/faces/Panel.h>

#include <XiaoLabs/graphics/Color.h>

#include "./shared.h"

#include <memory>



namespace tl7::internals {
    inline
    cl7::u8string to_string(const xl7::graphics::Color& color) { return u8"rgba(" + cl7::to_string(color.get_r()) + u8", " + cl7::to_string(color.get_g()) + u8", " + cl7::to_string(color.get_b()) + u8", " + cl7::to_string(color.get_a()) + u8")"; }
}



// #############################################################################
// Style override vs. theme resolution precedence (get_effective_style())
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  Face:  a face's own style override is returned directly from get_effective_style(), regardless of theme or shell; clearing it falls back to a default-constructed Style" )
{
    fl7::gui::faces::Panel panel; // no Shell at all

    fl7::gui::Style style_override;
    style_override.background_color = {1.0f, 0.0f, 0.0f, 1.0f};
    panel.set_style_override(style_override);

    TESTLABS_CHECK_EQ( panel.get_effective_style().background_color, style_override.background_color );

    panel.clear_style_override();
    TESTLABS_CHECK_EQ( panel.get_effective_style().background_color, fl7::gui::Style{}.background_color );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Face:  with no theme assigned to the shell, a themed face still falls through to the nearest ancestor's explicit style override, right through an anonymous (role-less) descendant in between" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard); // get_theme() stays nullptr

    auto& panel = shell.add_face<fl7::gui::faces::Panel>(); // themed ("panel")
    fl7::gui::Style style_override;
    style_override.background_color = {1.0f, 0.0f, 0.0f, 1.0f};
    panel.set_style_override(style_override);

    auto& container = panel.add_child<fl7::gui::Container>(); // no theme role of its own
    auto& button = container.add_child<fl7::gui::faces::Button>(U"Caption"); // themed ("button")

    TESTLABS_CHECK_EQ( container.get_effective_style().background_color, style_override.background_color );
    TESTLABS_CHECK_EQ( button.get_effective_style().background_color, style_override.background_color );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Face:  once the shell has a theme assigned, a themed descendant resolves via the theme instead, no longer inheriting the ancestor's override; an anonymous descendant still inherits it" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    fl7::gui::Theme theme; // deliberately left empty: no "button" level configured at all
    shell.set_theme(&theme);

    auto& panel = shell.add_face<fl7::gui::faces::Panel>();
    fl7::gui::Style style_override;
    style_override.background_color = {1.0f, 0.0f, 0.0f, 1.0f};
    panel.set_style_override(style_override);

    auto& container = panel.add_child<fl7::gui::Container>();
    auto& button = container.add_child<fl7::gui::faces::Button>(U"Caption");

    TESTLABS_CHECK_EQ( container.get_effective_style().background_color, style_override.background_color ); // still anonymous: still inherits

    // Resolved via the (empty) theme now, not inherited: falls all the way to
    // Style's own hard-coded default, NOT the Panel's override.
    TESTLABS_CHECK_EQ( button.get_effective_style().background_color, fl7::gui::Style{}.background_color );
}



// #############################################################################
// Hovered/pressed theme overlays tracking Shell's live interaction state
// #############################################################################

TESTLABS_CASE( u8"FaceLabs:  gui:  Face:  dragging off a pressed face cancels its visual hovered/pressed overlay (without losing the mouse capture itself); dragging back onto it reinstates it" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    const xl7::graphics::Color base_background{0.2f, 0.2f, 0.2f, 1.0f};
    const xl7::graphics::Color hovered_background{0.4f, 0.4f, 0.4f, 1.0f};
    const xl7::graphics::Color pressed_background{0.5f, 0.5f, 0.5f, 1.0f};

    fl7::gui::Theme theme;
    fl7::gui::ThemeLevel& level = theme.get_level(u8"button");
    level.background_color = base_background;
    level.hovered = std::make_unique<fl7::gui::ThemeLevel>();
    level.hovered->background_color = hovered_background;
    level.pressed = std::make_unique<fl7::gui::ThemeLevel>();
    level.pressed->background_color = pressed_background;
    shell.set_theme(&theme);

    auto& button = shell.add_face<fl7::gui::faces::Button>(U"Caption");
    button.set_position({0.0f, 0.0f});
    button.set_size({50.0f, 20.0f});

    TESTLABS_CHECK_EQ( button.get_effective_style().background_color, base_background );

    input.move_mouse_to({25.0f, 10.0f}); // hover
    shell.update();
    TESTLABS_CHECK_EQ( button.get_effective_style().background_color, hovered_background );

    input.set_button_down(xl7::input::MouseButton::Left, true); // press: now hovered AND pressed
    shell.update();
    TESTLABS_CHECK_EQ( button.get_effective_style().background_color, pressed_background );

    input.move_mouse_to({500.0f, 500.0f}); // drag off, button still held
    shell.update();
    TESTLABS_CHECK( shell.get_pressed_face() == static_cast<fl7::gui::Face*>(&button) ); // capture persists
    TESTLABS_CHECK_EQ( button.get_effective_style().background_color, base_background ); // visually neither hovered nor pressed anymore

    input.move_mouse_to({25.0f, 10.0f}); // drag back on, still held
    shell.update();
    TESTLABS_CHECK_EQ( button.get_effective_style().background_color, pressed_background ); // reinstated

    input.set_button_down(xl7::input::MouseButton::Left, false); // release, still hovering it
    shell.update();
    TESTLABS_CHECK_EQ( button.get_effective_style().background_color, hovered_background );
}

TESTLABS_CASE( u8"FaceLabs:  gui:  Face:  while a press is captured on one face, hovering a different face doesn't visually light that other face up either" )
{
    f77_tests::gui::NullRenderer renderer;
    f77_tests::gui::SimulatedInput input;
    fl7::gui::Shell shell(&renderer, &input.mouse, &input.keyboard);

    const xl7::graphics::Color base_background{0.2f, 0.2f, 0.2f, 1.0f};
    const xl7::graphics::Color hovered_background{0.4f, 0.4f, 0.4f, 1.0f};

    fl7::gui::Theme theme;
    fl7::gui::ThemeLevel& level = theme.get_level(u8"button");
    level.background_color = base_background;
    level.hovered = std::make_unique<fl7::gui::ThemeLevel>();
    level.hovered->background_color = hovered_background;
    shell.set_theme(&theme);

    auto& button_a = shell.add_face<fl7::gui::faces::Button>(U"A");
    button_a.set_position({0.0f, 0.0f});
    button_a.set_size({50.0f, 20.0f});
    auto& button_b = shell.add_face<fl7::gui::faces::Button>(U"B");
    button_b.set_position({100.0f, 0.0f});
    button_b.set_size({50.0f, 20.0f});

    input.move_mouse_to({25.0f, 10.0f}); // hover and press button_a
    shell.update();
    input.set_button_down(xl7::input::MouseButton::Left, true);
    shell.update();
    TESTLABS_CHECK_EQ( button_a.get_effective_style().background_color, hovered_background ); // still hovering it, held in place (delta == 0): no "pressed" overlay configured here

    input.move_mouse_to({125.0f, 10.0f}); // now hovering button_b, while button_a is still captured
    shell.update();

    TESTLABS_CHECK( shell.get_hovered_face() == static_cast<fl7::gui::Face*>(&button_b) );
    TESTLABS_CHECK( shell.get_pressed_face() == static_cast<fl7::gui::Face*>(&button_a) );
    TESTLABS_CHECK_EQ( button_a.get_effective_style().background_color, base_background ); // no longer hovered itself
    TESTLABS_CHECK_EQ( button_b.get_effective_style().background_color, base_background ); // hovered, but a press is captured elsewhere

    input.set_button_down(xl7::input::MouseButton::Left, false);
    shell.update();
}
