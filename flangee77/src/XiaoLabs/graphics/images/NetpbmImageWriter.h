#ifndef XL7_GRAPHICS_IMAGES_NETPBMIMAGEWRITER_H
#define XL7_GRAPHICS_IMAGES_NETPBMIMAGEWRITER_H
#include "./ImageWriter.h"



namespace xl7::graphics::images {



class NetpbmImageWriter
    : public ImageWriter
{

public:
    /** The (more or less) specific Netpbm image file format to write. */
    enum struct Format
    {
        /** The "plain" (aka "ascii") version of the PBM format (black and white). */
        PbmAscii = 1,
        /** The "plain" (aka "ascii") version of the PGM format (shades of gray). */
        PgmAscii = 2,
        /** The "plain" (aka "ascii") version of the PPM format (full RGB color). */
        PpmAscii = 3,

        /** The "raw" (aka "binary") version of the PBM format (black and white). */
        PbmBinary = 4,
        /** The "raw" (aka "binary") version of the PGM format (shades of gray). */
        PgmBinary = 5,
        /** The "raw" (aka "binary") version of the PPM format (full RGB color). */
        PpmBinary = 6,

        /**
         * The PAM format as a "replacement" for all three other formats. The
         * specific format characteristics (such as depth and tuple type) are
         * derived from the image to be processed.
         */
        Pam = 7,
    };



    NetpbmImageWriter(Format format = Format::Pam) : _format(format) {}

    ~NetpbmImageWriter() override = default;



private:

    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * "Dumps" an image to any writable object.
     */
    bool _dump_to(const Image& image, cl7::io::IWritable& writable, const cl7::u8string& target_name) override;



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Writes any PBM, PGM, or PPM format.
     */
    static bool _write_pnm(const Image& image, cl7::io::IWritable& writable, const cl7::u8string& target_name, Format format);

    /**
     * Writes the PAM format.
     */
    static bool _write_pam(const Image& image, cl7::io::IWritable& writable, const cl7::u8string& target_name);



    static size_t _write_1bit_ascii(cl7::io::IWritable& writable, const cl7::u8string& target_name, const Image& image);
    static size_t _write_1bit_binary(cl7::io::IWritable& writable, const cl7::u8string& target_name, const Image& image);
    static size_t _write_ascii(cl7::io::IWritable& writable, const cl7::u8string& target_name, const Image& image);
    static size_t _write_binary(cl7::io::IWritable& writable, const cl7::u8string& target_name, const Image& image);



    // #############################################################################
    // Attributes
    // #############################################################################

    /** The (more or less) specific Netpbm image file format to write. */
    Format _format;

}; // class NetpbmImageWriter



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_NETPBMIMAGEWRITER_H
