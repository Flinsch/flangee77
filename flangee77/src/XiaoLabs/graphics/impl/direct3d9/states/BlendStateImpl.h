#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_STATES_BLENDSTATEIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_STATES_BLENDSTATEIMPL_H
#include "../../../states/BlendState.h"

#include "../prerequisites.h"

#include "./D3DBlendStateTypeValues.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace states {



class BlendStateImpl final
    : public xl7::graphics::states::BlendState
{

public:
    class Attorney
    {
        static BlendStateImpl* create(const CreateParams<Desc>& params) { return new BlendStateImpl( params ); }
        friend class ResourceFactoryImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    BlendStateImpl(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~BlendStateImpl() = default;

private:
    /** Default constructor. */
    BlendStateImpl() = delete;
    /** Copy constructor. */
    BlendStateImpl(const BlendStateImpl&) = delete;
    /** Copy assignment operator. */
    BlendStateImpl& operator = (const BlendStateImpl&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The Direct3D 9 blend state type values.
     */
    D3DBlendStateTypeValues _d3d_blend_state_type_values;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the Direct3D 9 blend state type values.
     */
    const D3DBlendStateTypeValues& get_d3d_blend_state_type_values() const { return _d3d_blend_state_type_values; }



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    virtual void* _get_raw_resource_impl() const override { return nullptr; }

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

}; // class BlendStateImpl



} // namespace states
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_STATES_BLENDSTATEIMPL_H
