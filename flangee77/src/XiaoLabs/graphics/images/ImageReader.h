#ifndef XL7_GRAPHICS_IMAGES_IMAGEREADER_H
#define XL7_GRAPHICS_IMAGES_IMAGEREADER_H

#include "./Image.h"

#include <CoreLabs/io/IReadable.h>
#include <CoreLabs/string.h>



namespace xl7::graphics::images {



class ImageReader
{
public:
    virtual ~ImageReader() = default;



    /**
     * Loads an image from a file.
     */
    bool load_from_file(const cl7::u8string& file_path, Image& image);

    /**
     * Loads an image from any readable object.
     */
    bool load_from(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image);



protected:
    static bool _log_unknown_format_error(const cl7::u8string& source_name, const cl7::u8string& description = u8"");
    static bool _log_unsupported_format_error(const cl7::u8string& source_name, const cl7::u8string& description = u8"");
    static bool _log_bad_header_error(const cl7::u8string& source_name, const cl7::u8string& description = u8"");
    static bool _log_bad_data_error(const cl7::u8string& source_name, const cl7::u8string& description = u8"");



private:
    /**
     * Loads an image from any readable object.
     */
    virtual bool _load_from(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image) = 0;

}; // class ImageReader



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_IMAGEREADER_H
