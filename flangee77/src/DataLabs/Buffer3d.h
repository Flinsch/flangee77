#ifndef DL7_BUFFER3D_H
#define DL7_BUFFER3D_H

#include "./Buffer1d.h"
#include "./Buffer2d.h"



namespace dl7 {



/**
 * Represents a non-owning view or span over contiguous or strided 3D memory buffers
 * and provides a flexible, type-safe way to access elements in 3D memory layouts,
 * including support for custom element strides as well as row and slice pitches.
 */
template <typename Telement = std::byte>
    requires(!std::is_reference_v<Telement> && std::is_trivially_copyable_v<Telement>)
struct Buffer3d
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
    /** The number of 2D slices. */
    size_t depth = 0;
    /** The distance between offsets of consecutive elements, in bytes. */
    size_t element_stride = 0;
    /** The distance between offsets of the first elements of consecutive rows, in bytes. */
    size_t row_pitch = 0;
    /** The distance between offsets of the first elements of consecutive 2D slices, in bytes. */
    size_t slice_pitch = 0;

    constexpr Buffer3d() noexcept = default;

    constexpr Buffer3d(std::span<element_type> data, size_t width, size_t height, size_t depth)
        requires(!std::is_same_v<std::remove_cv_t<element_type>, std::byte>)
        : Buffer3d({reinterpret_cast<byte_pointer>(data.data()), data.size_bytes()}, width, height, depth)
    {
    }

    constexpr Buffer3d(std::span<byte_type> data, size_t width, size_t height, size_t depth, size_t element_stride = sizeof(element_type), size_t row_pitch = 0, size_t slice_pitch = 0)
        : data(data)
        , width(width)
        , height(height)
        , depth(depth)
        , element_stride(element_stride)
        , row_pitch(row_pitch ? row_pitch : width * element_stride)
        , slice_pitch(slice_pitch ? slice_pitch : height * (row_pitch ? row_pitch : width * element_stride))
    {
        assert(element_stride >= sizeof(element_type));
        assert(this->row_pitch >= width * element_stride);
        assert(this->slice_pitch >= height * this->row_pitch);
        assert(length() == 0 || data.size_bytes() >= (depth - 1) * this->slice_pitch + (height - 1) * this->row_pitch + (width - 1) * element_stride + sizeof(element_type));
    }

    /** Returns the buffer size, in bytes. */
    constexpr size_t size_bytes() const noexcept { return data.size_bytes(); }

    /** Returns the total number of elements. */
    constexpr size_t length() const noexcept { return width * height * depth; }

    /** Creates a 2D slice subview. */
    constexpr Buffer2d<element_type> slice(size_t slice) const
    {
        assert(slice < depth);
        const size_t offset = slice * slice_pitch;
        assert(offset < data.size_bytes());
        size_t slice_bytes = height * row_pitch;
        if (offset + slice_bytes > data.size_bytes())
            slice_bytes = data.size_bytes() - offset; // excess stride of last element in last slice allowed
        byte_pointer const ptr = data.data() + offset;
        return {{ptr, slice_bytes}, width, height, element_stride, row_pitch};
    }

    /** Creates a 1D row subview. */
    constexpr Buffer1d<element_type> row(size_t slice, size_t row) const
    {
        assert(slice < depth);
        assert(row < height);
        return this->slice(slice).row(row);
    }

    /** Gains access to the specified element. */
    constexpr element_type& element(size_t slice, size_t row, size_t col) const
    {
        assert(slice < depth);
        assert(row < height);
        assert(col < width);
        return this->row(slice, row).element(col);
    }

    /** Gains access to the specified element. */
    constexpr element_type& element(size_t index) const
    {
        assert(index < length());
        const size_t plane = width * height;
        const size_t slice = index / plane;
        const size_t rem = index % plane;
        const size_t row = rem / width;
        const size_t col = rem % width;
        return element(slice, row, col);
    }

    /** Gains access to the specified element. */
    template <typename Tother>
        requires(!std::is_reference_v<Tother> && std::is_trivially_copyable_v<Tother> &&
            (std::is_const_v<Tother> || !std::is_const_v<element_type>))
    constexpr Tother& map_element(size_t slice, size_t row, size_t col) const
    {
        assert(sizeof(Tother) <= element_stride);
        assert(slice < depth);
        assert(row < height);
        assert(col < width);
        return this->row(slice, row).template map_element<Tother>(col);
    }

    /** Gains access to the specified element. */
    template <typename Tother>
        requires(!std::is_reference_v<Tother> && std::is_trivially_copyable_v<Tother> &&
            (std::is_const_v<Tother> || !std::is_const_v<element_type>))
    constexpr Tother& map_element(size_t index) const
    {
        assert(sizeof(Tother) <= element_stride);
        assert(index < length());
        const size_t plane = width * height;
        const size_t slice = index / plane;
        const size_t rem = index % plane;
        const size_t row = rem / width;
        const size_t col = rem % width;
        return map_element<Tother>(slice, row, col);
    }
};



using Buffer3dView = Buffer3d<const std::byte>;
using Buffer3dSpan = Buffer3d<std::byte>;



} // namespace dl7

#endif // DL7_BUFFER3D_H
