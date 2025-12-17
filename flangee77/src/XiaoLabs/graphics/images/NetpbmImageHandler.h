#ifndef XL7_GRAPHICS_IMAGES_NETPBMIMAGEHANDLER_H
#define XL7_GRAPHICS_IMAGES_NETPBMIMAGEHANDLER_H
#include "./ImageHandler.h"



namespace xl7::graphics::images {



class NetpbmImageHandler
    : public ImageHandler
{

public:
    ~NetpbmImageHandler() override = default;



private:

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
     * Parses any PBM, PGM, or PPM format.
     */
    static bool _load_pnm(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image);

    /**
     * Parses the PAM format.
     */
    static bool _load_pam(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image);



    static cl7::byte_vector _read_1bit_ascii(cl7::io::IReadable& readable, const cl7::u8string& source_name, const Image::Desc& desc);
    static cl7::byte_vector _read_1bit_binary(cl7::io::IReadable& readable, const cl7::u8string& source_name, const Image::Desc& desc, bool zero_is_white);
    static cl7::byte_vector _read_ascii(cl7::io::IReadable& readable, const cl7::u8string& source_name, const Image::Desc& desc);
    static cl7::byte_vector _read_binary(cl7::io::IReadable& readable, const cl7::u8string& source_name, const Image::Desc& desc);



    static void _skip_comments(cl7::io::IReadable& readable);

}; // class NetpbmImageHandler



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_NETPBMIMAGEHANDLER_H
