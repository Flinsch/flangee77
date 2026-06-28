#ifndef XL7_GRAPHICS_IMAGES_CODECS_TARGA_RLEDECODER_H
#define XL7_GRAPHICS_IMAGES_CODECS_TARGA_RLEDECODER_H

#include <CoreLabs/io/IReadable.h>



namespace xl7::graphics::images::codecs::targa {



class RleDecoder
{

public:
    enum struct Result
    {
        Success = 0,
        BadPixelDepth,
        BadOutputBufferSize,
        BadChunkHeaderLength,
        BadRawPacketChunkDataLength,
        BadRlePacketChunkDataLength,
        BadCompressedImageDataLength,
    };



    static Result decode(cl7::io::IReadable& readable, unsigned pixel_depth, cl7::byte_span output);

}; // class RleDecoder



} // namespace xl7::graphics::images::codecs::targa

#endif // XL7_GRAPHICS_IMAGES_CODECS_TARGA_RLEDECODER_H
