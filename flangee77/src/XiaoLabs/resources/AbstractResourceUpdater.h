#ifndef XL7_RESOURCES_ABSTRACTAbstractResourceUpdater_H
#define XL7_RESOURCES_ABSTRACTAbstractResourceUpdater_H

#include <CoreLabs/byte_span.h>
#include <CoreLabs/byte_view.h>

#include <CoreLabs/string.h>



namespace xl7::resources {



class Resource;



/**
 * Base class guarantees:
 * - bounds safety
 * - raw memory write
 *
 * Derived classes must ensure:
 * - semantic correctness (stride, size, layout)
 * - any dirty-state updates
 */
class AbstractResourceUpdater
{
public:
    AbstractResourceUpdater() = default;

    explicit AbstractResourceUpdater(cl7::byte_span data, const Resource* resource = nullptr);

    AbstractResourceUpdater(const AbstractResourceUpdater&) = delete;
    AbstractResourceUpdater& operator=(const AbstractResourceUpdater&) = delete;
    AbstractResourceUpdater(AbstractResourceUpdater&&) = delete;
    AbstractResourceUpdater& operator=(AbstractResourceUpdater&&) = delete;

    virtual ~AbstractResourceUpdater() noexcept = default;



    /** Returns the fixed-size data buffer to be updated. */
    cl7::byte_view get_data() const { return _data; }

    /** Returns the (optional) supposedly updatable resource. */
    const Resource* get_resource() const { return _resource; }



protected:
    /** Returns the fixed-size data buffer to be updated. */
    cl7::byte_span _access_data() { return _data; }

    /**
     * Writes the given data into the internal buffer at the specified byte offset.
     * The function assumes that all semantic validations (size, stride, etc.) have
     * already been performed by the caller. Only bounds checking is enforced.
     * Returns true if the write succeeded, false otherwise.
     */
    bool _write(cl7::byte_view data, size_t offset = 0);



    /**
     * Validates that the specified data size and offset do not exceed the total
     * size of the internal buffer, and otherwise issues an error message.
     */
    bool _check_data_bounds(size_t data_size, size_t offset = 0) const;



    /**
     * Validates that the provided data size exactly matches the expected size, and
     * otherwise issues an error message.
     *
     * Typically used when the caller computes the required size explicitly (e.g.,
     * element_count * stride).
     */
    bool _check_exact_size(size_t expected_size, size_t data_size) const;

    /**
     * Validates that both offset and data size are aligned to the given stride.
     *
     * Ensures that writes operate on whole elements/chunks rather than partial ones.
     */
    bool _check_stride_alignment(size_t custom_stride, size_t data_size, size_t offset = 0) const;



    /**
     * Tries to return a "human-friendly" string that combines the resource type
     * with the resource identifier (if specified). If the resource is unknown
     * (i.e., NULL), simply "resource" is returned instead.
     */
    cl7::u8string get_qualified_identifier() const;



private:
    /** The fixed-size data buffer to be updated. */
    cl7::byte_span _data;

    /** The (optional) supposedly updatable resource. */
    const Resource* _resource = nullptr;

}; // class AbstractResourceUpdater



} // namespace xl7::resources

#endif // XL7_RESOURCES_ABSTRACTAbstractResourceUpdater_H
