#include "Texture.h"

#include "../PixelBitKit.h"



namespace xl7 {
namespace graphics {
namespace textures {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Texture::Texture(Type type, const CreateParams<Desc>& params)
        : Resource( params )
        , _type( type )
        , _desc( params.desc )
        , _stride( PixelBitKit::determine_stride( params.desc.pixel_format ) )
        , _line_pitch( _stride * params.desc.width )
        , _image_pitch( _line_pitch * params.desc.height )
        , _channel_order( params.desc.preferred_channel_order )
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
    bool Texture::_check_impl(const resources::DataProvider& data_provider)
    {
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
        return _acquire_impl( data_provider, _channel_order );
    }



} // namespace textures
} // namespace graphics
} // namespace xl7
