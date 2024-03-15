#include "Texture.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"
#include "../PixelBitKit.h"

#include "../images/ImageConverter.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace textures {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Texture::Texture(Type type, const CreateParams<Desc>& params, unsigned depth, unsigned image_count)
        : Resource( params )
        , _type( type )
        , _desc( params.desc )
        , _channel_order( GraphicsSystem::instance().get_rendering_device()->recommend_channel_order( type, params.desc.pixel_format, params.desc.preferred_channel_order ).first )
        , _depth( depth )
        , _stride( PixelBitKit::determine_stride( params.desc.pixel_format ) )
        , _line_pitch( _stride * params.desc.width )
        , _slice_pitch( _line_pitch * params.desc.height )
        , _data_size( _slice_pitch * depth * image_count )
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool Texture::_check_data_impl(const resources::DataProvider& data_provider)
    {
        assert( typeid(data_provider) == typeid(const ImageDataProvider&) );
        auto image_data_provider = static_cast<const ImageDataProvider&>( data_provider );

        if ( image_data_provider.image_desc.width != _desc.width )
        {
            LOG_ERROR( TEXT("The image width provided for ") + get_typed_identifier_string() + TEXT(" does not match the width of the ") + cl7::string(get_type_string()) + TEXT(".") );
            return false;
        }
        if ( image_data_provider.image_desc.height != _desc.height )
        {
            LOG_ERROR( TEXT("The image height provided for ") + get_typed_identifier_string() + TEXT(" does not match the height of the ") + cl7::string(get_type_string()) + TEXT(".") );
            return false;
        }
        if ( image_data_provider.image_desc.depth != _depth )
        {
            LOG_ERROR( TEXT("The image depth provided for ") + get_typed_identifier_string() + TEXT(" does not match the depth of the ") + cl7::string(get_type_string()) + TEXT(".") );
            return false;
        }

        if ( !_check_against_size( data_provider, image_data_provider.image_desc.calculate_data_size() * image_data_provider.image_count ) )
            return false;
        if ( !_check_against_stride( data_provider, image_data_provider.image_desc.determine_pixel_stride() ) )
            return false;

        return true;
    }

    /**
     * (Re)populates the local data buffer based on the given data provider.
     */
    bool Texture::_fill_data_impl(const resources::DataProvider& data_provider)
    {
        assert( typeid(data_provider) == typeid(const ImageDataProvider&) );
        auto image_data_provider = static_cast<const ImageDataProvider&>( data_provider );

        cl7::byte_vector image_data;
        data_provider.fill( image_data );

        if ( image_data_provider.image_desc.pixel_format == _desc.pixel_format && image_data_provider.image_desc.channel_order == _channel_order )
        {
            // No conversion required at all.
            image_data.swap( _data );
        }
        else
        {
            // Perform image format conversion.
            images::Image source_image( image_data_provider.image_desc, std::move(image_data) );
            images::Image target_image = images::ImageConverter().convert_image( source_image, _desc.pixel_format, _channel_order );
            target_image.swap( _data );
        }

        return true;
    }

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool Texture::_acquire_impl(const resources::DataProvider& data_provider)
    {
        assert( typeid(data_provider) == typeid(const ImageDataProvider&) );
        auto image_data_provider = static_cast<const ImageDataProvider&>( data_provider );

        return _acquire_impl( image_data_provider );
    }



} // namespace textures
} // namespace graphics
} // namespace xl7
