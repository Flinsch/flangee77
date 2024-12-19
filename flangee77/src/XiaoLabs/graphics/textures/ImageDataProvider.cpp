#include "ImageDataProvider.h"



namespace xl7::graphics::textures {



    ImageDataProvider::ImageDataProvider()
        : _image_desc()
        , _image_count(0)
    {
    }

    ImageDataProvider::ImageDataProvider(const images::Image* image)
        : resources::DefaultDataProvider(image ? image->get_data() : cl7::byte_view())
        , _image_desc(image ? image->get_desc() : images::Image::Desc{})
        , _image_count(image ? 1 : 0)
    {
    }

    ImageDataProvider::ImageDataProvider(const images::ImageStack* image_stack)
        : resources::DefaultDataProvider(image_stack ? image_stack->get_data() : cl7::byte_view())
        , _image_desc(image_stack ? image_stack->get_image_desc() : images::Image::Desc{})
        , _image_count(image_stack ? image_stack->get_image_count() : 0)
    {
    }



} // namespace xl7::graphics::textures
