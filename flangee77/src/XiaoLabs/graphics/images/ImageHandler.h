#pragma once
#ifndef XL7_GRAPHICS_IMAGES_IMAGEHANDLER_H
#define XL7_GRAPHICS_IMAGES_IMAGEHANDLER_H

#include "./Image.h"

#include <CoreLabs/io/irom.h>
#include <CoreLabs/string.h>



namespace xl7 {
namespace graphics {
namespace images {



class ImageHandler
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Destructor.
     */
    virtual ~ImageHandler() = default;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Loads an image from a file.
     */
    bool load_from_file(cl7::string_view file_path, Image& image);

    /**
     * Loads an image from any rom.
     */
    bool load_from(cl7::io::irom& rom, cl7::string_view rom_name, Image& image);



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Loads an image from any rom.
     */
    virtual bool _load_from(cl7::io::irom& rom, cl7::string_view rom_name, Image& image) = 0;



    // #############################################################################
    // Helpers
    // #############################################################################
protected:
    bool _log_bad_format_error(cl7::string_view rom_name);
    bool _log_bad_header_error(cl7::string_view rom_name);

}; // class ImageHandler



} // namespace images
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMAGES_IMAGEHANDLER_H
