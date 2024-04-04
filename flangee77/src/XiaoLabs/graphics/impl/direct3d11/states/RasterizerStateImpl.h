#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_STATES_RASTERIZERSTATEIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_STATES_RASTERIZERSTATEIMPL_H
#include "../../../states/RasterizerState.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace states {



class RasterizerStateImpl final
    : public xl7::graphics::states::RasterizerState
{

public:
    class Attorney
    {
        static RasterizerStateImpl* create(const CreateParams<Desc>& params) { return new RasterizerStateImpl( params ); }
        friend class ResourceFactoryImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    RasterizerStateImpl(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~RasterizerStateImpl() = default;

private:
    /** Default constructor. */
    RasterizerStateImpl() = delete;
    /** Copy constructor. */
    RasterizerStateImpl(const RasterizerStateImpl&) = delete;
    /** Copy assignment operator. */
    RasterizerStateImpl& operator = (const RasterizerStateImpl&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The Direct3D 11 rasterizer state interface.
     */
    wrl::ComPtr<ID3D11RasterizerState> _d3d_rasterizer_state;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the Direct3D 11 rasterizer state interface.
     */
    ID3D11RasterizerState* get_raw_d3d_rasterizer_state() const { return _d3d_rasterizer_state.Get(); }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Maps the specified rasterizer state descriptor to corresponding Direct3D 11
     * values and fills the given structure accordingly.
     */
    static void map_d3d_values(const Desc& desc, D3D11_RASTERIZER_DESC& d3d_rasterizer_desc);



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    virtual void* _get_raw_resource_impl() const override { return _d3d_rasterizer_state.Get(); }

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

}; // class RasterizerStateImpl



} // namespace states
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_STATES_RASTERIZERSTATEIMPL_H
