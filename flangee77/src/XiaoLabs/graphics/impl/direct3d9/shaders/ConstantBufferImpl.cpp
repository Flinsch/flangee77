#include "ConstantBufferImpl.h"



namespace xl7::graphics::impl::direct3d9::shaders {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    ConstantBufferImpl::ConstantBufferImpl(const CreateContext& ctx, const graphics::shaders::ConstantBufferDesc& desc)
        : ConstantBuffer(ctx, desc)
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool ConstantBufferImpl::_acquire_impl()
    {
        return true;
    }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state before this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool ConstantBufferImpl::_dispose_impl()
    {
        return true;
    }

    /**
     * Flushes recent changes made to the local data copy by transferring them
     * "dirty" parts to the hardware and returns true after a successful transfer.
     */
    bool ConstantBufferImpl::_flush_data_impl()
    {
        return true;
    }



} // namespace xl7::graphics::impl::direct3d9::shaders
