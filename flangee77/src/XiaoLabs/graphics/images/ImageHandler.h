#pragma once
#ifndef XL7_GRAPHICS_IMAGES_IMAGEHANDLER_H
#define XL7_GRAPHICS_IMAGES_IMAGEHANDLER_H

#include "./Image.h"

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
    virtual ~ImageHandler(void) = default;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Loads an image from a file.
     */
    bool load_from_file(cl7::string_view file_path, Image& image);



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Loads an image from a file.
     */
    virtual bool _load_from_file(cl7::string_view file_path, Image& image) = 0;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Performs a final check to validate the result.
     */
    bool _validate(const Image& image);

}; // class ImageHandler



} // namespace images
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMAGES_IMAGEHANDLER_H
