#ifndef XL7_GRAPHICS_IMAGES_IMAGEHANDLER_H
#define XL7_GRAPHICS_IMAGES_IMAGEHANDLER_H

#include "./Image.h"

#include <CoreLabs/io/IReadable.h>
#include <CoreLabs/string.h>



namespace xl7::graphics::images {



class ImageHandler
{
public:
    virtual ~ImageHandler() = default;



    /**
     * Loads an image from a file.
     */
    bool load_from_file(const cl7::u8string& file_path, Image& image);

    /**
     * Loads an image from any readable object.
     */
    bool load_from(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image);



protected:
    static bool _log_bad_format_error(const cl7::u8string& source_name);
    static bool _log_bad_header_error(const cl7::u8string& source_name);



private:
    /**
     * Loads an image from any readable object.
     */
    virtual bool _load_from(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image) = 0;

}; // class ImageHandler



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_IMAGEHANDLER_H
