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
     * Creates and acquires the specified sampler state if not already done.
     */
    resources::ResourceID StateManager::ensure_sampler_state(const SamplerState::Desc& desc)
    {
        const cl7::astring identifier = _identifier( "sampler state", desc );
        resources::Resource* resource = find_resource( identifier );
        if ( resource )
            return resource->get_id();

        resources::Resource::CreateParams<SamplerState::Desc> params{ this, _next_id(), identifier, desc };

        ResourcePtr sampler_state( _factory->create_sampler_state( params ), _destroy_resource );

        return _try_acquire_and_add_resource( std::move(sampler_state), resources::DefaultDataProvider() );
    }



} // namespace states
} // namespace graphics
} // namespace xl7
