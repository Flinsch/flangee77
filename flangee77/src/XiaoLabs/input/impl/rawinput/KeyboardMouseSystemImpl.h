#ifndef XL7_INPUT_IMPL_RAWINPUT_KEYBOARDMOUSESYSTEMIMPL_H
#define XL7_INPUT_IMPL_RAWINPUT_KEYBOARDMOUSESYSTEMIMPL_H
#include "../../KeyboardMouseSystem.h"
#if F77_IS_WINDOWS

#include <CoreLabs/string.h>

#include <windows.h>

#include <array>
#include <vector>



namespace xl7::input::impl::rawinput {



class KeyboardMouseSystemImpl final
    : public KeyboardMouseSystem
{
    struct Registrar
    {
        Registrar() { backend_registry().register_backend(DRIVER_NAME, DRIVER_ALIASES, factory_func, 1); }
        static KeyboardMouseSystem* factory_func() { return new KeyboardMouseSystemImpl(); }
    } static inline const _registrar = {};

public:
    static constexpr cl7::u8string_view DRIVER_NAME = u8"Raw Input";
    static constexpr std::initializer_list<cl7::u8string_view> DRIVER_ALIASES = {u8"RawInput", u8"Raw Input", u8"RI"};



    KeyboardMouseSystemImpl(const KeyboardMouseSystemImpl&) = delete;
    KeyboardMouseSystemImpl& operator=(const KeyboardMouseSystemImpl&) = delete;
    KeyboardMouseSystemImpl(KeyboardMouseSystemImpl&&) = delete;
    KeyboardMouseSystemImpl& operator=(KeyboardMouseSystemImpl&&) = delete;



    /**
     * Returns the name of the driver/implementation.
     */
    cl7::u8string_view get_driver_name() const override { return DRIVER_NAME; }



    /**
     * Handles a window message that may carry raw keyboard/mouse input (WM_INPUT,
     * WM_INPUT_DEVICE_CHANGE, WM_CHAR). Called by WindowImpl::wnd_proc for the
     * active keyboard/mouse backend.
     */
    void handle_window_message(UINT msg, WPARAM wparam, LPARAM lparam);



protected:
    KeyboardMouseSystemImpl() = default;
    ~KeyboardMouseSystemImpl() override = default;



private:

    // #############################################################################
    // Types
    // #############################################################################

    struct KeyboardDeviceState
    {
        DeviceId id = 0;
        Keyboard* keyboard = nullptr;
        std::array<bool, static_cast<size_t>(Key::COUNT)> key_values{};
    };

    struct MouseDeviceState
    {
        DeviceId id = 0;
        Mouse* mouse = nullptr;
        std::array<bool, static_cast<size_t>(MouseButton::COUNT)> button_values{};
    };



    // #############################################################################
    // KeyboardMouseSystem Implementations
    // #############################################################################

    /**
     * Registers for raw keyboard/mouse input and enumerates already-connected
     * devices.
     */
    bool _init_impl() override;

    /**
     * Unregisters raw keyboard/mouse input.
     */
    bool _shutdown_impl() override;

    /**
     * Queries the real cursor position via GetCursorPos() plus ScreenToClient(),
     * independent of Raw Input's own (relative-only) mouse deltas. See
     * KeyboardMouseSystem::_query_cursor_position_impl().
     */
    std::optional<CursorPosition> _query_cursor_position_impl() override;



    // #############################################################################
    // Message Handlers
    // #############################################################################

    /**
     * Handles WM_INPUT: reads the RAWINPUT payload and dispatches it to the
     * keyboard/mouse handler.
     */
    void _handle_input_message(LPARAM lparam);

    /**
     * Handles WM_INPUT_DEVICE_CHANGE: registers/unregisters the arriving or
     * departing device (hotplug).
     */
    void _handle_input_device_change_message(WPARAM wparam, LPARAM lparam);

    /**
     * Handles WM_CHAR: buffers the UTF-16 code unit and, once a complete
     * (non-dangling) sequence is available, converts and queues it as composed text
     * input on the aggregate keyboard.
     */
    void _handle_char_message(WPARAM wparam);

    /**
     * Applies a raw keyboard input report to the corresponding device.
     */
    void _handle_raw_keyboard(HANDLE device, const RAWKEYBOARD& raw_keyboard);

    /**
     * Applies a raw mouse input report to the corresponding device.
     */
    void _handle_raw_mouse(HANDLE device, const RAWMOUSE& raw_mouse);



    // #############################################################################
    // Device Registration
    // #############################################################################

    /**
     * Registers for WM_INPUT/WM_INPUT_DEVICE_CHANGE for the generic keyboard
     * and mouse usages.
     */
    static bool _register_devices();

    /**
     * Unregisters raw input.
     */
    static bool _unregister_devices();

    /**
     * Registers all currently connected raw input devices.
     */
    void _enumerate_existing_devices();

    /**
     * Determines the type of the specified (newly seen) device and registers it as
     * a keyboard or mouse, if applicable (anything else, e.g. HID game controllers,
     * is ignored, those are handled via XInput or whatever).
     */
    void _register_device(HANDLE device);

    /**
     * Unregisters the specified device, wherever it's currently tracked.
     */
    void _remove_device(HANDLE device);

    /**
     * Returns the existing device state for the specified handle, or registers and
     * returns a new one.
     */
    KeyboardDeviceState& _find_or_add_keyboard_device(HANDLE device);

    /**
     * Returns the existing device state for the specified handle, or registers and
     * returns a new one.
     */
    MouseDeviceState& _find_or_add_mouse_device(HANDLE device);

    /**
     * Derives our opaque DeviceId from a raw input device handle.
     */
    static DeviceId _device_id_from_handle(HANDLE device);

    /**
     * Maps a raw keyboard input report to our backend-agnostic Key, resolving the
     * left/right ambiguity Raw Input leaves for Ctrl/Alt/Shift.
     */
    static Key _key_from_raw_keyboard(const RAWKEYBOARD& raw_keyboard);



    // #############################################################################
    // Attributes
    // #############################################################################

    std::vector<KeyboardDeviceState> _keyboard_devices;
    std::vector<MouseDeviceState> _mouse_devices;

    /** Buffers WM_CHAR code units until a complete UTF-16 sequence is available. */
    cl7::u16string _pending_text_input_utf16;

}; // class KeyboardMouseSystemImpl



} // namespace xl7::input::impl::rawinput



#endif // F77_IS_WINDOWS
#endif // XL7_INPUT_IMPL_RAWINPUT_KEYBOARDMOUSESYSTEMIMPL_H
