#ifndef F77_TESTS_FL7_GUI_SHARED_H
#define F77_TESTS_FL7_GUI_SHARED_H

#include "../../shared.h"

#include <FaceLabs/gui/render/AbstractRenderer.h>

#include <XiaoLabs/input/Key.h>
#include <XiaoLabs/input/Keyboard.h>
#include <XiaoLabs/input/KeyboardMouseSystem.h>
#include <XiaoLabs/input/Mouse.h>
#include <XiaoLabs/input/MouseButton.h>

#include <MathLabs/Vector2.h>

#include <array>
#include <chrono>



namespace f77_tests::gui {



    /**
     * A no-op AbstractRenderer: a test exercises input/interaction only, never
     * actual drawing, so every drawing primitive is a no-op.
     */
    class NullRenderer
        : public fl7::gui::render::AbstractRenderer
    {
    private:
        void _begin_frame_impl() override {}
        void _end_frame_impl() override {}

        void _render_faces_impl(const std::vector<std::unique_ptr<fl7::gui::Face>>& top_level_faces) override {}

        void _draw_rect_impl(ml7::Vector2f position_min, ml7::Vector2f position_max, const xl7::graphics::Color& color) override {}
        void _draw_textured_rect_impl(ml7::Vector2f position_min, ml7::Vector2f position_max, xl7::graphics::textures::Texture2D::Id texture_id, ml7::Vector2f uv_min, ml7::Vector2f uv_max, const xl7::graphics::Color& tint) override {}
        void _draw_text_impl(cl7::u32string_view text, fl7::fonts::Font* font, const fl7::fonts::TextStyle* text_style, ml7::Vector2f position) override {}
        void _draw_text_in_box_impl(cl7::u32string_view text, fl7::fonts::Font* font, const fl7::fonts::TextStyle* text_style, ml7::Vector2f box_position, ml7::Vector2f box_size) override {}

        void _push_clip_rect_impl(ml7::Vector2f clip_min, ml7::Vector2f clip_max) override {}
        void _pop_clip_rect_impl() override {}

    }; // class NullRenderer



    /**
     * Exists purely to re-expose KeyboardMouseSystem's protected static apply
     * bridge (meant for a backend's own device implementation, see
     * KeyboardMouseSystem) to test code, so a test can feed a locally-constructed
     * Mouse/Keyboard pair directly, without any real OS input. Never instantiated:
     * every member used here is static, so the (otherwise still abstract, since
     * its own backend hooks are never overridden here) class is never actually
     * constructed, only named.
     */
    class InputFeeder
        : public xl7::input::KeyboardMouseSystem
    {
    public:
        using KeyboardMouseSystem::_apply_mouse;
        using KeyboardMouseSystem::_apply_mouse_cursor_position;
        using KeyboardMouseSystem::_apply_keyboard;

    }; // class InputFeeder



    /**
     * A locally-constructed Mouse/Keyboard pair, fed via InputFeeder instead of
     * any real OS input, for a test to drive a Shell under test with. Each setter
     * call below applies immediately (not batched into some later "frame"
     * boundary): Shell::update() must be called in between two calls that are
     * meant to land in different frames (e.g., a button's down and up edge), same
     * as it would against real input.
     */
    class SimulatedInput
    {
    public:
        xl7::input::Mouse mouse;
        xl7::input::Keyboard keyboard;

        /** Moves the (simulated) mouse cursor to the specified absolute position, leaving every button as it was. */
        void move_mouse_to(ml7::Vector2f position)
        {
            InputFeeder::_apply_mouse_cursor_position(mouse, static_cast<int>(position.x), static_cast<int>(position.y));
        }

        /** Sets whether the specified mouse button is currently held, leaving the cursor position as it was. */
        void set_button_down(xl7::input::MouseButton button, bool down)
        {
            _button_levels[static_cast<size_t>(button)] = down;
            InputFeeder::_apply_mouse(mouse, _button_levels, 0, 0, 0, std::chrono::steady_clock::now());
        }

        /** Sets whether the specified key is currently held. */
        void set_key_down(xl7::input::Key key, bool down)
        {
            _key_levels[static_cast<size_t>(key)] = down;
            InputFeeder::_apply_keyboard(keyboard, _key_levels, U"", std::chrono::steady_clock::now());
        }

        /** Feeds this frame's mouse wheel movement (one notch/detent is 1), leaving every button and the cursor position as they were. */
        void scroll_wheel(int delta)
        {
            InputFeeder::_apply_mouse(mouse, _button_levels, 0, 0, delta, std::chrono::steady_clock::now());
        }

    private:
        std::array<bool, static_cast<size_t>(xl7::input::MouseButton::COUNT)> _button_levels{};
        std::array<bool, static_cast<size_t>(xl7::input::Key::COUNT)> _key_levels{};

    }; // class SimulatedInput



} // namespace f77_tests::gui

#endif // F77_TESTS_FL7_GUI_SHARED_H
