#include "DepthStencilStateImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../mappings.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace states {



    static D3D11_STENCIL_OP _d3d_stencil_op_from(xl7::graphics::states::DepthStencilState::StencilOperation stencil_operation)
    {
        static_assert( static_cast<unsigned>( xl7::graphics::states::DepthStencilState::StencilOperation::Keep ) == static_cast<unsigned>( D3D11_STENCIL_OP_KEEP ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::DepthStencilState::StencilOperation::Zero ) == static_cast<unsigned>( D3D11_STENCIL_OP_ZERO ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::DepthStencilState::StencilOperation::Replace ) == static_cast<unsigned>( D3D11_STENCIL_OP_REPLACE ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::DepthStencilState::StencilOperation::IncrClamp ) == static_cast<unsigned>( D3D11_STENCIL_OP_INCR_SAT ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::DepthStencilState::StencilOperation::DecrClamp ) == static_cast<unsigned>( D3D11_STENCIL_OP_DECR_SAT ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::DepthStencilState::StencilOperation::Invert ) == static_cast<unsigned>( D3D11_STENCIL_OP_INVERT ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::DepthStencilState::StencilOperation::IncrWrap ) == static_cast<unsigned>( D3D11_STENCIL_OP_INCR ) );
        static_assert( static_cast<unsigned>( xl7::graphics::states::DepthStencilState::StencilOperation::DecrWrap ) == static_cast<unsigned>( D3D11_STENCIL_OP_DECR ) );

        return static_cast<D3D11_STENCIL_OP>( stencil_operation );
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    DepthStencilStateImpl::DepthStencilStateImpl(const CreateParams<Desc>& params)
        : DepthStencilState( params )
    {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Maps the specified depth/stencil state descriptor to corresponding Direct3D 11
     * values and fills the given structure accordingly.
     */
    void DepthStencilStateImpl::map_d3d_values(const Desc& desc, D3D11_DEPTH_STENCIL_DESC& d3d_depth_stencil_desc)
    {
        d3d_depth_stencil_desc.DepthEnable = desc.is_depth_testing_enabled ? TRUE : FALSE;
        d3d_depth_stencil_desc.DepthWriteMask = desc.is_depth_writing_enabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
        d3d_depth_stencil_desc.DepthFunc = mappings::_d3d_comparison_func_from( desc.depth_test_function );

        d3d_depth_stencil_desc.StencilEnable = desc.is_stenciling_enabled ? TRUE : FALSE;
        d3d_depth_stencil_desc.StencilReadMask = static_cast<UINT8>( desc.stencil_test_mask );
        d3d_depth_stencil_desc.StencilWriteMask = static_cast<UINT8>( desc.stencil_write_mask );

        d3d_depth_stencil_desc.FrontFace.StencilFailOp = _d3d_stencil_op_from( desc.front_face_stenciling.stencil_fail_operation );
        d3d_depth_stencil_desc.FrontFace.StencilDepthFailOp = _d3d_stencil_op_from( desc.front_face_stenciling.depth_fail_operation );
        d3d_depth_stencil_desc.FrontFace.StencilPassOp = _d3d_stencil_op_from( desc.front_face_stenciling.pass_operation );
        d3d_depth_stencil_desc.FrontFace.StencilFunc = mappings::_d3d_comparison_func_from( desc.front_face_stenciling.stencil_test_function );

        d3d_depth_stencil_desc.BackFace.StencilFailOp = _d3d_stencil_op_from( desc.back_face_stenciling.stencil_fail_operation );
        d3d_depth_stencil_desc.BackFace.StencilDepthFailOp = _d3d_stencil_op_from( desc.back_face_stenciling.depth_fail_operation );
        d3d_depth_stencil_desc.BackFace.StencilPassOp = _d3d_stencil_op_from( desc.back_face_stenciling.pass_operation );
        d3d_depth_stencil_desc.BackFace.StencilFunc = mappings::_d3d_comparison_func_from( desc.back_face_stenciling.stencil_test_function );
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
        auto d3d_device = static_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->get_raw_d3d_device();
        assert( d3d_device );

        D3D11_DEPTH_STENCIL_DESC d3d_depth_stencil_desc;
        map_d3d_values( get_desc(), d3d_depth_stencil_desc );

        HRESULT hresult = d3d_device->CreateDepthStencilState(
            &d3d_depth_stencil_desc,
            &_d3d_depth_stencil_state );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::CreateDepthStencilState") ) );
            return false;
        }

        return true;
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool DepthStencilStateImpl::_dispose_impl()
    {
        _d3d_depth_stencil_state.Reset();

        return true;
    }



} // namespace states
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
