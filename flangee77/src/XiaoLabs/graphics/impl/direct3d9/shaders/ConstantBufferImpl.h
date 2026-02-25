#ifndef XL7_GRAPHICS_IMPL_D3D9_SHADERS_CONSTANTBUFFERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_SHADERS_CONSTANTBUFFERIMPL_H
#include "../../../shaders/ConstantBuffer.h"

#include "../prerequisites.h"



namespace xl7::graphics::impl::direct3d9 {
    class ResourceFactoryImpl;
namespace shaders {



class ConstantBufferImpl final
    : public xl7::graphics::shaders::ConstantBuffer
{

public:
    class Attorney
    {
        static ConstantBufferImpl* create(const CreateParams<Desc>& params) { return new ConstantBufferImpl(params); }
        friend class xl7::graphics::impl::direct3d9::ResourceFactoryImpl;
    };



    ConstantBufferImpl() = delete;

    ConstantBufferImpl(const ConstantBufferImpl&) = delete;
    ConstantBufferImpl& operator=(const ConstantBufferImpl&) = delete;
    ConstantBufferImpl(ConstantBufferImpl&&) = delete;
    ConstantBufferImpl& operator=(ConstantBufferImpl&&) = delete;



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    explicit ConstantBufferImpl(const CreateParams<Desc>& params);
    ~ConstantBufferImpl() override = default;



private:

    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    void* _get_raw_resource_impl() const override { return nullptr; }

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool _dispose_impl() override;



    // #############################################################################
    // ConstantBuffer Implementations
    // #############################################################################

    /**
     * Requests/acquires the constant buffer resource.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool _acquire_impl(const xl7::graphics::shaders::ConstantDataProvider& constant_data_provider) override;

    /**
     * Updates the contents of this constant buffer.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool _update_impl(const xl7::graphics::shaders::ConstantDataProvider& constant_data_provider, bool discard, bool no_overwrite) override;

}; // class ConstantBufferImpl



} // namespace shaders
} // namespace xl7::graphics::impl::direct3d9

#endif // XL7_GRAPHICS_IMPL_D3D9_SHADERS_CONSTANTBUFFERIMPL_H
