#ifndef XL7_GRAPHICS_IMAGES_CODECS_TARGA_WRITER_H
#define XL7_GRAPHICS_IMAGES_CODECS_TARGA_WRITER_H
#include "../../ImageWriter.h"



namespace xl7::graphics::images::codecs::targa {



class Writer
    : public ImageWriter
{

public:
    /** Defines the image origin location (coordinate 0,0) used in the output file. */
    enum struct Origin
    {
        /**
         * The origin is at the bottom-left corner of the image (i.e., scanlines are
         * stored bottom-to-top). This is the traditional TGA convention used by
         * many legacy tools.
         */
        BottomLeft,

        /**
         * The origin is at the top-left corner of the image (i.e., scanlines are
         * stored top-to-bottom). This corresponds to the layout used by most modern
         * image formats and graphics frameworks (including this one).
         */
        TopLeft,
    };

    /**
     * Options to control the behavior of the Targa image writer.
     *
     * Most image characteristics (such as color model, bit depth, and presence of
     * alpha) are derived automatically from the source image. These options only
     * influence container/encoding details that cannot be inferred.
     */
    struct Options
    {
        /** Enables Run-Length Encoding (RLE) compression. */
        bool compression = true;
        /** Defines the image origin location (coordinate 0,0) used in the output file. */
        Origin origin = Origin::TopLeft;
    };

    /** Default options with RLE compression enabled. */
    static constexpr Options DEFAULT_OPTIONS = {.compression = true, .origin = Origin::TopLeft};



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
     * Writes the Targa header.
     */
    bool _write_header(cl7::io::IWritable& writable, const cl7::u8string& target_name, const ImageDesc& image_desc);

    /**
     * Writes uncompressed image data.
     */
    static bool _write_uncompressed(cl7::io::IWritable& writable, const cl7::u8string& target_name, cl7::byte_view image_data);

    /**
     * Writes RLE-compressed image data.
     */
    static bool _write_compressed(cl7::io::IWritable& writable, const cl7::u8string& target_name, unsigned pixel_depth, cl7::byte_view image_data);



    // #############################################################################
    // Attributes
    // #############################################################################

    /** The options to control the behavior of the Targa image writer. */
    Options _options;

}; // class Writer



} // namespace xl7::graphics::images::codecs::targa

#endif // XL7_GRAPHICS_IMAGES_CODECS_TARGA_WRITER_H
