#ifndef XL7_COMPONENT_H
#define XL7_COMPONENT_H
#include <CoreLabs/creational/Singleton.h>

#include "./ConfigProvider.h"



namespace xl7 {



template <class TSingleton>
class Component
    : public cl7::creational::Singleton<TSingleton>
{

public:
    enum struct State
    {
        NotInitialized,
        FullyInitialized,
        PartiallyInitialized,
    };



    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    Component(Component&&) = delete;
    Component& operator=(Component&&) = delete;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Initializes the component.
     */
    bool init()
    {
        // Capture own copy of the "X" pre-config structure.
        _config = ConfigProvider::instance().get_config();

        if (_state != State::NotInitialized)
        {
            if (!shutdown())
                return false;
        }

        assert(_state == State::NotInitialized);

        _state = State::PartiallyInitialized;
        if (_init())
            _state = State::FullyInitialized;

        return is_operational();
    }

    /**
     * De-initializes the component.
     */
    bool shutdown()
    {
        const bool b = _shutdown();

        _state = State::NotInitialized;

        return b;
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns own copy of the "X" pre-config structure.
     */
    const Config& get_config() const { return _config; }

    /**
     * Returns the state indicating whether the component has been initialized.
     */
    State get_state() const { return _state; }

    /**
     * Returns the flag indicating whether the component has been fully initialized.
     */
    bool is_operational() const { return _state == State::FullyInitialized; }



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Component()
        : _state(State::NotInitialized)
    {
    }

    ~Component() override
    {
        assert(_state == State::NotInitialized);
    }



private:

    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Initializes the component.
     */
    virtual bool _init() = 0;

    /**
     * De-initializes the component.
     */
    virtual bool _shutdown() = 0;



    // #############################################################################
    // Singleton Implementations
    // #############################################################################

    /**
     * This is called just before the singleton object is destroyed.
     * If the singleton object has cleanup functions that are virtual and therefore
     * cannot be called from the destructor, then they should be called here.
     */
    void _before_destroy() final
    {
        if (_state != State::NotInitialized)
            shutdown();
    }



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * Own copy of the "X" pre-config structure.
     */
    Config _config;

    /**
     * The state indicating whether the component has been initialized.
     */
    State _state;

}; // class Component



} // namespace xl7

#endif // XL7_COMPONENT_H
