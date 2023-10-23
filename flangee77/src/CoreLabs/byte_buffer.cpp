#include "byte_buffer.h"



namespace cl7 {



    // #############################################################################
    // Construction
    // #############################################################################

    /**
     * Explicit constructor.
     * Constructs a byte buffer containing the specified number of bytes.
     */
    byte_buffer::byte_buffer(const std::byte* data, size_t size)
        : byte_buffer()
    {
        fill( data, size );
    }

    /**
     * Explicit constructor.
     * Constructs a byte buffer of specified size with every byte set to the
     * specified value.
     */
    byte_buffer::byte_buffer(size_t size, std::byte value)
        : byte_buffer()
    {
        fill( value, size );
    }

    /**
     * Swap operation.
     */
    void byte_buffer::swap(byte_buffer& rhs) noexcept
    {
        _data.swap( rhs._data );
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Returns a byte buffer containing the specified number of bytes from this byte
     * buffer, starting at the specified index position. If the specified range
     * exceeds the size of this byte buffer, the resulting byte buffer will be
     * cropped accordingly.
     */
    byte_buffer byte_buffer::mid(size_t i, size_t size) const
    {
        const size_t _size = _data.size();

        if ( i >= _size )
            return byte_buffer();

        if ( i + size >= _size )
            size = _size - i;

        return byte_buffer( _data.data() + i, size );
    }

    /**
     * Sets every byte in this byte buffer to the specified value. The byte buffer
     * is resized beforehand (if the specified size differs).
     */
    constexpr void byte_buffer::fill(std::byte value, size_t size)
    {
        if ( size != _data.size() )
            resize( size );

        std::fill_n( _data.begin(), size, value );
    }

    /**
     * Sets the bytes from the specified begin position up to (but not including)
     * the specified end position. If the specified range exceeds the end of this
     * byte buffer, the byte buffer is resized beforehand. The specified range must
     * be valid (i.e., begin <= end).
     */
    constexpr void byte_buffer::fill(std::byte value, size_t begin, size_t end)
    {
        assert( begin <= end );

        if ( end > _data.size() )
            resize( end );

        std::fill_n( _data.begin() + begin, end - begin, value );
    }

    /**
     * Sets the bytes of this byte buffer to the values of the specified data buffer.
     * The byte buffer is resized beforehand (if the specified size differs).
     */
    constexpr void byte_buffer::fill(const std::byte* data, size_t size)
    {
        if ( size != _data.size() )
            resize( size );

        std::copy_n( data, size, _data.begin() );
    }

    /**
     * Appends the specified byte buffer to this byte buffer.
     */
    constexpr void byte_buffer::append(const byte_buffer& byb)
    {
        _data.insert( _data.end(), byb._data.begin(), byb._data.end() );
    }

    /**
     * Appends the specified number of bytes to this byte buffer.
     */
    constexpr void byte_buffer::append(std::byte value, size_t count)
    {
        _data.insert( _data.end(), count, value );
    }

    /**
     * Appends the specified number of bytes to this byte buffer.
     */
    constexpr void byte_buffer::append(const std::byte* data, size_t size)
    {
        _data.insert( _data.end(), data, data + size );
    }

    /**
     * Appends the specified byte to this byte buffer.
     */
    constexpr void byte_buffer::append(std::byte value)
    {
        _data.push_back( value );
    }

    /**
     * Prepends the specified byte buffer to this byte buffer.
     */
    constexpr void byte_buffer::prepend(const byte_buffer& byb)
    {
        _data.insert( _data.begin(), byb._data.begin(), byb._data.end() );
    }

    /**
     * Prepends the specified number of bytes to this byte buffer.
     */
    constexpr void byte_buffer::prepend(std::byte value, size_t count)
    {
        _data.insert( _data.begin(), count, value );
    }

    /**
     * Prepends the specified number of bytes to this byte buffer.
     */
    constexpr void byte_buffer::prepend(const std::byte* data, size_t size)
    {
        _data.insert( _data.begin(), data, data + size );
    }

    /**
     * Prepends the specified byte to this byte buffer.
     */
    constexpr void byte_buffer::prepend(std::byte value)
    {
        _data.insert( _data.begin(), value );
    }

    /**
     * Inserts another byte buffer at the specified index position.
     */
    constexpr void byte_buffer::insert(size_t i, const byte_buffer& byb)
    {
        if ( i > _data.size() )
            resize( i );

        _data.insert( _data.begin() + i, byb._data.begin(), byb._data.end() );
    }

    /**
     * Inserts a number of bytes at the specified index position.
     */
    constexpr void byte_buffer::insert(size_t i, std::byte value, size_t count)
    {
        if ( i > _data.size() )
            resize( i );

        _data.insert( _data.begin() + i, count, value );
    }

    /**
     * Inserts a number of bytes at the specified index position.
     */
    constexpr void byte_buffer::insert(size_t i, const std::byte* data, size_t size)
    {
        if ( i > _data.size() )
            resize( i );

        _data.insert( _data.begin() + i, data, data + size );
    }

    /**
     * Inserts one byte at the specified index position.
     */
    constexpr void byte_buffer::insert(size_t i, std::byte value)
    {
        if ( i > _data.size() )
            resize( i );

        _data.insert( _data.begin() + i, value );
    }

    /**
     * Removes the specified number of bytes from this byte buffer, starting at the
     * specified index position.
     */
    constexpr void byte_buffer::remove(size_t i, size_t count) noexcept
    {
        const size_t _size = _data.size();

        if ( i < _size )
        {
            if ( i + count > _size )
                count = _size - i;

            _data.erase( _data.begin() + i, _data.begin() + i + count );
        }
    }

    /**
     * Removes the specified byte from this byte buffer and returns an iterator
     * following the removed byte.
     */
    constexpr byte_buffer::iterator byte_buffer::remove(const_iterator pos) noexcept
    {
        return _data.erase( pos );
    }

    /**
     * Removes the specified range of bytes from this byte buffer and returns an
     * iterator following the last removed byte.
     */
    constexpr byte_buffer::iterator byte_buffer::remove(const_iterator begin, const_iterator end) noexcept
    {
        return _data.erase( begin, end );
    }

    /**
     * Reverses the order of the bytes in this byte buffer.
     */
    constexpr void byte_buffer::reverse() noexcept
    {
        std::reverse( _data.begin(), _data.end() );
    }



    // #############################################################################
    // Access Operators
    // #############################################################################

    /**
     * Returns the byte at the specified index position as a modifiable reference.
     * If an assignment is made beyond the end of the byte buffer, the buffer is
     * extended beforehand.
     */
    constexpr std::byte& byte_buffer::operator [] (size_t i)
    {
        if ( i >= _data.size() )
            resize( i + 1 );

        return _data[ i ];
    }



} // namespace cl7
