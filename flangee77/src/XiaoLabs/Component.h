#pragma once
#ifndef XL7_COMPONENT_H
#define XL7_COMPONENT_H
#include <CoreLabs/creational/Singleton.h>

#include "./Config.h"



namespace xl7 {



template <class TSingleton>
class Component
    : public cl7::creational::Singleton<TSingleton>
{

public:
    enum class State
    {
        NotInitialized,
        FullyInitialized,
        PartiallyInitialized,
    }; // enum class State



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    Component()
        : _config()
        , _state( State::NotInitialized )
    {
    }

    /**
     * Destructor.
     */
    virtual ~Component() override
    {
        assert( _state == State::NotInitialized );
    }

private:
    /** Copy constructor. */
    Component(const Component&) = delete;
    /** Copy assignment operator. */
    Component& operator = (const Component&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The "X" pre-config structure.
     */
    Config _config;

    /**
     * The state indicating whether the component has been initialized.
     */
    State _state;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the "X" pre-config structure.
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



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Initializes the component.
     */
    bool init(const Config& config)
    {
        _config = config;

        if ( _state != State::NotInitialized )
        {
            if ( !shutdown() )
                return false;
        }

        assert( _state == State::NotInitialized );

        _state = State::PartiallyInitialized;
        if ( _init() )
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
    // Singleton Implementations
    // #############################################################################
private:
    /**
     * This is called just before the singleton object is destroyed.
     * If the singleton object has cleanup functions that are virtual and therefore
     * cannot be called from the destructor, then they should be called here.
     */
    virtual void _before_destroy() final
    {
        if ( _state != State::NotInitialized )
            shutdown();
    }



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Initializes the component.
     */
    virtual bool _init() = 0;

    /**
     * De-initializes the component.
     */
    virtual bool _shutdown() = 0;

}; // class Component



} // namespace xl7

#endif // XL7_COMPONENT_H
