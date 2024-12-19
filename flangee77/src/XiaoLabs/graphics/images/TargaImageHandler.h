#pragma once
#ifndef XL7_GRAPHICS_IMAGES_TARGAIMAGEHANDLER_H
#define XL7_GRAPHICS_IMAGES_TARGAIMAGEHANDLER_H
#include "./ImageHandler.h"



namespace xl7::graphics::images {



class TargaImageHandler
    : public ImageHandler
{

public:
    ~TargaImageHandler() override = default;



private:

#pragma pack(push, 1)
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
    };
#pragma pack(pop)
    static_assert(sizeof(Header) == 18);



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Loads an image from any rom.
     */
    bool _load_from(cl7::io::irom& rom, const cl7::string& rom_name, Image& image) override;



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Loads an uncompressed TGA.
     */
    static bool _load_uncompressed(cl7::io::irom& rom, const cl7::string& rom_name, const Header& header, const Image::Desc& desc, cl7::byte_span data);

    /**
     * Loads a compressed TGA.
     */
    static bool _load_compressed(cl7::io::irom& rom, const cl7::string& rom_name, const Header& header, const Image::Desc& desc, cl7::byte_span data);

}; // class TargaImageHandler



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_TARGAIMAGEHANDLER_H
