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
class Buffer3d
{

public:
    using element_type = Telement;
    using byte_type = std::conditional_t<std::is_const_v<element_type>, const std::byte, std::byte>;
    using byte_pointer = std::conditional_t<std::is_const_v<element_type>, const std::byte*, std::byte*>;


    constexpr Buffer3d() noexcept = default;

    constexpr Buffer3d(std::span<element_type> data, size_t width, size_t height, size_t depth)
        requires(!std::is_same_v<std::remove_cv_t<element_type>, std::byte>)
        : Buffer3d({reinterpret_cast<byte_pointer>(data.data()), data.size_bytes()}, width, height, depth)
    {
    }

    constexpr Buffer3d(std::span<byte_type> data, size_t width, size_t height, size_t depth, size_t element_stride = sizeof(element_type), size_t row_pitch = 0, size_t slice_pitch = 0)
        : _data(data)
        , _width(width)
        , _height(height)
        , _depth(depth)
        , _element_stride(element_stride)
        , _row_pitch(row_pitch ? row_pitch : width * element_stride)
        , _slice_pitch(slice_pitch ? slice_pitch : height * (row_pitch ? row_pitch : width * element_stride))
    {
        assert(_element_stride >= sizeof(element_type));
        assert(_element_stride % alignof(element_type) == 0);
        assert(_row_pitch >= _width * _element_stride);
        assert(_slice_pitch >= _height * _row_pitch);
        assert(size() == 0 || size_bytes() >= (_depth - 1) * _slice_pitch + (_height - 1) * _row_pitch + (_width - 1) * _element_stride + sizeof(element_type));
    }


    /** Returns the actual non-owning data buffer. */
    constexpr std::span<byte_type> data() const { return _data; }
    /** Returns the number of logical elements per row. */
    constexpr size_t width() const { return _width; }
    /** Returns the number of rows. */
    constexpr size_t height() const { return _height; }
    /** Returns the number of 2D slices. */
    constexpr size_t depth() const { return _depth; }
    /** Returns the distance between offsets of consecutive elements, in bytes. */
    constexpr size_t element_stride() const { return _element_stride; }
    /** Returns the distance between offsets of the first elements of consecutive rows, in bytes. */
    constexpr size_t row_pitch() const { return _row_pitch; }
    /** Returns the distance between offsets of the first elements of consecutive 2D slices, in bytes. */
    constexpr size_t slice_pitch() const { return _slice_pitch; }


    /** Returns the buffer size, in bytes. */
    constexpr size_t size_bytes() const noexcept { return _data.size_bytes(); }

    /** Returns the total number of elements (width times height times depth). */
    constexpr size_t size() const noexcept { return _width * _height * _depth; }


    /** Creates a 2D slice subview. */
    constexpr Buffer2d<element_type> slice(size_t slice) const
    {
        assert(slice < _depth);
        const size_t offset = slice * _slice_pitch;
        assert(offset < _data.size_bytes());
        size_t slice_bytes = _height * _row_pitch;
        if (offset + slice_bytes > _data.size_bytes())
            slice_bytes = _data.size_bytes() - offset; // excess stride of last element in last slice allowed
        byte_pointer const ptr = _data.data() + offset;
        return {{ptr, slice_bytes}, _width, _height, _element_stride, _row_pitch};
    }

    /** Creates a 1D row subview. */
    constexpr Buffer1d<element_type> row(size_t slice, size_t row) const
    {
        assert(slice < _depth);
        assert(row < _height);
        return this->slice(slice).row(row);
    }

    /** Gains access to the specified element. */
    constexpr element_type& element(size_t slice, size_t row, size_t col) const
    {
        assert(slice < _depth);
        assert(row < _height);
        assert(col < _width);
        return this->row(slice, row).element(col);
    }

    /** Gains access to the specified element. */
    constexpr element_type& element(size_t index) const
    {
        assert(index < size());
        const size_t plane = _width * _height;
        const size_t slice = index / plane;
        const size_t rem = index % plane;
        const size_t row = rem / _width;
        const size_t col = rem % _width;
        return element(slice, row, col);
    }

    /** Gains access to the specified element. */
    template <typename Tother>
        requires(!std::is_reference_v<Tother> && std::is_trivially_copyable_v<Tother> &&
            (std::is_const_v<Tother> || !std::is_const_v<element_type>))
    constexpr Tother& map_element(size_t slice, size_t row, size_t col) const
    {
        assert(sizeof(Tother) <= _element_stride);
        assert(slice < _depth);
        assert(row < _height);
        assert(col < _width);
        return this->row(slice, row).template map_element<Tother>(col);
    }

    /** Gains access to the specified element. */
    template <typename Tother>
        requires(!std::is_reference_v<Tother> && std::is_trivially_copyable_v<Tother> &&
            (std::is_const_v<Tother> || !std::is_const_v<element_type>))
    constexpr Tother& map_element(size_t index) const
    {
        assert(sizeof(Tother) <= _element_stride);
        assert(index < size());
        const size_t plane = _width * _height;
        const size_t slice = index / plane;
        const size_t rem = index % plane;
        const size_t row = rem / _width;
        const size_t col = rem % _width;
        return map_element<Tother>(slice, row, col);
    }


private:
    /** The actual non-owning data buffer. */
    std::span<byte_type> _data;
    /** The number of logical elements per row. */
    size_t _width = 0;
    /** The number of rows. */
    size_t _height = 0;
    /** The number of 2D slices. */
    size_t _depth = 0;
    /** The distance between offsets of consecutive elements, in bytes. */
    size_t _element_stride = 0;
    /** The distance between offsets of the first elements of consecutive rows, in bytes. */
    size_t _row_pitch = 0;
    /** The distance between offsets of the first elements of consecutive 2D slices, in bytes. */
    size_t _slice_pitch = 0;

}; // Buffer3d



using Buffer3dView = Buffer3d<const std::byte>;
using Buffer3dSpan = Buffer3d<std::byte>;



} // namespace dl7

#endif // DL7_BUFFER3D_H
