#ifndef XL7_INPUT_KEYBOARD_H
#define XL7_INPUT_KEYBOARD_H

#include "./DigitalState.h"
#include "./Key.h"

#include <CoreLabs/string.h>

#include <array>
#include <chrono>



namespace xl7::input {



/**
 * Represents a single keyboard's state (or the aggregate of all connected
 * keyboards, which is fed the same way, just with OR'd values instead of a
 * single physical device's raw values).
 */
class Keyboard
{

public:
    /**
     * Grants KeyboardMouseSystem exclusive access to feed new state into a
     * Keyboard instance, keeping that (backend-facing) capability out of the
     * public query API.
     */
    class Attorney
    {
        static void apply(Keyboard& keyboard, const std::array<bool, static_cast<size_t>(Key::COUNT)>& key_values, const cl7::u32string& text_input_delta, std::chrono::steady_clock::time_point now) { keyboard._apply(key_values, text_input_delta, now); }
        friend class KeyboardMouseSystem;
    };



    Keyboard() = default;

    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;
    Keyboard(Keyboard&&) = delete;
    Keyboard& operator=(Keyboard&&) = delete;

    ~Keyboard() = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the state of the specified key.
     */
    const DigitalState& get_button(Key key) const { return _keys[static_cast<size_t>(key)]; }

    /**
     * Returns whether the specified key is currently held down.
     */
    bool is_key_down(Key key) const { return _keys[static_cast<size_t>(key)].is_down(); }

    /**
     * Returns whether the specified key was pressed down during this frame.
     */
    bool is_key_pressed(Key key) const { return _keys[static_cast<size_t>(key)].is_pressed(); }

    /**
     * Returns whether the specified key was released during this frame.
     */
    bool is_key_released(Key key) const { return _keys[static_cast<size_t>(key)].is_released(); }

    /**
     * Returns whether the specified keys was released during this frame, and the
     * down/up cycle it completed fit within the specified time window.
     */
    bool is_key_tapped(Key key, std::chrono::milliseconds time_window = DigitalState::DEFAULT_TAP_TIME_WINDOW) const { return _keys[static_cast<size_t>(key)].is_tapped(_now, time_window); }

    /**
     * Returns the composed text input received during this frame. Unlike the key
     * state above, this already reflects keyboard-layout translation and
     * dead-key/IME composition (fed from WM_CHAR on Windows, for example, not from
     * the raw key codes), so it's what a GUI text field should consume.
     *
     * UTF-32, per the framework's "user interaction" convention, so callers get
     * 1-to-1 code-point indexing for free (e.g., for cursor movement, selection,
     * backspace, glyph lookup, etc.).
     *
     * Note: WM_CHAR isn't attributable to a specific source keyboard, so in
     * practice only the aggregate keyboard's text input is ever populated,
     * individual keyboards' text input stays empty.
     */
    const cl7::u32string& get_text_input() const { return _text_input; }



private:

    // #############################################################################
    // Backend Interface
    // #############################################################################

    /**
     * Applies the newly observed values/text for this frame.
     */
    void _apply(const std::array<bool, static_cast<size_t>(Key::COUNT)>& key_values, const cl7::u32string& text_input_delta, std::chrono::steady_clock::time_point now);



    // #############################################################################
    // Attributes
    // #############################################################################

    std::array<DigitalState, static_cast<size_t>(Key::COUNT)> _keys{};

    cl7::u32string _text_input;

    std::chrono::steady_clock::time_point _now;

}; // class Keyboard



} // namespace xl7::input

#endif // XL7_INPUT_KEYBOARD_H
