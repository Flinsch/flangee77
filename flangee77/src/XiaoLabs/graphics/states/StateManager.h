#pragma once
#ifndef XL7_GRAPHICS_STATES_STATEMANAGER_H
#define XL7_GRAPHICS_STATES_STATEMANAGER_H
#include "../../resources/ResourceManager.h"

#include "./IStateFactory.h"



namespace xl7 {
namespace graphics {
    class RenderingDevice;
namespace states {



class StateManager
    : public resources::ResourceManager
{

public:
    class Attorney
    {
        static StateManager* create(IStateFactory* factory) { return new StateManager( factory ); }
        static void destroy(StateManager* manager) { delete manager; }
        friend class RenderingDevice;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    StateManager(IStateFactory* factory);

    /**
     * Destructor.
     */
    virtual ~StateManager();

private:
    /** Default constructor. */
    StateManager() = delete;
    /** Copy constructor. */
    StateManager(const StateManager&) = delete;
    /** Copy assignment operator. */
    StateManager& operator = (const StateManager&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The state factory.
     */
    IStateFactory* const _factory;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Creates and acquires the specified sampler state if not already done.
     */
    resources::ResourceID ensure_sampler_state(const SamplerState::Desc& desc);

}; // class StateManager



} // namespace states
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_STATES_STATEMANAGER_H
