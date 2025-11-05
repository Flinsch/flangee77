#include "StateManager.h"

#include <DataLabs/Base64.h>



namespace xl7::graphics::states {



    template <class TDesc>
    static cl7::u8string _identifier(cl7::u8string_view type_string, const TDesc& desc)
    {
        cl7::byte_view desc_data{reinterpret_cast<const std::byte*>(&desc), sizeof(TDesc)};

        return cl7::u8string(type_string) + u8" " + dl7::Base64().encode(desc_data);
    }



    /**
     * Creates and acquires the default state objects.
     */
    bool StateManager::create_default_states()
    {
        release_default_states();

        _default_sampler_state = find_resource<states::SamplerState>(ensure_sampler_state(states::SamplerState::Desc()));
        assert(_default_sampler_state);

        _default_rasterizer_state = find_resource<states::RasterizerState>(ensure_rasterizer_state(states::RasterizerState::Desc()));
        assert(_default_rasterizer_state);

        _default_depth_stencil_state = find_resource<states::DepthStencilState>(ensure_depth_stencil_state(states::DepthStencilState::Desc()));
        assert(_default_depth_stencil_state);

        _default_blend_state = find_resource<states::BlendState>(ensure_blend_state(states::BlendState::Desc()));
        assert(_default_blend_state);

        return _default_sampler_state && _default_rasterizer_state && _default_depth_stencil_state && _default_blend_state; // NOLINT(*-implicit-bool-conversion)
    }

    /**
     * Releases the default state objects.
     */
    bool StateManager::release_default_states()
    {
        release_resource(_default_sampler_state);
        _default_sampler_state = nullptr;

        release_resource(_default_rasterizer_state);
        _default_rasterizer_state = nullptr;

        release_resource(_default_depth_stencil_state);
        _default_depth_stencil_state = nullptr;

        release_resource(_default_blend_state);
        _default_blend_state = nullptr;

        return true;
    }

    /**
     * Creates and acquires the specified sampler state if not already done.
     */
    SamplerState::Id StateManager::ensure_sampler_state(const SamplerState::Desc& desc)
    {
        const cl7::u8string identifier = _identifier(u8"sampler state", desc);
        auto* resource = find_resource<SamplerState>(identifier);
        if (resource)
        {
            resource->add_reference();
            return resource->get_id();
        }

        resources::Resource::CreateParams<SamplerState::Desc> params{.manager = this, .id = _next_id(), .identifier = identifier, .desc = desc};

        ResourcePtr sampler_state(_factory->create_sampler_state(params), _destroy_resource);

        return _try_acquire_and_add_resource<SamplerState::Id>(std::move(sampler_state), resources::DefaultDataProvider());
    }

    /**
     * Creates and acquires the specified rasterizer state if not already done.
     */
    RasterizerState::Id StateManager::ensure_rasterizer_state(const RasterizerState::Desc& desc)
    {
        const cl7::u8string identifier = _identifier(u8"rasterizer state", desc);
        auto* resource = find_resource<RasterizerState>(identifier);
        if (resource)
        {
            resource->add_reference();
            return resource->get_id();
        }

        resources::Resource::CreateParams<RasterizerState::Desc> params{.manager = this, .id = _next_id(), .identifier = identifier, .desc = desc};

        ResourcePtr rasterizer_state(_factory->create_rasterizer_state(params), _destroy_resource);

        return _try_acquire_and_add_resource<RasterizerState::Id>(std::move(rasterizer_state), resources::DefaultDataProvider());
    }

    /**
     * Creates and acquires the specified depth/stencil state if not already done.
     */
    DepthStencilState::Id StateManager::ensure_depth_stencil_state(const DepthStencilState::Desc& desc)
    {
        const cl7::u8string identifier = _identifier(u8"depth/stencil state", desc);
        auto* resource = find_resource<DepthStencilState>(identifier);
        if (resource)
        {
            resource->add_reference();
            return resource->get_id();
        }

        resources::Resource::CreateParams<DepthStencilState::Desc> params{.manager = this, .id = _next_id(), .identifier = identifier, .desc = desc};

        ResourcePtr depth_stencil_state(_factory->create_depth_stencil_state(params), _destroy_resource);

        return _try_acquire_and_add_resource<DepthStencilState::Id>(std::move(depth_stencil_state), resources::DefaultDataProvider());
    }

    /**
     * Creates and acquires the specified blend state if not already done.
     */
    BlendState::Id StateManager::ensure_blend_state(const BlendState::Desc& desc)
    {
        const cl7::u8string identifier = _identifier(u8"blend state", desc);
        auto* resource = find_resource<BlendState>(identifier);
        if (resource)
        {
            resource->add_reference();
            return resource->get_id();
        }

        resources::Resource::CreateParams<BlendState::Desc> params{.manager = this, .id = _next_id(), .identifier = identifier, .desc = desc};

        ResourcePtr blend_state(_factory->create_blend_state(params), _destroy_resource);

        return _try_acquire_and_add_resource<BlendState::Id>(std::move(blend_state), resources::DefaultDataProvider());
    }



} // namespace xl7::graphics::states
