#include "StateManager.h"

#include <CoreLabs/Base64.h>



namespace xl7 {
namespace graphics {
namespace states {



    template <class TDesc>
    static cl7::astring _identifier(cl7::astring_view type_string, const TDesc& desc)
    {
        cl7::byte_view desc_data{ (const std::byte*)&desc, sizeof(TDesc) };

        return cl7::astring(type_string) + (" ") + cl7::Base64().encode( desc_data );
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    StateManager::StateManager(IStateFactory* factory)
        : ResourceManager()
        , _factory( factory )
        , _default_sampler_state( nullptr )
        , _default_rasterizer_state( nullptr )
    {
    }

    /**
     * Destructor.
     */
    StateManager::~StateManager()
    {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Creates and acquires the default state objects.
     */
    bool StateManager::create_default_states()
    {
        release_default_states();

        _default_sampler_state = find_resource<states::SamplerState>( ensure_sampler_state( states::SamplerState::Desc() ) );
        assert( _default_sampler_state );

        _default_rasterizer_state = find_resource<states::RasterizerState>( ensure_rasterizer_state( states::RasterizerState::Desc() ) );
        assert( _default_rasterizer_state );

        return _default_sampler_state && _default_rasterizer_state;
    }

    /**
     * Releases the default state objects.
     */
    bool StateManager::release_default_states()
    {
        release_resource( _default_sampler_state );
        _default_sampler_state = nullptr;

        release_resource( _default_rasterizer_state );
        _default_rasterizer_state = nullptr;

        return true;
    }

    /**
     * Creates and acquires the specified sampler state if not already done.
     */
    resources::ResourceID StateManager::ensure_sampler_state(const SamplerState::Desc& desc)
    {
        const cl7::astring identifier = _identifier( "sampler state", desc );
        resources::Resource* resource = find_resource( identifier );
        if ( resource )
        {
            resource->add_reference();
            return resource->get_id();
        }

        resources::Resource::CreateParams<SamplerState::Desc> params{ this, _next_id(), identifier, desc };

        ResourcePtr sampler_state( _factory->create_sampler_state( params ), _destroy_resource );

        return _try_acquire_and_add_resource( std::move(sampler_state), resources::DefaultDataProvider() );
    }

    /**
     * Creates and acquires the specified rasterizer state if not already done.
     */
    resources::ResourceID StateManager::ensure_rasterizer_state(const RasterizerState::Desc& desc)
    {
        const cl7::astring identifier = _identifier( "rasterizer state", desc );
        resources::Resource* resource = find_resource( identifier );
        if ( resource )
        {
            resource->add_reference();
            return resource->get_id();
        }

        resources::Resource::CreateParams<RasterizerState::Desc> params{ this, _next_id(), identifier, desc };

        ResourcePtr rasterizer_state( _factory->create_rasterizer_state( params ), _destroy_resource );

        return _try_acquire_and_add_resource( std::move(rasterizer_state), resources::DefaultDataProvider() );
    }



} // namespace states
} // namespace graphics
} // namespace xl7
