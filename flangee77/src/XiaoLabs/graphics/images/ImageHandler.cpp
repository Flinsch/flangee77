#include "ImageHandler.h"

#include "../PixelBitKit.h"



namespace xl7 {
namespace graphics {
namespace images {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Loads an image from a file.
     */
    bool ImageHandler::load_from_file(cl7::string_view file_path, Image& image)
    {
        if ( !_load_from_file( file_path, image ) )
            return false;

        return _validate( image );
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Performs a final check to validate the result.
     */
    bool ImageHandler::_validate(const Image& image)
    {
        PixelBitKit pbk{ image.get_pixel_format(), image.get_channel_order() };

        if ( static_cast<size_t>( image.get_width() ) * static_cast<size_t>( image.get_height() ) * static_cast<size_t>( pbk.stride ) != image.get_data().size() )
        {
            // Should we log an error message or something?
            return false;
        }

        return true;
    }



} // namespace images
} // namespace graphics
} // namespace xl7
