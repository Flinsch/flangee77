#include "DepthStencilStateImpl.h"

#include "../mappings.h"



namespace xl7::graphics::impl::direct3d9::states {



    static D3DSTENCILOP _d3d_stencil_op_from(xl7::graphics::states::DepthStencilState::StencilOperation stencil_operation)
    {
        static_assert(static_cast<unsigned>(xl7::graphics::states::DepthStencilState::StencilOperation::Keep) == static_cast<unsigned>(D3DSTENCILOP_KEEP));
        static_assert(static_cast<unsigned>(xl7::graphics::states::DepthStencilState::StencilOperation::Zero) == static_cast<unsigned>(D3DSTENCILOP_ZERO));
        static_assert(static_cast<unsigned>(xl7::graphics::states::DepthStencilState::StencilOperation::Replace) == static_cast<unsigned>(D3DSTENCILOP_REPLACE));
        static_assert(static_cast<unsigned>(xl7::graphics::states::DepthStencilState::StencilOperation::IncrClamp) == static_cast<unsigned>(D3DSTENCILOP_INCRSAT));
        static_assert(static_cast<unsigned>(xl7::graphics::states::DepthStencilState::StencilOperation::DecrClamp) == static_cast<unsigned>(D3DSTENCILOP_DECRSAT));
        static_assert(static_cast<unsigned>(xl7::graphics::states::DepthStencilState::StencilOperation::Invert) == static_cast<unsigned>(D3DSTENCILOP_INVERT));
        static_assert(static_cast<unsigned>(xl7::graphics::states::DepthStencilState::StencilOperation::IncrWrap) == static_cast<unsigned>(D3DSTENCILOP_INCR));
        static_assert(static_cast<unsigned>(xl7::graphics::states::DepthStencilState::StencilOperation::DecrWrap) == static_cast<unsigned>(D3DSTENCILOP_DECR));

        return static_cast<D3DSTENCILOP>(stencil_operation);
    }



    /**
     * Maps the specified depth/stencil state descriptor to corresponding Direct3D 9
     * values and fills the given structure accordingly.
     */
    void DepthStencilStateImpl::map_d3d_values(const Desc& desc, D3DDepthStencilStateTypeValues& d3d_depth_stencil_state_type_values)
    {
        d3d_depth_stencil_state_type_values = D3DDepthStencilStateTypeValues({
            {D3DRS_ZENABLE, desc.is_depth_testing_enabled ? D3DZB_TRUE : D3DZB_FALSE},
            {D3DRS_ZWRITEENABLE, desc.is_depth_writing_enabled ? TRUE : FALSE},
            {D3DRS_ZFUNC, mappings::_d3d_cmp_func_from(desc.depth_test_function)},

            {D3DRS_STENCILENABLE, desc.is_stenciling_enabled ? TRUE : FALSE},
            {D3DRS_STENCILMASK, static_cast<DWORD>(desc.stencil_test_mask)},
            {D3DRS_STENCILWRITEMASK, static_cast<DWORD>(desc.stencil_write_mask)},

            // Here we first equate "CW" with "front faces" and "CCW" with "back faces".
            // These values may have to be swapped when flushing the rendering context.

            {D3DRS_STENCILFAIL, _d3d_stencil_op_from(desc.front_face_stenciling.stencil_fail_operation)},
            {D3DRS_STENCILZFAIL, _d3d_stencil_op_from(desc.front_face_stenciling.depth_fail_operation)},
            {D3DRS_STENCILPASS, _d3d_stencil_op_from(desc.front_face_stenciling.pass_operation)},
            {D3DRS_STENCILFUNC, mappings::_d3d_cmp_func_from(desc.front_face_stenciling.stencil_test_function)},

            {D3DRS_CCW_STENCILFAIL, _d3d_stencil_op_from(desc.back_face_stenciling.stencil_fail_operation)},
            {D3DRS_CCW_STENCILZFAIL, _d3d_stencil_op_from(desc.back_face_stenciling.depth_fail_operation)},
            {D3DRS_CCW_STENCILPASS, _d3d_stencil_op_from(desc.back_face_stenciling.pass_operation)},
            {D3DRS_CCW_STENCILFUNC, mappings::_d3d_cmp_func_from(desc.back_face_stenciling.stencil_test_function)},
        });
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    DepthStencilStateImpl::DepthStencilStateImpl(const CreateParams<Desc>& params)
        : DepthStencilState(params)
        , _d3d_depth_stencil_state_type_values()
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
    bool DepthStencilStateImpl::_acquire_impl(const xl7::resources::DataProvider& data_provider)
    {
        map_d3d_values(get_desc(), _d3d_depth_stencil_state_type_values);

        return true;
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool DepthStencilStateImpl::_dispose_impl()
    {
        return true;
    }



} // namespace xl7::graphics::impl::direct3d9::states
