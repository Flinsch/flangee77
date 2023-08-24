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

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    Component()
        : _config()
        , _initialized( false )
    {
    }

    /**
     * Destructor.
     */
    virtual ~Component() override
    {
        assert( !_initialized );
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
     * The DirectX pre-config structure.
     */
    Config _config;

    /**
     * The flag indicating whether the component has been fully initialized.
     */
    bool _initialized;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the DirectX pre-config structure.
     */
    const Config& get_config() const { return _config; }

    /**
     * Returns the flag indicating whether the component has been fully initialized.
     */
    bool is_initialized() const { return _initialized; }



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

        if ( _initialized )
        {
            if ( !shutdown() )
                return false;
        }

        assert( !_initialized );

        _initialized = _init();

        return _initialized;
    }

    /**
     * De-initializes the component.
     */
    bool shutdown()
    {
        const bool b = _shutdown();

        _initialized = false;

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
        if ( _initialized )
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
