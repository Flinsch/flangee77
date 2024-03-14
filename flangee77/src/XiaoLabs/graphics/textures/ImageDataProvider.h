#pragma once
#ifndef XL7_GRAPHICS_TEXTURES_IMAGEDATAPROVIDER_H
#define XL7_GRAPHICS_TEXTURES_IMAGEDATAPROVIDER_H
#include "../../resources/DefaultDataProvider.h"

#include "../images/Image.h"
#include "../images/ImageStack.h"



namespace xl7 {
namespace graphics {
namespace textures {



class ImageDataProvider
    : public resources::DefaultDataProvider
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    ImageDataProvider();

    /**
     * Explicit constructor.
     */
    ImageDataProvider(const images::Image* image);

    /**
     * Explicit constructor.
     */
    ImageDataProvider(const images::ImageStack* image_stack);

    /**
     * Destructor.
     */
    virtual ~ImageDataProvider() = default;



    // #############################################################################
    // Attributes
    // #############################################################################
public:
    /** The descriptor of the image(s). */
    const images::Image::Desc image_desc;
    /** The number of provided images. */
    const unsigned image_count;

}; // class ImageDataProvider



} // namespace textures
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_TEXTURES_IMAGEDATAPROVIDER_H
