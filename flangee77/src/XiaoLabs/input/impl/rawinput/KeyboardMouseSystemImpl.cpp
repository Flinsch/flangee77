#include "KeyboardMouseSystemImpl.h"
#if F77_IS_WINDOWS

#include <XiaoLabs/MainWindow.h>

#include <CoreLabs/logging.h>
#include <CoreLabs/platform/errors.h>
#include <CoreLabs/text/codec.h>

#include <algorithm>
#include <bit>
#include <chrono>



namespace xl7::input::impl::rawinput {



    // #############################################################################
    // Message Dispatch
    // #############################################################################

    /**
     * Handles a window message that may carry raw keyboard/mouse input (WM_INPUT,
     * WM_INPUT_DEVICE_CHANGE, WM_CHAR). Called by WindowImpl::wnd_proc for the
     * active keyboard/mouse backend.
     */
    void KeyboardMouseSystemImpl::handle_window_message(UINT msg, WPARAM wparam, LPARAM lparam)
    {
        switch (msg)
        {

        case WM_INPUT:
            _handle_input_message(lparam);
            break;

        case WM_INPUT_DEVICE_CHANGE:
            _handle_input_device_change_message(wparam, lparam);
            break;

        case WM_CHAR:
            _handle_char_message(wparam);
            break;

        default:
            break;

        } // switch (msg)
    }



    // #############################################################################
    // KeyboardMouseSystem Implementations
    // #############################################################################

    /**
     * Registers for raw keyboard/mouse input and enumerates already-connected
     * devices.
     */
    bool KeyboardMouseSystemImpl::_init_impl()
    {
        if (!_register_devices())
            return false;

        _enumerate_existing_devices();

        return true;
    }

    /**
     * Unregisters raw keyboard/mouse input.
     */
    bool KeyboardMouseSystemImpl::_shutdown_impl()
    {
        for (const auto& entry : _keyboard_devices)
            _remove_keyboard(entry.id);
        _keyboard_devices.clear();

        for (const auto& entry : _mouse_devices)
            _remove_mouse(entry.id);
        _mouse_devices.clear();

        return _unregister_devices();
    }

    /**
     * Queries the real cursor position via GetCursorPos() plus ScreenToClient(),
     * independent of Raw Input's own (relative-only) mouse deltas. See
     * KeyboardMouseSystem::_query_cursor_position_impl().
     */
    std::optional<KeyboardMouseSystem::CursorPosition> KeyboardMouseSystemImpl::_query_cursor_position_impl()
    {
        POINT point;
        if (!::GetCursorPos(&point))
            return std::nullopt;

        const HWND hwnd = static_cast<HWND>(MainWindow::instance().get_handle());
        if (!::ScreenToClient(hwnd, &point))
            return std::nullopt;

        return CursorPosition{.x = point.x, .y = point.y};
    }



    // #############################################################################
    // Message Handlers
    // #############################################################################

    /**
     * Handles WM_INPUT: reads the RAWINPUT payload and dispatches it to the
     * keyboard/mouse handler.
     */
    void KeyboardMouseSystemImpl::_handle_input_message(LPARAM lparam)
    {
        UINT size = 0;
        ::GetRawInputData(std::bit_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
        if (size == 0 || size > sizeof(RAWINPUT))
            return;

        alignas(RAWINPUT) BYTE buffer[sizeof(RAWINPUT)];
        if (::GetRawInputData(std::bit_cast<HRAWINPUT>(lparam), RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER)) != size)
            return;

        // GetRawInputData() fills `buffer` with a RAWINPUT-compatible byte layout
        // by contract, reinterpreting it is the documented way to read the result
        // (bit_cast doesn't apply here: we're reinterpreting an address, not
        // converting a same-sized value).
        const auto& raw_input = *reinterpret_cast<const RAWINPUT*>(buffer);

        switch (raw_input.header.dwType)
        {

        case RIM_TYPEKEYBOARD:
            _handle_raw_keyboard(raw_input.header.hDevice, raw_input.data.keyboard);
            break;

        case RIM_TYPEMOUSE:
            _handle_raw_mouse(raw_input.header.hDevice, raw_input.data.mouse);
            break;

        default:
            break;

        } // switch (raw_input.header.dwType)
    }

    /**
     * Handles WM_INPUT_DEVICE_CHANGE: registers/unregisters the arriving or
     * departing device (hotplug).
     */
    void KeyboardMouseSystemImpl::_handle_input_device_change_message(WPARAM wparam, LPARAM lparam)
    {
        const auto device = std::bit_cast<HANDLE>(lparam);

        if (wparam == GIDC_ARRIVAL)
            _register_device(device);
        else if (wparam == GIDC_REMOVAL)
            _remove_device(device);
    }

    /**
     * Handles WM_CHAR: buffers the UTF-16 code unit and, once a complete
     * (non-dangling) sequence is available, converts and queues it as composed text
     * input on the aggregate keyboard.
     */
    void KeyboardMouseSystemImpl::_handle_char_message(WPARAM wparam)
    {
        _pending_text_input_utf16 += static_cast<char16_t>(wparam);

        // Don't flush while the buffer ends in an unpaired leading surrogate,
        // wait for its matching trailing surrogate to arrive with the next
        // WM_CHAR, so that we never pass a dangling half-pair.
        const char16_t last_unit = _pending_text_input_utf16.back();
        if (last_unit >= 0xD800 && last_unit <= 0xDBFF)
            return;

        _queue_text_input(cl7::text::codec::to_utf32(_pending_text_input_utf16));
        _pending_text_input_utf16.clear();
    }

    /**
     * Applies a raw keyboard input report to the corresponding device.
     */
    void KeyboardMouseSystemImpl::_handle_raw_keyboard(HANDLE device, const RAWKEYBOARD& raw_keyboard)
    {
        // Some devices send a sentinel "overrun" entry with this VKey. Ignore it.
        if (raw_keyboard.VKey == 0xFF)
            return;

        const Key key = _key_from_raw_keyboard(raw_keyboard);
        if (key == Key::Unknown)
            return;

        KeyboardDeviceState& state = _find_or_add_keyboard_device(device);
        state.key_values[static_cast<size_t>(key)] = (raw_keyboard.Flags & RI_KEY_BREAK) == 0;

        _apply_keyboard(*state.keyboard, state.key_values, cl7::u32string{}, std::chrono::steady_clock::now());
    }

    /**
     * Applies a raw mouse input report to the corresponding device.
     */
    void KeyboardMouseSystemImpl::_handle_raw_mouse(HANDLE device, const RAWMOUSE& raw_mouse)
    {
        MouseDeviceState& state = _find_or_add_mouse_device(device);

        if (raw_mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)   state.button_values[static_cast<size_t>(MouseButton::Left)] = true;
        if (raw_mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)     state.button_values[static_cast<size_t>(MouseButton::Left)] = false;
        if (raw_mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)  state.button_values[static_cast<size_t>(MouseButton::Right)] = true;
        if (raw_mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)    state.button_values[static_cast<size_t>(MouseButton::Right)] = false;
        if (raw_mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) state.button_values[static_cast<size_t>(MouseButton::Middle)] = true;
        if (raw_mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)   state.button_values[static_cast<size_t>(MouseButton::Middle)] = false;
        if (raw_mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN)      state.button_values[static_cast<size_t>(MouseButton::X1)] = true;
        if (raw_mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP)        state.button_values[static_cast<size_t>(MouseButton::X1)] = false;
        if (raw_mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN)      state.button_values[static_cast<size_t>(MouseButton::X2)] = true;
        if (raw_mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP)        state.button_values[static_cast<size_t>(MouseButton::X2)] = false;

        // Absolute-positioning devices (rare: VMs/RDP/tablets) are
        // not handled in this pass: their movement is silently dropped
        // rather than misread as a (huge) relative delta.
        int delta_x = 0;
        int delta_y = 0;
        if ((raw_mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == 0)
        {
            delta_x = raw_mouse.lLastX;
            delta_y = raw_mouse.lLastY;
        }

        int wheel_delta = 0;
        if (raw_mouse.usButtonFlags & RI_MOUSE_WHEEL)
            wheel_delta = static_cast<SHORT>(raw_mouse.usButtonData) / WHEEL_DELTA;

        _apply_mouse(*state.mouse, state.button_values, delta_x, delta_y, wheel_delta, std::chrono::steady_clock::now());
    }



    // #############################################################################
    // Device Registration
    // #############################################################################

    /**
     * Registers for WM_INPUT/WM_INPUT_DEVICE_CHANGE for the generic keyboard
     * and mouse usages.
     */
    bool KeyboardMouseSystemImpl::_register_devices()
    {
        const HWND hwnd = static_cast<HWND>(MainWindow::instance().get_handle());

        RAWINPUTDEVICE devices[2]{};

        devices[0].usUsagePage = 0x01; // Generic Desktop Controls
        devices[0].usUsage = 0x06;     // Keyboard
        devices[0].dwFlags = RIDEV_DEVNOTIFY;
        devices[0].hwndTarget = hwnd;

        devices[1].usUsagePage = 0x01; // Generic Desktop Controls
        devices[1].usUsage = 0x02;     // Mouse
        devices[1].dwFlags = RIDEV_DEVNOTIFY;
        devices[1].hwndTarget = hwnd;

        if (!::RegisterRawInputDevices(devices, static_cast<UINT>(std::size(devices)), sizeof(RAWINPUTDEVICE)))
        {
            LOG_ERROR(cl7::platform::errors::system_result(static_cast<int>(::GetLastError()), u8"::RegisterRawInputDevices"));
            return false;
        }

        LOG(u8"Registered for raw keyboard/mouse input.");
        return true;
    }

    /**
     * Unregisters raw input.
     */
    bool KeyboardMouseSystemImpl::_unregister_devices()
    {
        RAWINPUTDEVICE devices[2]{};

        devices[0].usUsagePage = 0x01;
        devices[0].usUsage = 0x06;
        devices[0].dwFlags = RIDEV_REMOVE;
        devices[0].hwndTarget = nullptr;

        devices[1].usUsagePage = 0x01;
        devices[1].usUsage = 0x02;
        devices[1].dwFlags = RIDEV_REMOVE;
        devices[1].hwndTarget = nullptr;

        if (!::RegisterRawInputDevices(devices, static_cast<UINT>(std::size(devices)), sizeof(RAWINPUTDEVICE)))
        {
            LOG_WARNING(cl7::platform::errors::system_result(static_cast<int>(::GetLastError()), u8"::RegisterRawInputDevices"));
            return false;
        }

        LOG(u8"Unregistered raw keyboard/mouse input.");
        return true;
    }

    /**
     * Registers all currently connected raw input devices.
     */
    void KeyboardMouseSystemImpl::_enumerate_existing_devices()
    {
        UINT device_count = 0;
        if (::GetRawInputDeviceList(nullptr, &device_count, sizeof(RAWINPUTDEVICELIST)) != 0 || device_count == 0)
            return;

        std::vector<RAWINPUTDEVICELIST> device_list(device_count);
        const UINT result = ::GetRawInputDeviceList(device_list.data(), &device_count, sizeof(RAWINPUTDEVICELIST));
        if (result == static_cast<UINT>(-1))
        {
            LOG_WARNING(cl7::platform::errors::system_result(static_cast<int>(::GetLastError()), u8"::GetRawInputDeviceList"));
            return;
        }

        for (UINT i = 0; i < result; ++i)
            _register_device(device_list[i].hDevice);
    }

    /**
     * Determines the type of the specified (newly seen) device and registers it as
     * a keyboard or mouse, if applicable (anything else, e.g. HID game controllers,
     * is ignored, those are handled via XInput or whatever).
     */
    void KeyboardMouseSystemImpl::_register_device(HANDLE device)
    {
        RID_DEVICE_INFO device_info{};
        device_info.cbSize = sizeof(device_info);
        UINT size = sizeof(device_info);

        if (::GetRawInputDeviceInfoW(device, RIDI_DEVICEINFO, &device_info, &size) == static_cast<UINT>(-1))
            return;

        if (device_info.dwType == RIM_TYPEKEYBOARD)
            _find_or_add_keyboard_device(device);
        else if (device_info.dwType == RIM_TYPEMOUSE)
            _find_or_add_mouse_device(device);
        // RIM_TYPEHID (e.g., game controllers) is ignored,
        // those are handled via XInput or whatever.
    }

    /**
     * Unregisters the specified device, wherever it's currently tracked.
     */
    void KeyboardMouseSystemImpl::_remove_device(HANDLE device)
    {
        const DeviceId id = _device_id_from_handle(device);

        if (std::erase_if(_keyboard_devices, [id](const KeyboardDeviceState& entry) { return entry.id == id; }) > 0)
        {
            _remove_keyboard(id);
            return;
        }

        if (std::erase_if(_mouse_devices, [id](const MouseDeviceState& entry) { return entry.id == id; }) > 0)
            _remove_mouse(id);
    }

    /**
     * Returns the existing device state for the specified handle, or registers and
     * returns a new one.
     */
    KeyboardMouseSystemImpl::KeyboardDeviceState& KeyboardMouseSystemImpl::_find_or_add_keyboard_device(HANDLE device)
    {
        const DeviceId id = _device_id_from_handle(device);

        for (auto& entry : _keyboard_devices)
            if (entry.id == id)
                return entry;

        _keyboard_devices.push_back({.id = id, .keyboard = &_add_keyboard(id)});
        return _keyboard_devices.back();
    }

    /**
     * Returns the existing device state for the specified handle, or registers and
     * returns a new one.
     */
    KeyboardMouseSystemImpl::MouseDeviceState& KeyboardMouseSystemImpl::_find_or_add_mouse_device(HANDLE device)
    {
        const DeviceId id = _device_id_from_handle(device);

        for (auto& entry : _mouse_devices)
            if (entry.id == id)
                return entry;

        _mouse_devices.push_back({.id = id, .mouse = &_add_mouse(id)});
        return _mouse_devices.back();
    }

    /**
     * Derives our opaque DeviceId from a raw input device handle.
     */
    KeyboardMouseSystemImpl::DeviceId KeyboardMouseSystemImpl::_device_id_from_handle(HANDLE device)
    {
        return static_cast<DeviceId>(reinterpret_cast<std::uintptr_t>(device));
    }

    /**
     * Maps a raw keyboard input report to our backend-agnostic Key, resolving the
     * left/right ambiguity Raw Input leaves for Ctrl/Alt/Shift.
     */
    Key KeyboardMouseSystemImpl::_key_from_raw_keyboard(const RAWKEYBOARD& raw_keyboard)
    {
        const bool is_e0 = (raw_keyboard.Flags & RI_KEY_E0) != 0;

        switch (raw_keyboard.VKey)
        {

        // Ctrl/Alt: Raw Input's E0 flag distinguishes right (set) from left (unset).
        case VK_CONTROL: return is_e0 ? Key::RightControl : Key::LeftControl;
        case VK_MENU:    return is_e0 ? Key::RightAlt : Key::LeftAlt;

        // Shift: not covered by the E0 flag (a well-known Raw Input quirk),
        // resolved via the scan code instead.
        case VK_SHIFT:
        {
            const UINT vkey_ex = ::MapVirtualKeyW(raw_keyboard.MakeCode, MAPVK_VSC_TO_VK_EX);
            return vkey_ex == VK_RSHIFT ? Key::RightShift : Key::LeftShift;
        }

        case 'A': return Key::A;
        case 'B': return Key::B;
        case 'C': return Key::C;
        case 'D': return Key::D;
        case 'E': return Key::E;
        case 'F': return Key::F;
        case 'G': return Key::G;
        case 'H': return Key::H;
        case 'I': return Key::I;
        case 'J': return Key::J;
        case 'K': return Key::K;
        case 'L': return Key::L;
        case 'M': return Key::M;
        case 'N': return Key::N;
        case 'O': return Key::O;
        case 'P': return Key::P;
        case 'Q': return Key::Q;
        case 'R': return Key::R;
        case 'S': return Key::S;
        case 'T': return Key::T;
        case 'U': return Key::U;
        case 'V': return Key::V;
        case 'W': return Key::W;
        case 'X': return Key::X;
        case 'Y': return Key::Y;
        case 'Z': return Key::Z;

        case '0': return Key::Digit0;
        case '1': return Key::Digit1;
        case '2': return Key::Digit2;
        case '3': return Key::Digit3;
        case '4': return Key::Digit4;
        case '5': return Key::Digit5;
        case '6': return Key::Digit6;
        case '7': return Key::Digit7;
        case '8': return Key::Digit8;
        case '9': return Key::Digit9;

        case VK_F1:  return Key::F1;
        case VK_F2:  return Key::F2;
        case VK_F3:  return Key::F3;
        case VK_F4:  return Key::F4;
        case VK_F5:  return Key::F5;
        case VK_F6:  return Key::F6;
        case VK_F7:  return Key::F7;
        case VK_F8:  return Key::F8;
        case VK_F9:  return Key::F9;
        case VK_F10: return Key::F10;
        case VK_F11: return Key::F11;
        case VK_F12: return Key::F12;

        case VK_SPACE:  return Key::Space;
        case VK_RETURN: return Key::Enter;
        case VK_ESCAPE: return Key::Escape;
        case VK_TAB:    return Key::Tab;
        case VK_BACK:   return Key::Backspace;
        case VK_DELETE: return Key::Delete;
        case VK_INSERT: return Key::Insert;

        case VK_LEFT:  return Key::Left;
        case VK_RIGHT: return Key::Right;
        case VK_UP:    return Key::Up;
        case VK_DOWN:  return Key::Down;
        case VK_HOME:  return Key::Home;
        case VK_END:   return Key::End;
        case VK_PRIOR: return Key::PageUp;
        case VK_NEXT:  return Key::PageDown;

        default:
        {
            // Key doesn't cover every VK code yet. We will expand this gradually
            // and/or at an appropriate time. Log each unmapped VK code to help find
            // gaps. Log each distinct VK code only once, not on every occurrence,
            // to avoid spamming the log for a held/repeated key.
            static std::array<bool, 256> logged_vkeys{};
            if (raw_keyboard.VKey < logged_vkeys.size() && !logged_vkeys[raw_keyboard.VKey])
            {
                logged_vkeys[raw_keyboard.VKey] = true;
                LOG_WARNING(u8"Raw Input: unmapped virtual key code " + cl7::to_string(static_cast<unsigned>(raw_keyboard.VKey)) + u8".");
            }
            return Key::Unknown;
        }

        } // switch (raw_keyboard.VKey)
    }



} // namespace xl7::input::impl::rawinput

#endif // F77_IS_WINDOWS
