#include "AbstractResourceUpdater.h"

#include "./Resource.h"

#include <CoreLabs/logging.h>



namespace xl7::resources {



    AbstractResourceUpdater::AbstractResourceUpdater(cl7::byte_span data, const Resource* resource)
        : _data(data)
        , _resource(resource)
    {
    }



    /**
     * Writes the given data into the internal buffer at the specified byte offset.
     * The function assumes that all semantic validations (size, stride, etc.) have
     * already been performed by the caller. Only bounds checking is enforced.
     * Returns true if the write succeeded, false otherwise.
     */
    bool AbstractResourceUpdater::_write(cl7::byte_view data, size_t offset)
    {
        if (data.empty())
            return true;

        if (!_check_data_bounds(data.size(), offset))
            return false;

        std::memcpy(_data.data() + offset, data.data(), data.size());
        return true;
    }



    /**
     * Validates that the specified data size and offset do not exceed the total
     * size of the internal buffer, and otherwise issues an error message.
     */
    bool AbstractResourceUpdater::_check_data_bounds(size_t data_size, size_t offset) const
    {
        if (offset + data_size > _data.size())
        {
            LOG_ERROR(u8"The provided data (size and/or offset) would exceed the total data size of the " + get_qualified_identifier() + u8".");
            return false;
        }

        return true;
    }



    /**
     * Validates that the provided data size exactly matches the expected size, and
     * otherwise issues an error message.
     *
     * Typically used when the caller computes the required size explicitly (e.g.,
     * element_count * stride).
     */
    bool AbstractResourceUpdater::_check_exact_size(size_t expected_size, size_t data_size) const
    {
        if (data_size != expected_size)
        {
            LOG_ERROR(u8"The provided data does not match the expected data size for updating the " + get_qualified_identifier() + u8".");
            return false;
        }

        return true;
    }

    /**
     * Validates that both offset and data size are aligned to the given stride.
     *
     * Ensures that writes operate on whole elements/chunks rather than partial ones.
     */
    bool AbstractResourceUpdater::_check_stride_alignment(size_t custom_stride, size_t data_size, size_t offset) const
    {
        assert(custom_stride != 0);

        bool is_valid = true;

        if (offset % custom_stride != 0)
        {
            LOG_ERROR(u8"The data offset provided does not match the element/chunk stride/size of the " + get_qualified_identifier() + u8".");
            is_valid = false;
        }

        if (data_size % custom_stride != 0)
        {
            LOG_ERROR(u8"The data size provided does not match the element/chunk stride/size of the " + get_qualified_identifier() + u8".");
            is_valid = false;
        }

        return is_valid;
    }



    /**
     * Tries to return a "human-friendly" string that combines the resource type
     * with the resource identifier (if specified). If the resource is unknown
     * (i.e., NULL), simply "resource" is returned instead.
     */
    cl7::u8string AbstractResourceUpdater::get_qualified_identifier() const
    {
        if (_resource)
            return _resource->get_qualified_identifier();
        return u8"resource";
    }



} // namespace xl7::resources
