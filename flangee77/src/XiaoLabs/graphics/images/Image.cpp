#include "Image.h"



namespace xl7 {
namespace graphics {
namespace images {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    Image::Image(void)
        : _pixel_format( PixelFormat::UNKNOWN )
        , _channel_order( ChannelOrder::RGBA )
        , _width( 0 )
        , _height( 0 )
        , _data()
    {
    }



} // namespace images
} // namespace graphics
} // namespace xl7
