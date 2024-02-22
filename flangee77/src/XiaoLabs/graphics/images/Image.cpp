#include "Image.h"

#include "../PixelBitKit.h"



namespace xl7 {
namespace graphics {
namespace images {



    /** Returns the size of one pixel, in bytes. */
    unsigned Image::Desc::determine_pixel_stride() const
    {
        PixelBitKit pbk{ pixel_format, channel_order };

        return pbk.stride;
    }

    /** Calculates the total size of the image data, in bytes. */
    size_t Image::Desc::calculate_data_size() const
    {
        return
            static_cast<size_t>( width ) *
            static_cast<size_t>( height ) *
            static_cast<size_t>( determine_pixel_stride() );
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    Image::Image()
        : _desc( { PixelFormat::UNKNOWN, ChannelOrder::RGBA, 0, 0 } )
        , _data()
    {
    }

    /**
     * Explicit constructor.
     */
    Image::Image(const Desc& desc, cl7::byte_view data)
        : Image()
    {
        init( desc, data );
    }

    /**
     * Explicit constructor.
     */
    Image::Image(const Desc& desc, cl7::byte_vector&& data)
        : Image()
    {
        init( desc, std::move(data) );
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * (Re)initializes the image.
     */
    bool Image::init(const Desc& desc, cl7::byte_view data)
    {
        if ( !_validate( desc, data ) )
            return false;

        _desc = desc;

        _data.resize( data.size() );
        ::memcpy( &_data[0], &data[0], data.size() );

        return true;
    }

    /**
     * (Re)initializes the image.
     */
    bool Image::init(const Desc& desc, cl7::byte_vector&& data)
    {
        if ( !_validate( desc, data ) )
            return false;

        _desc = desc;

        _data.swap( data );

        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Validates the initialization data (not in terms of content, but only roughly
     * with regard to technical aspects).
     */
    bool Image::_validate(const Desc& desc, cl7::byte_view data)
    {
        if ( desc.calculate_data_size() != data.size() )
        {
            // Should we log an error message or something?
            return false;
        }

        return true;
    }



} // namespace images
} // namespace graphics
} // namespace xl7
