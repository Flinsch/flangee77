#ifndef XL7_INPUT_IMPL_RAWINPUT_KEYBOARDMOUSESYSTEMIMPL_H
#define XL7_INPUT_IMPL_RAWINPUT_KEYBOARDMOUSESYSTEMIMPL_H
#include "../../KeyboardMouseSystem.h"
#if F77_IS_WINDOWS



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



protected:
    KeyboardMouseSystemImpl() = default;
    ~KeyboardMouseSystemImpl() override = default;

    // Raw Input keeps state current as messages arrive rather than being
    // polled, so there's no per-frame work to override here (unlike
    // GameControllerSystem's _poll_impl()). This backend doesn't actually
    // register for/handle Raw Input messages yet, since that needs a
    // message-forwarding hook in WindowImpl::wnd_proc that doesn't exist yet;
    // TODO once that hook lands: register devices via _add_keyboard()/
    // _add_mouse() and feed them via _apply_keyboard()/_apply_mouse().

}; // class KeyboardMouseSystemImpl



} // namespace xl7::input::impl::rawinput



#endif // F77_IS_WINDOWS
#endif // XL7_INPUT_IMPL_RAWINPUT_KEYBOARDMOUSESYSTEMIMPL_H
