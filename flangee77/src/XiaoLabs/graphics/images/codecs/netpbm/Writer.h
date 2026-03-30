#ifndef XL7_GRAPHICS_IMAGES_CODECS_NETPBM_WRITER_H
#define XL7_GRAPHICS_IMAGES_CODECS_NETPBM_WRITER_H
#include "../../ImageWriter.h"



namespace xl7::graphics::images::codecs::netpbm {



class Writer
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

    /**
     * Options to control the behavior of the Netpbm image writer.
     *
     * Most image characteristics (such as color model, bit depth, and presence of
     * alpha) are derived automatically from the source image. These options only
     * influence container/encoding details that cannot be inferred.
     */
    struct Options
    {
        /** The (more or less) specific Netpbm image file format to write. */
        Format format = Format::Pam;
    };

    /** Default options with PAM as the output format. */
    static constexpr Options DEFAULT_OPTIONS = {.format = Format::Pam};



    explicit Writer(Options options = DEFAULT_OPTIONS) : _options(options) {}

    ~Writer() override = default;



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

    /** The options to control the behavior of the Netpbm image writer. */
    Options _options;

}; // class Writer



} // namespace xl7::graphics::images::codecs::netpbm

#endif // XL7_GRAPHICS_IMAGES_CODECS_NETPBM_WRITER_H
