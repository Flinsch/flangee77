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
class Buffer1d
{

public:
    using element_type = Telement;
    using byte_type = std::conditional_t<std::is_const_v<element_type>, const std::byte, std::byte>;
    using byte_pointer = std::conditional_t<std::is_const_v<element_type>, const std::byte*, std::byte*>;


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
        : _data(data)
        , _length(length)
        , _element_stride(element_stride)
    {
        assert(_element_stride >= sizeof(element_type));
        assert(_element_stride % alignof(element_type) == 0);
        assert(size() == 0 || size_bytes() >= (_length - 1) * _element_stride + sizeof(element_type));
    }


    /** Returns the actual non-owning data buffer. */
    constexpr std::span<byte_type> data() const { return _data; }
    /** Returns the number of logical elements. */
    constexpr size_t length() const { return _length; }
    /** Returns the distance between offsets of consecutive elements, in bytes. */
    constexpr size_t element_stride() const { return _element_stride; }


    /** Returns the buffer size, in bytes. */
    constexpr size_t size_bytes() const noexcept { return _data.size_bytes(); }

    /** Returns the total number of elements (simply the 1-dimensional length). */
    constexpr size_t size() const noexcept { return _length; }


    /** Gains access to the specified element. */
    constexpr element_type& element(size_t index) const
    {
        assert(index < size());
        const size_t offset = index * _element_stride;
        assert(offset + sizeof(element_type) <= _data.size_bytes());
        byte_pointer const ptr = _data.data() + offset;
        assert(reinterpret_cast<uintptr_t>(ptr) % alignof(element_type) == 0);
        return *reinterpret_cast<element_type*>(ptr);
    }

    /** Gains access to the specified element. */
    template <typename Tother>
        requires(!std::is_reference_v<Tother> && std::is_trivially_copyable_v<Tother> &&
            (std::is_const_v<Tother> || !std::is_const_v<element_type>))
    constexpr Tother& map_element(size_t index) const
    {
        assert(sizeof(Tother) <= _element_stride);
        assert(index < size());
        const size_t offset = index * _element_stride;
        assert(offset + sizeof(Tother) <= _data.size_bytes());
        byte_pointer const ptr = _data.data() + offset;
        assert(reinterpret_cast<uintptr_t>(ptr) % alignof(Tother) == 0);
        return *reinterpret_cast<Tother*>(ptr);
    }


private:
    /** The actual non-owning data buffer. */
    std::span<byte_type> _data;
    /** The number of logical elements. */
    size_t _length = 0;
    /** The distance between offsets of consecutive elements, in bytes. */
    size_t _element_stride = 0;

}; // class Buffer1d



using Buffer1dView = Buffer1d<const std::byte>;
using Buffer1dSpan = Buffer1d<std::byte>;



} // namespace dl7

#endif // DL7_BUFFER1D_H
