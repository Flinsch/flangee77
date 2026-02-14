#include "PixelFormat.h"



namespace cl7 {
    cl7::u8string to_string(xl7::graphics::PixelFormat pixel_format)
    {
        switch (pixel_format)
        {
#define DEFINE_CASE(name) case xl7::graphics::PixelFormat::name: return u8"" #name
            DEFINE_CASE(UNKNOWN);

            DEFINE_CASE(R8_UNORM);
            DEFINE_CASE(R8_SNORM);
            DEFINE_CASE(R8_UINT);
            DEFINE_CASE(R8_SINT);

            DEFINE_CASE(R16_UNORM);
            DEFINE_CASE(R16_SNORM);
            DEFINE_CASE(R16_UINT);
            DEFINE_CASE(R16_SINT);
            DEFINE_CASE(R16_FLOAT);

            DEFINE_CASE(R32_UINT);
            DEFINE_CASE(R32_SINT);
            DEFINE_CASE(R32_FLOAT);

            DEFINE_CASE(R8G8_UNORM);
            DEFINE_CASE(R8G8_SNORM);
            DEFINE_CASE(R8G8_UINT);
            DEFINE_CASE(R8G8_SINT);

            DEFINE_CASE(R16G16_UNORM);
            DEFINE_CASE(R16G16_SNORM);
            DEFINE_CASE(R16G16_UINT);
            DEFINE_CASE(R16G16_SINT);
            DEFINE_CASE(R16G16_FLOAT);

            DEFINE_CASE(R32G32_UINT);
            DEFINE_CASE(R32G32_SINT);
            DEFINE_CASE(R32G32_FLOAT);

            DEFINE_CASE(R4G4B4X4_UNORM);
            DEFINE_CASE(R5G5B5X1_UNORM);
            DEFINE_CASE(R5G6B5_UNORM);

            DEFINE_CASE(R8G8B8_UNORM);
            DEFINE_CASE(R8G8B8_SNORM);
            DEFINE_CASE(R8G8B8_UINT);
            DEFINE_CASE(R8G8B8_SINT);

            DEFINE_CASE(R8G8B8X8_UNORM);

            DEFINE_CASE(R11G11B10_FLOAT);

            DEFINE_CASE(R16G16B16_UNORM);
            DEFINE_CASE(R16G16B16_FLOAT);

            DEFINE_CASE(R32G32B32_UINT);
            DEFINE_CASE(R32G32B32_SINT);
            DEFINE_CASE(R32G32B32_FLOAT);

            DEFINE_CASE(R4G4B4A4_UNORM);
            DEFINE_CASE(R5G5B5A1_UNORM);

            DEFINE_CASE(R8G8B8A8_UNORM);
            DEFINE_CASE(R8G8B8A8_SNORM);
            DEFINE_CASE(R8G8B8A8_UINT);
            DEFINE_CASE(R8G8B8A8_SINT);

            DEFINE_CASE(R10G10B10A2_UNORM);
            DEFINE_CASE(R10G10B10A2_UINT);

            DEFINE_CASE(R16G16B16A16_UNORM);
            DEFINE_CASE(R16G16B16A16_SNORM);
            DEFINE_CASE(R16G16B16A16_UINT);
            DEFINE_CASE(R16G16B16A16_SINT);
            DEFINE_CASE(R16G16B16A16_FLOAT);

            DEFINE_CASE(R32G32B32A32_UINT);
            DEFINE_CASE(R32G32B32A32_SINT);
            DEFINE_CASE(R32G32B32A32_FLOAT);

            DEFINE_CASE(A8_UNORM);
#undef DEFINE_CASE
        default:
            assert(false);
        }

        return u8"UNKNOWN";
    }
}
