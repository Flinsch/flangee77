#include "SamplerStateImpl.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace states {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    SamplerStateImpl::SamplerStateImpl(const CreateParams<Desc>& params)
        : SamplerState( params )
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
    bool SamplerStateImpl::_acquire_impl(const xl7::resources::DataProvider& data_provider)
    {
        

        return true;
    }

    /**
     * Releases/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool SamplerStateImpl::_release_impl()
    {
        return true;
    }



} // namespace states
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
