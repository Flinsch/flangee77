#ifndef DL7_BUFFER1D_H
#define DL7_BUFFER1D_H

#include <CoreLabs/root.h>

#include <span>



namespace dl7 {



/**
 * Represents a non-owning view or span over contiguous or strided 1D memory buffers
 * and provides a flexible, type-safe way to access elements in 1D memory layouts,
 * including support for custom strides.
 */
template <typename Telement = std::byte>
    requires(!std::is_reference_v<Telement> && std::is_trivially_copyable_v<Telement>)
struct Buffer1d
{
    using element_type = Telement;
    using byte_type = std::conditional_t<std::is_const_v<element_type>, const std::byte, std::byte>;
    using byte_pointer = std::conditional_t<std::is_const_v<element_type>, const std::byte*, std::byte*>;

    /** The actual non-owning data buffer. */
    std::span<byte_type> data;
    /** The number of logical elements. */
    size_t length = 0;
    /** The distance between offsets of consecutive elements, in bytes. */
    size_t element_stride = 0;

    constexpr Buffer1d() noexcept = default;

    constexpr Buffer1d(std::span<element_type> data)
        requires(!std::is_same_v<std::remove_cv_t<element_type>, std::byte>)
        : Buffer1d({reinterpret_cast<byte_pointer>(data.data()), data.size_bytes()}, data.size())
    {
    }

    constexpr Buffer1d(std::span<byte_type> data)
        : Buffer1d(data, data.size_bytes() / sizeof(element_type))
    {
    }

    constexpr Buffer1d(std::span<byte_type> data, size_t length, size_t element_stride = sizeof(element_type))
        : data(data)
        , length(length)
        , element_stride(element_stride)
    {
        assert(element_stride >= sizeof(element_type));
        assert(element_stride % alignof(element_type) == 0);
        assert(length == 0 || data.size_bytes() >= (length - 1) * element_stride + sizeof(element_type));
    }

    /** Returns the buffer size, in bytes. */
    constexpr size_t size_bytes() const noexcept { return data.size_bytes(); }

    /** Gains access to the specified element. */
    constexpr element_type& element(size_t index) const
    {
        assert(index < length);
        const size_t offset = index * element_stride;
        assert(offset + sizeof(element_type) <= data.size_bytes());
        byte_pointer const ptr = data.data() + offset;
        assert(reinterpret_cast<uintptr_t>(ptr) % alignof(element_type) == 0);
        return *reinterpret_cast<element_type*>(ptr);
    }

    /** Gains access to the specified element. */
    template <typename Tother>
        requires(!std::is_reference_v<Tother> && std::is_trivially_copyable_v<Tother> &&
            (std::is_const_v<Tother> || !std::is_const_v<element_type>))
    constexpr Tother& map_element(size_t index) const
    {
        assert(sizeof(Tother) <= element_stride);
        assert(index < length);
        const size_t offset = index * element_stride;
        assert(offset + sizeof(Tother) <= data.size_bytes());
        byte_pointer const ptr = data.data() + offset;
        assert(reinterpret_cast<uintptr_t>(ptr) % alignof(Tother) == 0);
        return *reinterpret_cast<Tother*>(ptr);
    }
};



using Buffer1dView = Buffer1d<const std::byte>;
using Buffer1dSpan = Buffer1d<std::byte>;



} // namespace dl7

#endif // DL7_BUFFER1D_H
