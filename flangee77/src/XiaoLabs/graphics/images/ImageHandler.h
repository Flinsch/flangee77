#pragma once
#ifndef XL7_GRAPHICS_IMAGES_IMAGEHANDLER_H
#define XL7_GRAPHICS_IMAGES_IMAGEHANDLER_H

#include "./Image.h"

#include <CoreLabs/io/irom.h>
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
     * Loads an image from any rom.
     */
    bool load_from(cl7::io::irom& rom, const cl7::u8string& rom_name, Image& image);



protected:
    static bool _log_bad_format_error(const cl7::u8string& rom_name);
    static bool _log_bad_header_error(const cl7::u8string& rom_name);



private:
    /**
     * Loads an image from any rom.
     */
    virtual bool _load_from(cl7::io::irom& rom, const cl7::u8string& rom_name, Image& image) = 0;

}; // class ImageHandler



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_IMAGEHANDLER_H
