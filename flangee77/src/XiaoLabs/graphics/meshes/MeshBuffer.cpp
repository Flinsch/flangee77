#include "MeshBuffer.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::meshes {



    /**
     * Updates the contents of this buffer (unless it is immutable).
     */
    bool MeshBuffer::update(const resources::DataProvider& data_provider)
    {
        if (_desc.usage == resources::ResourceUsage::Immutable)
        {
            LOG_ERROR(u8"The immutable " + get_typed_identifier_string() + u8" cannot be updated.");
            return false;
        }

        if (!_try_fill_data(data_provider))
            return false;

        bool discard = data_provider.get_offset() == 0 && data_provider.get_size() == static_cast<size_t>(_size);
        bool no_overwrite = false;

        return _update_impl(data_provider, discard, no_overwrite);
    }



    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool MeshBuffer::_check_data_impl(const resources::DataProvider& data_provider)
    {
        if (!_check_against_size(data_provider, _size))
            return false;
        if (!_check_against_stride(data_provider, _stride))
            return false;

        return true;
    }



} // namespace xl7::graphics::meshes
