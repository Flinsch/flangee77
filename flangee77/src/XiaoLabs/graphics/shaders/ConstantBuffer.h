#ifndef XL7_GRAPHICS_SHADERS_CONSTANTBUFFER_H
#define XL7_GRAPHICS_SHADERS_CONSTANTBUFFER_H
#include "../../resources/UpdatableResource.h"

#include "./ConstantBufferDesc.h"
#include "./ConstantBufferUpdater.h"



namespace xl7::graphics::shaders {



class ShaderManager;



class ConstantBuffer
    : public resources::UpdatableResource<ConstantBuffer, ConstantBufferUpdater, resources::ResourceWithData<ConstantBuffer>>
{

public:
    ConstantBuffer() = delete;

    ConstantBuffer(const ConstantBuffer&) = delete;
    ConstantBuffer& operator=(const ConstantBuffer&) = delete;
    ConstantBuffer(ConstantBuffer&&) = delete;
    ConstantBuffer& operator=(ConstantBuffer&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"constant buffer"; }

    /**
     * Returns the descriptor of the constant buffer.
     */
    const ConstantBufferDesc& get_desc() const { return _desc; }



protected:

    ConstantBuffer(const CreateContext& ctx, const ConstantBufferDesc& desc);

    ~ConstantBuffer() override = default;



private:

    /**
     * The descriptor of the constant buffer.
     */
    const ConstantBufferDesc _desc;

}; // class ConstantBuffer



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_CONSTANTBUFFER_H
