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
        friend class xl7::graphics::RenderingDevice;
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

private:
    /**
     * The default sampler state.
     */
    states::SamplerState* _default_sampler_state;

    /**
     * The default rasterizer state.
     */
    states::RasterizerState* _default_rasterizer_state;

    /**
     * The default depth/stencil state.
     */
    states::DepthStencilState* _default_depth_stencil_state;

    /**
     * The default blend state.
     */
    states::BlendState* _default_blend_state;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the default sampler state.
     */
    states::SamplerState* get_default_sampler_state() const { return _default_sampler_state; }

    /**
     * Returns the default rasterizer state.
     */
    states::RasterizerState* get_default_rasterizer_state() const { return _default_rasterizer_state; }

    /**
     * Returns the default depth/stencil state.
     */
    states::DepthStencilState* get_default_depth_stencil_state() const { return _default_depth_stencil_state; }

    /**
     * Returns the default blend state.
     */
    states::BlendState* get_default_blend_state() const { return _default_blend_state; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Creates and acquires the default state objects.
     */
    bool create_default_states();

    /**
     * Releases the default state objects.
     */
    bool release_default_states();

    /**
     * Creates and acquires the specified sampler state if not already done.
     */
    resources::ResourceID ensure_sampler_state(const SamplerState::Desc& desc);

    /**
     * Creates and acquires the specified rasterizer state if not already done.
     */
    resources::ResourceID ensure_rasterizer_state(const RasterizerState::Desc& desc);

    /**
     * Creates and acquires the specified depth/stencil state if not already done.
     */
    resources::ResourceID ensure_depth_stencil_state(const DepthStencilState::Desc& desc);

    /**
     * Creates and acquires the specified blend state if not already done.
     */
    resources::ResourceID ensure_blend_state(const BlendState::Desc& desc);

}; // class StateManager



} // namespace states
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_STATES_STATEMANAGER_H
