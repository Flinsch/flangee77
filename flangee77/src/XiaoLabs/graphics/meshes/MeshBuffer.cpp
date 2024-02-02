#include "MeshBuffer.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace meshes {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Updates the contents of this buffer (unless it is immutable).
     */
    bool MeshBuffer::update(const DataProvider& data_provider)
    {
        if ( _desc.usage == ResourceUsage::Immutable )
        {
            LOG_ERROR( TEXT("The immutable ") + get_typed_identifier_string() + TEXT(" cannot be updated.") );
            return false;
        }

        if ( !_try_fill_data( data_provider ) )
            return false;

        bool discard = data_provider.get_offset() == 0 && data_provider.get_size() == static_cast<size_t>( _size );
        bool no_overwrite = false;

        return _update_impl( data_provider, discard, no_overwrite );
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool MeshBuffer::_check_impl(const DataProvider& data_provider)
    {
        if ( !_check_against_size( data_provider, _size ) )
            return false;
        if ( !_check_against_stride( data_provider, _stride ) )
            return false;

        return true;
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
