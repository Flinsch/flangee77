#pragma once
#ifndef XL7_GRAPHICS_IMAGES_TARGAIMAGEHANDLER_H
#define XL7_GRAPHICS_IMAGES_TARGAIMAGEHANDLER_H
#include "./ImageHandler.h"



namespace xl7 {
namespace graphics {
namespace images {



class TargaImageHandler
    : public ImageHandler
{

private:
#pragma pack( push, 1 )
    struct Header
    {
        uint8_t     id_length;
        uint8_t     color_map_type;
        uint8_t     image_type;
        uint16_t    map_start;
        uint16_t    map_length;
        uint8_t     map_depth;
        uint16_t    x_origin;
        uint16_t    y_origin;
        uint16_t    width;
        uint16_t    height;
        uint8_t     pixel_depth;
        uint8_t     image_descriptor;
    }; // struct Header
#pragma pack( pop )
    static_assert( sizeof(Header) == 18 );



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Destructor.
     */
    virtual ~TargaImageHandler() = default;



    // #############################################################################
    // Implementations
    // #############################################################################
private:
    /**
     * Loads an image from any rom.
     */
    virtual bool _load_from(cl7::io::irom& rom, cl7::string_view rom_name, Image& image) override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Loads an uncompressed TGA.
     */
    bool _load_uncompressed(cl7::io::irom& rom, cl7::string_view rom_name, const Header& header, const Image::Desc& desc, cl7::byte_span data);

    /**
     * Loads a compressed TGA.
     */
    bool _load_compressed(cl7::io::irom& rom, cl7::string_view rom_name, const Header& header, const Image::Desc& desc, cl7::byte_span data);

}; // class TargaImageHandler



} // namespace images
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMAGES_TARGAIMAGEHANDLER_H
