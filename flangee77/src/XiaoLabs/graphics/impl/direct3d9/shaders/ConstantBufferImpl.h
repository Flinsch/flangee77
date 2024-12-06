#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_SHADERS_CONSTANTBUFFERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_SHADERS_CONSTANTBUFFERIMPL_H
#include "../../../shaders/ConstantBuffer.h"

#include "../prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
    class ResourceFactoryImpl;
namespace shaders {



class ConstantBufferImpl final
    : public xl7::graphics::shaders::ConstantBuffer
{

public:
    class Attorney
    {
        static ConstantBufferImpl* create(const CreateParams<Desc>& params) { return new ConstantBufferImpl( params ); }
        friend class xl7::graphics::impl::direct3d9::ResourceFactoryImpl;
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
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    virtual bool _dispose_impl() override;



    // #############################################################################
    // ConstantBuffer Implementations
    // #############################################################################
private:
    /**
     * Requests/acquires the constant buffer resource.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const xl7::graphics::shaders::ConstantDataProvider& constant_data_provider) override;

    /**
     * Updates the contents of this constant buffer.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _update_impl(const xl7::graphics::shaders::ConstantDataProvider& constant_data_provider, bool discard, bool no_overwrite) override;

}; // class ConstantBufferImpl



} // namespace shaders
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_SHADERS_CONSTANTBUFFERIMPL_H
