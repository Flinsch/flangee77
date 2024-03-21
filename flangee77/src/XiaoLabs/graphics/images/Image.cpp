#include "Image.h"

#include "../PixelBitKit.h"



namespace xl7 {
namespace graphics {
namespace images {



    /** Returns the size of one pixel, in bytes. */
    unsigned Image::Desc::determine_pixel_stride() const
    {
        return PixelBitKit::determine_stride( pixel_format );
    }

    /** Calculates the number of pixels of the image. */
    size_t Image::Desc::calculate_pixel_count() const
    {
        return
            static_cast<size_t>( width ) *
            static_cast<size_t>( height ) *
            static_cast<size_t>( depth );
    }

    /** Calculates the total size of the image data, in bytes. */
    size_t Image::Desc::calculate_data_size() const
    {
        return
            calculate_pixel_count() *
            determine_pixel_stride();
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an "empty" image.
     */
    Image::Image()
        : _desc( { PixelFormat::UNKNOWN, ChannelOrder::RGBA, 0, 0, 0 } )
        , _data_view()
        , _data_buffer()
    {
    }

    /**
     * Explicit constructor. Initializes an "empty" image.
     */
    Image::Image(const Desc& desc)
        : Image()
    {
        init( desc );
    }

    /**
     * Explicit constructor. If view_only is set to true, no data will be duplicated;
     * instead, the image's data view will point to the specified data view, which
     * accordingly should persist beyond the lifetime of the image.
     */
    Image::Image(const Desc& desc, cl7::byte_view data, bool view_only)
        : Image()
    {
        init( desc, data, view_only );
    }

    /**
     * Explicit constructor.
     */
    Image::Image(const Desc& desc, cl7::byte_vector&& data)
        : Image()
    {
        init( desc, std::move(data) );
    }

    /**
     * Copy constructor. Creates a buffer and duplicates the data regardless of
     * whether the other image is "view only".
     */
    Image::Image(const Image& rhs)
        : Image()
    {
        *this = rhs;
    }

    /**
     * Copy assignment operator. Creates a buffer and duplicates the data regardless
     * of whether the other image is "view only".
     */
    Image& Image::operator = (const Image& rhs)
    {
        // We don't have to explicitly check the case of self-assignment here,
        // because the init function handles this case for us accordingly.
        init( rhs._desc, rhs._data_view, false );
        return *this;
    }

    /**
     * Swap operation.
     */
    void Image::swap(Image& rhs)
    {
        std::swap( _desc, rhs._desc );
        std::swap( _data_view, rhs._data_view );
        _data_buffer.swap( rhs._data_buffer );
    }

    /**
     * Special swap operation. The image's data is essentially "exported" and then
     * remains undefined.
     */
    void Image::swap(cl7::byte_vector& data)
    {
        // If our buffer is empty ("view only"),
        // fill it now with the "viewed" data.
        if ( _data_buffer.empty() && _data_view.size() )
            _data_buffer = { _data_view.data(), _data_view.data() + _data_view.size() };

        // "Export" the data.
        _data_buffer.swap( data );

        // Our "new" buffer should either be
        // empty or of the appropriate size.
        if ( !_data_buffer.empty() )
            _data_buffer.resize( _desc.calculate_data_size() );

        // "View" our "new" buffer.
        _data_view = _data_buffer;
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * (Re)initializes an "empty" image.
     */
    bool Image::init(const Desc& desc)
    {
        _desc = desc;

        _data_view = {};
        _data_buffer.clear();

        return true;
    }

    /**
     * (Re)initializes the image based on the given data. If view_only is set to
     * true, no data will be duplicated; instead, the image's data view will point
     * to the specified data view, which accordingly should persist beyond the
     * lifetime of the image.
     */
    bool Image::init(const Desc& desc, cl7::byte_view data, bool view_only)
    {
        if ( !_validate( desc, data ) )
            return false;

        _desc = desc;

        if ( view_only )
        {
            // Just adopt the specified data view.
            // No own buffer required.
            _data_view = data;
            _data_buffer.clear();
        }
        else
        {
            // Allocate new buffer,
            // copy data (unless empty),
            // adopt new buffer,
            // and "view" our new buffer.
            // The detour via the temporary buffer is "required"
            // to correctly handle the self-assignment case.
            cl7::byte_vector data_buffer{ data.size() };
            if ( data.size() )
                ::memcpy( &data_buffer[0], &data[0], data.size() );
            _data_buffer.swap( data_buffer );
            _data_view = _data_buffer;
        }

        return true;
    }

    /**
     * (Re)initializes the image based on the given data.
     */
    bool Image::init(const Desc& desc, cl7::byte_vector&& data)
    {
        if ( !_validate( desc, data ) )
            return false;

        _desc = desc;

        // Swap data buffers and
        // "view" our new buffer.
        _data_buffer.swap( data );
        _data_view = _data_buffer;

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
        // No data means an empty image: everything is fine.
        if ( data.empty() )
            return true;

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
