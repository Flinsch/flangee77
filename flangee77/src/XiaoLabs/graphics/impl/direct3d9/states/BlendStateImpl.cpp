#include "BlendStateImpl.h"



namespace xl7::graphics::impl::direct3d9::states {



    static D3DBLEND _d3d_blend_from(xl7::graphics::states::BlendState::BlendFactor blend_factor)
    {
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendFactor::Zero) == static_cast<unsigned>(D3DBLEND_ZERO));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendFactor::One) == static_cast<unsigned>(D3DBLEND_ONE));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendFactor::SrcColor) == static_cast<unsigned>(D3DBLEND_SRCCOLOR));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendFactor::InvSrcColor) == static_cast<unsigned>(D3DBLEND_INVSRCCOLOR));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendFactor::SrcAlpha) == static_cast<unsigned>(D3DBLEND_SRCALPHA));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendFactor::InvSrcAlpha) == static_cast<unsigned>(D3DBLEND_INVSRCALPHA));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendFactor::DestAlpha) == static_cast<unsigned>(D3DBLEND_DESTALPHA));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendFactor::InvDestAlpha) == static_cast<unsigned>(D3DBLEND_INVDESTALPHA));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendFactor::DestColor) == static_cast<unsigned>(D3DBLEND_DESTCOLOR));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendFactor::InvDestColor) == static_cast<unsigned>(D3DBLEND_INVDESTCOLOR));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendFactor::SrcAlphaSat) == static_cast<unsigned>(D3DBLEND_SRCALPHASAT));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendFactor::BlendFactor) == static_cast<unsigned>(D3DBLEND_BLENDFACTOR));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendFactor::InvBlendFactor) == static_cast<unsigned>(D3DBLEND_INVBLENDFACTOR));

        return static_cast<D3DBLEND>(blend_factor);
    }

    static D3DBLENDOP _d3d_blend_op_from(xl7::graphics::states::BlendState::BlendOperation blend_operation)
    {
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendOperation::Add) == static_cast<unsigned>(D3DBLENDOP_ADD));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendOperation::Subtract) == static_cast<unsigned>(D3DBLENDOP_SUBTRACT));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendOperation::RevSubtract) == static_cast<unsigned>(D3DBLENDOP_REVSUBTRACT));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendOperation::Min) == static_cast<unsigned>(D3DBLENDOP_MIN));
        static_assert(static_cast<unsigned>(xl7::graphics::states::BlendState::BlendOperation::Max) == static_cast<unsigned>(D3DBLENDOP_MAX));

        return static_cast<D3DBLENDOP>(blend_operation);
    }

    static DWORD _d3d_color_write_enable_from(xl7::graphics::ChannelFlags channel_write_flags)
    {
        static_assert(static_cast<unsigned>(xl7::graphics::ChannelFlags::Red) == static_cast<unsigned>(D3DCOLORWRITEENABLE_RED));
        static_assert(static_cast<unsigned>(xl7::graphics::ChannelFlags::Green) == static_cast<unsigned>(D3DCOLORWRITEENABLE_GREEN));
        static_assert(static_cast<unsigned>(xl7::graphics::ChannelFlags::Blue) == static_cast<unsigned>(D3DCOLORWRITEENABLE_BLUE));
        static_assert(static_cast<unsigned>(xl7::graphics::ChannelFlags::Alpha) == static_cast<unsigned>(D3DCOLORWRITEENABLE_ALPHA));

        return static_cast<DWORD>(channel_write_flags);
    }



    /**
     * Maps the specified blend state descriptor to corresponding Direct3D 9
     * values and fills the given structure accordingly.
     */
    void BlendStateImpl::map_d3d_values(const Desc& desc, D3DBlendStateTypeValues& d3d_blend_state_type_values)
    {
        d3d_blend_state_type_values = D3DBlendStateTypeValues({
            {D3DRS_ALPHABLENDENABLE, desc.is_blending_enabled ? TRUE : FALSE},
            {D3DRS_SEPARATEALPHABLENDENABLE, desc.is_blending_enabled && (desc.src_alpha_factor != desc.src_color_factor || desc.dest_alpha_factor != desc.dest_color_factor || desc.alpha_operation != desc.color_operation) ? TRUE : FALSE},
            {D3DRS_SRCBLEND, _d3d_blend_from(desc.src_color_factor)},
            {D3DRS_DESTBLEND, _d3d_blend_from(desc.dest_color_factor)},
            {D3DRS_BLENDOP, _d3d_blend_op_from(desc.color_operation)},
            {D3DRS_SRCBLENDALPHA, _d3d_blend_from(desc.src_alpha_factor)},
            {D3DRS_DESTBLENDALPHA, _d3d_blend_from(desc.dest_alpha_factor)},
            {D3DRS_BLENDOPALPHA, _d3d_blend_op_from(desc.alpha_operation)},
            {D3DRS_COLORWRITEENABLE, _d3d_color_write_enable_from(desc.channel_write_flags)},
        });
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    BlendStateImpl::BlendStateImpl(const CreateParams<Desc>& params)
        : BlendState(params)
        , _d3d_blend_state_type_values()
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool BlendStateImpl::_acquire_impl(const xl7::resources::DataProvider& data_provider)
    {
        map_d3d_values(get_desc(), _d3d_blend_state_type_values);

        return true;
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool BlendStateImpl::_dispose_impl()
    {
        return true;
    }



} // namespace xl7::graphics::impl::direct3d9::states
