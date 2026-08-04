#include "KeyboardMouseSystem.h"

#include "./impl/rawinput/KeyboardMouseSystemImpl.h"

#include <CoreLabs/logging.h>

#include <chrono>



namespace xl7::input {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    KeyboardMouseSystem::KeyboardMouseSystem()
        : _aggregated_keyboard(std::make_unique<Keyboard>())
        , _aggregated_mouse(std::make_unique<Mouse>())
    {
    }



    // #############################################################################
    // Factory Function
    // #############################################################################

    KeyboardMouseSystem* KeyboardMouseSystem::factory_func()
    {
        return backend_registry().resolve(config_provider().get_config().input.keyboard_mouse_driver_name);
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Refreshes the aggregate keyboard/mouse from the current state of all
     * individual devices.
     */
    void KeyboardMouseSystem::update()
    {
        const auto now = std::chrono::steady_clock::now();

        std::array<bool, static_cast<size_t>(Key::COUNT)> aggregated_keys{};
        for (const auto& entry : _keyboards)
            for (size_t i = 0; i < aggregated_keys.size(); ++i)
                aggregated_keys[i] = aggregated_keys[i] || entry.device->is_key_down(static_cast<Key>(i));

        _apply_keyboard(*_aggregated_keyboard, aggregated_keys, _pending_text_input, now);
        _pending_text_input.clear();

        // There's only one real OS cursor, regardless of how many physical mice
        // are plugged in feeding it: queried once here, then applied identically
        // to every individual mouse below and to the aggregate, independent of
        // each device's own (unrelated, raw) deltas.
        const std::optional<CursorPosition> cursor_position = _query_cursor_position_impl();

        std::array<bool, static_cast<size_t>(MouseButton::COUNT)> aggregated_buttons{};
        int aggregated_delta_x = 0;
        int aggregated_delta_y = 0;
        int aggregated_wheel_delta = 0;
        for (const auto& entry : _mice)
        {
            for (size_t i = 0; i < aggregated_buttons.size(); ++i)
                aggregated_buttons[i] = aggregated_buttons[i] || entry.device->is_button_down(static_cast<MouseButton>(i));
            aggregated_delta_x += entry.device->get_delta_x();
            aggregated_delta_y += entry.device->get_delta_y();
            aggregated_wheel_delta += entry.device->get_wheel_delta();

            if (cursor_position)
                _apply_mouse_cursor_position(*entry.device, cursor_position->x, cursor_position->y);
        }

        _apply_mouse(*_aggregated_mouse, aggregated_buttons, aggregated_delta_x, aggregated_delta_y, aggregated_wheel_delta, now);
        if (cursor_position)
            _apply_mouse_cursor_position(*_aggregated_mouse, cursor_position->x, cursor_position->y);
    }



    // #############################################################################
    // Backend Interface
    // #############################################################################

    /**
     * Registers a newly detected keyboard and returns the (initially empty)
     * instance for the backend to keep feeding.
     */
    Keyboard& KeyboardMouseSystem::_add_keyboard(DeviceId id)
    {
        _keyboards.push_back({.id = id, .device = std::make_unique<Keyboard>()});
        return *_keyboards.back().device;
    }

    /**
     * Unregisters a keyboard that has been disconnected.
     */
    void KeyboardMouseSystem::_remove_keyboard(DeviceId id)
    {
        std::erase_if(_keyboards, [id](const KeyboardEntry& entry) { return entry.id == id; });
    }

    /**
     * Registers a newly detected mouse and returns the (initially empty)
     * instance for the backend to keep feeding.
     */
    Mouse& KeyboardMouseSystem::_add_mouse(DeviceId id)
    {
        _mice.push_back({.id = id, .device = std::make_unique<Mouse>()});
        return *_mice.back().device;
    }

    /**
     * Unregisters a mouse that has been disconnected.
     */
    void KeyboardMouseSystem::_remove_mouse(DeviceId id)
    {
        std::erase_if(_mice, [id](const MouseEntry& entry) { return entry.id == id; });
    }

    /**
     * Feeds newly observed levels/deltas into an individual keyboard/mouse
     * instance. A protected bridge rather than direct Keyboard::Attorney /
     * Mouse::Attorney access, because backends derive from this class, and
     * friendship (granted to KeyboardMouseSystem itself) isn't inherited by
     * subclasses.
     */
    void KeyboardMouseSystem::_apply_keyboard(Keyboard& keyboard, const std::array<bool, static_cast<size_t>(Key::COUNT)>& key_values, const cl7::u32string& text_input_delta, std::chrono::steady_clock::time_point now)
    {
        Keyboard::Attorney::apply(keyboard, key_values, text_input_delta, now);
    }

    /**
     * Feeds newly observed levels/deltas into an individual mouse instance.
     * See _apply_keyboard() for why this bridge exists.
     */
    void KeyboardMouseSystem::_apply_mouse(Mouse& mouse, const std::array<bool, static_cast<size_t>(MouseButton::COUNT)>& button_values, int delta_x, int delta_y, int wheel_delta, std::chrono::steady_clock::time_point now)
    {
        Mouse::Attorney::apply(mouse, button_values, delta_x, delta_y, wheel_delta, now);
    }

    /**
     * Feeds a newly observed absolute cursor position into an individual mouse
     * instance. See _apply_keyboard() for why this bridge exists.
     */
    void KeyboardMouseSystem::_apply_mouse_cursor_position(Mouse& mouse, int x, int y)
    {
        Mouse::Attorney::apply_cursor_position(mouse, x, y);
    }

    /**
     * Queues composed text input to be reported by the aggregate keyboard on the
     * next update() (see aggregated_keyboard()'s docs for why this isn't attributed
     * to an individual keyboard). UTF-32, per the framework's "user interaction"
     * convention, see Keyboard::get_text_input().
     */
    void KeyboardMouseSystem::_queue_text_input(const cl7::u32string& text)
    {
        _pending_text_input += text;
    }



    // #############################################################################
    // Component Implementations
    // #############################################################################

    /**
     * Initializes the component.
     */
    bool KeyboardMouseSystem::_init()
    {
        const bool result = _init_impl();

        if (result)
            LOG_SUCCESS(u8"The keyboard/mouse input component based on " + cl7::u8string{get_driver_name()} + u8" has been successfully initialized.");
        else
            LOG_ERROR(u8"The keyboard/mouse input component based on " + cl7::u8string{get_driver_name()} + u8" could not be initialized.");

        return result;
    }

    /**
     * De-initializes the component.
     */
    bool KeyboardMouseSystem::_shutdown()
    {
        const bool result = _shutdown_impl();

        if (result)
            LOG_SUCCESS(u8"The keyboard/mouse input component based on " + cl7::u8string{get_driver_name()} + u8" has been shut down successfully.");
        else
            LOG_WARNING(u8"The keyboard/mouse input component based on " + cl7::u8string{get_driver_name()} + u8" could not be shut down correctly.");

        return result;
    }



} // namespace xl7::input
