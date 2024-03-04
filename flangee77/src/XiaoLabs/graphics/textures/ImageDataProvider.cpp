#include "ImageDataProvider.h"



namespace xl7 {
namespace graphics {
namespace textures {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    ImageDataProvider::ImageDataProvider()
        : resources::DefaultDataProvider()
        , image_desc()
        , image_count( 0 )
    {
    }

    /**
     * Explicit constructor.
     */
    ImageDataProvider::ImageDataProvider(const images::Image* image)
        : resources::DefaultDataProvider( image ? image->get_data() : cl7::byte_view() )
        , image_desc( image ? image->get_desc() : images::Image::Desc{} )
        , image_count( image ? 1 : 0 )
    {
    }

    /**
     * Explicit constructor.
     */
    ImageDataProvider::ImageDataProvider(const images::ImageStack* image_stack)
        : resources::DefaultDataProvider( image_stack ? image_stack->get_data() : cl7::byte_view() )
        , image_desc( image_stack ? image_stack->get_image_desc() : images::Image::Desc{} )
        , image_count( image_stack ? image_stack->get_image_count() : 0 )
    {
    }



} // namespace textures
} // namespace graphics
} // namespace xl7
