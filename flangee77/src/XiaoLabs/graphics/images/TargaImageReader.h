#ifndef XL7_GRAPHICS_IMAGES_TARGAIMAGEREADER_H
#define XL7_GRAPHICS_IMAGES_TARGAIMAGEREADER_H
#include "./ImageReader.h"



namespace xl7::graphics::images {



class TargaImageReader
    : public ImageReader
{

public:
    ~TargaImageReader() override = default;



private:

#pragma pack(push, 1)
    struct Header
    {
        uint8_t     id_length;
        uint8_t     color_map_type;
        uint8_t     image_type;
        uint16_t    color_map_start;
        uint16_t    color_map_length;
        uint8_t     color_map_depth;
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
     * Loads an image from any readable object.
     */
    bool _load_from(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image) override;



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Reads (and validates) the Targa header.
     */
    static bool _read_header(cl7::io::IReadable& readable, const cl7::u8string& source_name, Header& header);

    /**
     * Reads the color map.
     */
    static bool _read_color_map(cl7::io::IReadable& readable, const cl7::u8string& source_name, cl7::byte_span color_map);

    /**
     * Reads uncompressed image data.
     */
    static bool _read_uncompressed(cl7::io::IReadable& readable, const cl7::u8string& source_name, cl7::byte_span image_data);

    /**
     * Reads RLE-compressed image data.
     */
    static bool _read_compressed(cl7::io::IReadable& readable, const cl7::u8string& source_name, unsigned pixel_depth, cl7::byte_span image_data);

    /**
     * Translates the color indices to actual color values from the color map.
     */
    static bool _map_color_data(const cl7::u8string& source_name, cl7::byte_view color_map, cl7::byte_view index_data, cl7::byte_span color_data);

    /**
     * Flips the image vertically.
     */
    static void _flip_vertically(cl7::byte_span image_data, unsigned width, unsigned height);

}; // class TargaImageReader



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_TARGAIMAGEREADER_H
