#ifndef XL7_GRAPHICS_IMAGES_CODECS_PNG_WRITER_H
#define XL7_GRAPHICS_IMAGES_CODECS_PNG_WRITER_H
#include "../../ImageWriter.h"

#include "FilterEncoder.h"



namespace xl7::graphics::images::codecs::png {



class Writer
    : public ImageWriter
{

public:
    using FilterType = FilterEncoder::FilterType;

    struct Options
    {
        /** The filter type to use. */
        FilterType filter_type = FilterType::Auto;
        /** The compression level in the range 0-9 (-1 = default zlib). */
        int compression_level = -1;
    };

    /** Default options with automatic selection of the respective filter type per scanline and a default compression level. */
    static constexpr Options DEFAULT_OPTIONS = {.filter_type = FilterType::Auto, .compression_level = -1};



    explicit Writer(Options options = DEFAULT_OPTIONS) : _options(options) {}

    ~Writer() override = default;



private:

#pragma pack(push, 1)
    struct IhdrData
    {
        uint32_t width;
        uint32_t height;
        uint8_t bit_depth;
        uint8_t color_type;
        uint8_t compression_method;
        uint8_t filter_method;
        uint8_t interlace_method;
    };
#pragma pack(pop)
    static_assert(sizeof(IhdrData) == 13);



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

    static uint8_t _color_type_for(unsigned channel_count);



    // #############################################################################
    // Attributes
    // #############################################################################

    /** The options to control the behavior of the PNG image writer. */
    Options _options;

}; // class Writer



} // namespace xl7::graphics::images::codecs::png

#endif // XL7_GRAPHICS_IMAGES_CODECS_PNG_WRITER_H
