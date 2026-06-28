#ifndef XL7_GRAPHICS_IMAGES_CODECS_TARGA_RLEENCODER_H
#define XL7_GRAPHICS_IMAGES_CODECS_TARGA_RLEENCODER_H

#include <CoreLabs/io/IWritable.h>



namespace xl7::graphics::images::codecs::targa {



class RleEncoder
{

public:
    enum struct Result
    {
        Success = 0,
        BadPixelDepth,
        BadInputBufferSize,
        WriteError,
    };



    static Result encode(cl7::io::IWritable& writable, unsigned pixel_depth, cl7::byte_view input);



private:
    static size_t _count_raw_run(cl7::byte_view input, size_t start, size_t bytes_per_pixel);
    static size_t _count_rle_run(cl7::byte_view input, size_t start, size_t bytes_per_pixel);

}; // class RleEncoder



} // namespace xl7::graphics::images::codecs::targa

#endif // XL7_GRAPHICS_IMAGES_CODECS_TARGA_RLEENCODER_H
