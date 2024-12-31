#ifndef XL7_GRAPHICS_TEXTURES_IMAGEDATAPROVIDER_H
#define XL7_GRAPHICS_TEXTURES_IMAGEDATAPROVIDER_H
#include "../../resources/DefaultDataProvider.h"

#include "../images/Image.h"
#include "../images/ImageStack.h"



namespace xl7::graphics::textures {



class ImageDataProvider
    : public resources::DefaultDataProvider
{

public:
    ImageDataProvider();

    ImageDataProvider(const images::Image* image);
    ImageDataProvider(const images::ImageStack* image_stack);

    ~ImageDataProvider() override = default;



    /** Returns the descriptor of the image(s). */
    const images::Image::Desc& get_image_desc() const { return _image_desc; }
    /** Returns the number of provided images. */
    unsigned get_image_count() const { return _image_count; }



private:
    /** The descriptor of the image(s). */
    images::Image::Desc _image_desc;
    /** The number of provided images. */
    unsigned _image_count;

}; // class ImageDataProvider



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_IMAGEDATAPROVIDER_H
