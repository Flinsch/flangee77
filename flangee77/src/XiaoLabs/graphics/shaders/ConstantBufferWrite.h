#ifndef XL7_GRAPHICS_SHADERS_CONSTANTBUFFERWRITE_H
#define XL7_GRAPHICS_SHADERS_CONSTANTBUFFERWRITE_H

#include <CoreLabs/byte_view.h>



namespace xl7::graphics::shaders {



struct ConstantBufferWrite
{
    /** The provided source constant data to write. */
    cl7::byte_view data;



    template <class TData>
    static ConstantBufferWrite from_data_ptr(const TData* data)
    {
        if (!data)
            return {};

        return {
            .data = cl7::byte_view(reinterpret_cast<const std::byte*>(data), sizeof(TData)),
        };
    }
};



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_CONSTANTBUFFERWRITE_H
