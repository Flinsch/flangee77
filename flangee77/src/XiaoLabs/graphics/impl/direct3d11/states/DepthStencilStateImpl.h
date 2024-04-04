#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_STATES_DEPTHSTENCILSTATEIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_STATES_DEPTHSTENCILSTATEIMPL_H
#include "../../../states/DepthStencilState.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace states {



class DepthStencilStateImpl final
    : public xl7::graphics::states::DepthStencilState
{

public:
    class Attorney
    {
        static DepthStencilStateImpl* create(const CreateParams<Desc>& params) { return new DepthStencilStateImpl( params ); }
        friend class ResourceFactoryImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    DepthStencilStateImpl(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~DepthStencilStateImpl() = default;

private:
    /** Default constructor. */
    DepthStencilStateImpl() = delete;
    /** Copy constructor. */
    DepthStencilStateImpl(const DepthStencilStateImpl&) = delete;
    /** Copy assignment operator. */
    DepthStencilStateImpl& operator = (const DepthStencilStateImpl&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The Direct3D 11 depth/stencil state interface.
     */
    wrl::ComPtr<ID3D11DepthStencilState> _d3d_depth_stencil_state;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the Direct3D 11 depth/stencil state interface.
     */
    ID3D11DepthStencilState* get_raw_d3d_depth_stencil_state() const { return _d3d_depth_stencil_state.Get(); }



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    virtual void* _get_raw_resource_impl() const override { return _d3d_depth_stencil_state.Get(); }

private:
    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const xl7::resources::DataProvider& data_provider) override;

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    virtual bool _dispose_impl() override;

}; // class DepthStencilStateImpl



} // namespace states
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_STATES_DEPTHSTENCILSTATEIMPL_H
