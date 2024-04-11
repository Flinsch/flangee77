#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_SHADERS_CONSTANTBUFFERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_SHADERS_CONSTANTBUFFERIMPL_H
#include "../../../shaders/ConstantBuffer.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace shaders {



class ConstantBufferImpl final
    : public xl7::graphics::shaders::ConstantBuffer
{

public:
    class Attorney
    {
        static ConstantBufferImpl* create(const CreateParams<Desc>& params) { return new ConstantBufferImpl( params ); }
        friend class ResourceFactoryImpl;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    ConstantBufferImpl(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~ConstantBufferImpl() = default;

private:
    /** Default constructor. */
    ConstantBufferImpl() = delete;
    /** Copy constructor. */
    ConstantBufferImpl(const ConstantBufferImpl&) = delete;
    /** Copy assignment operator. */
    ConstantBufferImpl& operator = (const ConstantBufferImpl&) = delete;



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

}; // class ConstantBufferImpl



} // namespace shaders
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_SHADERS_CONSTANTBUFFERIMPL_H
