#ifndef XL7_INPUT_IMPL_XINPUT_GAMECONTROLLERSYSTEMIMPL_H
#define XL7_INPUT_IMPL_XINPUT_GAMECONTROLLERSYSTEMIMPL_H
#include "../../GameControllerSystem.h"
#if F77_IS_WINDOWS



namespace xl7::input::impl::xinput {



class GameControllerSystemImpl final
    : public GameControllerSystem
{
    struct Registrar
    {
        Registrar() { backend_registry().register_backend(DRIVER_NAME, DRIVER_ALIASES, factory_func, 1); }
        static GameControllerSystem* factory_func() { return new GameControllerSystemImpl(); }
    } static inline const _registrar = {};

public:
    static constexpr cl7::u8string_view DRIVER_NAME = u8"XInput";
    static constexpr std::initializer_list<cl7::u8string_view> DRIVER_ALIASES = {u8"XInput", u8"X-Input"};



    GameControllerSystemImpl(const GameControllerSystemImpl&) = delete;
    GameControllerSystemImpl& operator=(const GameControllerSystemImpl&) = delete;
    GameControllerSystemImpl(GameControllerSystemImpl&&) = delete;
    GameControllerSystemImpl& operator=(GameControllerSystemImpl&&) = delete;



    /**
     * Returns the name of the driver/implementation.
     */
    cl7::u8string_view get_driver_name() const override { return DRIVER_NAME; }



protected:
    GameControllerSystemImpl() = default;
    ~GameControllerSystemImpl() override = default;



private:

    // #############################################################################
    // GameControllerSystem Implementations
    // #############################################################################

    /**
     * Polls all XInput slots and fills in their raw state.
     */
    void _poll_impl(std::array<RawControllerState, MAX_CONTROLLERS>& out) override;

}; // class GameControllerSystemImpl



} // namespace xl7::input::impl::xinput



#endif // F77_IS_WINDOWS
#endif // XL7_INPUT_IMPL_XINPUT_GAMECONTROLLERSYSTEMIMPL_H
