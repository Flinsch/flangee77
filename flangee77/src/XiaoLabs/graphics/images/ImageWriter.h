#ifndef XL7_GRAPHICS_IMAGES_IMAGEWRITER_H
#define XL7_GRAPHICS_IMAGES_IMAGEWRITER_H

#include "./Image.h"

#include <CoreLabs/io/IWritable.h>
#include <CoreLabs/string.h>



namespace xl7::graphics::images {



class ImageWriter
{
public:
    virtual ~ImageWriter() = default;



    /**
     * "Dumps" an image to a file.
     */
    bool dump_to_file(const Image& image, const cl7::u8string& file_path);

    /**
     * "Dumps" an image to any writable object.
     */
    bool dump_to(const Image& image, cl7::io::IWritable& writable, const cl7::u8string& target_name);



private:
    /**
     * "Dumps" an image to any writable object.
     */
    virtual bool _dump_to(const Image& image, cl7::io::IWritable& writable, const cl7::u8string& target_name) = 0;

}; // class ImageWriter



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_IMAGEWRITER_H
