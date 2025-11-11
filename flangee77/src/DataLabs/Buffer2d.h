#ifndef DL7_BUFFER2D_H
#define DL7_BUFFER2D_H

#include "./Buffer1d.h"



namespace dl7 {



/**
 * Represents a non-owning view or span over contiguous or strided 2D memory buffers
 * and provides a flexible, type-safe way to access elements in 2D memory layouts,
 * including support for custom element strides and row pitches.
 */
template <typename Telement = std::byte>
    requires(!std::is_reference_v<Telement> && std::is_trivially_copyable_v<Telement>)
struct Buffer2d
{
    using element_type = Telement;
    using byte_type = std::conditional_t<std::is_const_v<element_type>, const std::byte, std::byte>;
    using byte_pointer = std::conditional_t<std::is_const_v<element_type>, const std::byte*, std::byte*>;

    /** The actual non-owning data buffer. */
    std::span<byte_type> data;
    /** The number of logical elements per row. */
    size_t width = 0;
    /** The number of rows. */
    size_t height = 0;
    /** The distance between offsets of consecutive elements, in bytes. */
    size_t element_stride = 0;
    /** The distance between offsets of the first elements of consecutive rows, in bytes. */
    size_t row_pitch = 0;

    constexpr Buffer2d() noexcept = default;

    constexpr Buffer2d(std::span<element_type> data, size_t width, size_t height)
        requires(!std::is_same_v<std::remove_cv_t<element_type>, std::byte>)
        : Buffer2d({reinterpret_cast<byte_pointer>(data.data()), data.size_bytes()}, width, height)
    {
    }

    constexpr Buffer2d(std::span<byte_type> data, size_t width, size_t height, size_t element_stride = sizeof(element_type), size_t row_pitch = 0)
        : data(data)
        , width(width)
        , height(height)
        , element_stride(element_stride)
        , row_pitch(row_pitch ? row_pitch : width * element_stride)
    {
        assert(element_stride >= sizeof(element_type));
        assert(element_stride % alignof(element_type) == 0);
        assert(this->row_pitch >= width * element_stride);
        assert(length() == 0 || data.size_bytes() >= (height - 1) * this->row_pitch + (width - 1) * element_stride + sizeof(element_type));
    }

    /** Returns the buffer size, in bytes. */
    constexpr size_t size_bytes() const noexcept { return data.size_bytes(); }

    /** Returns the total number of elements. */
    constexpr size_t length() const noexcept { return width * height; }

    /** Creates a 1D row subview. */
    constexpr Buffer1d<element_type> row(size_t row) const
    {
        assert(row < height);
        const size_t offset = row * row_pitch;
        assert(offset < data.size_bytes());
        size_t row_bytes = width * element_stride;
        if (offset + row_bytes > data.size_bytes())
            row_bytes = data.size_bytes() - offset; // excess stride of last element in last row allowed
        byte_pointer const ptr = data.data() + offset;
        return {{ptr, row_bytes}, width, element_stride};
    }

    /** Gains access to the specified element. */
    constexpr element_type& element(size_t row, size_t col) const
    {
        assert(row < height);
        assert(col < width);
        return this->row(row).element(col);
    }

    /** Gains access to the specified element. */
    constexpr element_type& element(size_t index) const
    {
        assert(index < length());
        const size_t row = index / width;
        const size_t col = index % width;
        return element(row, col);
    }

    /** Gains access to the specified element. */
    template <typename Tother>
        requires(!std::is_reference_v<Tother> && std::is_trivially_copyable_v<Tother> &&
            (std::is_const_v<Tother> || !std::is_const_v<element_type>))
    constexpr Tother& map_element(size_t row, size_t col) const
    {
        assert(sizeof(Tother) <= element_stride);
        assert(row < height);
        assert(col < width);
        return this->row(row).template map_element<Tother>(col);
    }

    /** Gains access to the specified element. */
    template <typename Tother>
        requires(!std::is_reference_v<Tother> && std::is_trivially_copyable_v<Tother> &&
            (std::is_const_v<Tother> || !std::is_const_v<element_type>))
    constexpr Tother& map_element(size_t index) const
    {
        assert(sizeof(Tother) <= element_stride);
        assert(index < length());
        const size_t row = index / width;
        const size_t col = index % width;
        return map_element<Tother>(row, col);
    }
};



using Buffer2dView = Buffer2d<const std::byte>;
using Buffer2dSpan = Buffer2d<std::byte>;



} // namespace dl7

#endif // DL7_BUFFER2D_H
