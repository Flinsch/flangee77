#ifndef XL7_GRAPHICS_IMPL_D3D11_SHADERS_CONSTANTBUFFERIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_SHADERS_CONSTANTBUFFERIMPL_H
#include "../../../shaders/ConstantBuffer.h"

#include "../prerequisites.h"



namespace xl7::graphics::impl::direct3d11 {
    class ResourceFactoryImpl;
namespace shaders {



class ConstantBufferImpl final
    : public graphics::shaders::ConstantBuffer
{

public:
    class Attorney
    {
        static ConstantBufferImpl* create(const CreateContext& ctx, const graphics::shaders::ConstantBufferDesc& desc) { return new ConstantBufferImpl(ctx, desc); }
        friend class direct3d11::ResourceFactoryImpl;
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

    explicit ConstantBufferImpl(const CreateContext& ctx, const graphics::shaders::ConstantBufferDesc& desc);
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
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool _acquire_impl() override;

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state before this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool _dispose_impl() override;

    /**
     * Flushes recent changes made to the local data copy by transferring them
     * "dirty" parts to the hardware and returns true after a successful transfer.
     */
    bool _flush_data_impl() override;

}; // class ConstantBufferImpl



} // namespace shaders
} // namespace xl7::graphics::impl::direct3d11

#endif // XL7_GRAPHICS_IMPL_D3D11_SHADERS_CONSTANTBUFFERIMPL_H
