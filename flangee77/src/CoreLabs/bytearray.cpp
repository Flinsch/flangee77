#include "bytearray.h"



namespace cl7 {



    // #############################################################################
    // Construction
    // #############################################################################

    /**
     * Explicit constructor.
     * Constructs a byte array containing the specified number of bytes.
     */
    bytearray::bytearray(const std::byte* data, size_t size)
        : bytearray()
    {
        fill( data, size );
    }

    /**
     * Explicit constructor.
     * Constructs a byte array of specified size with every byte set to the
     * specified value.
     */
    bytearray::bytearray(size_t size, std::byte value)
        : bytearray()
    {
        fill( value, size );
    }

    /**
     * Swap operation.
     */
    void bytearray::swap(bytearray& rhs) noexcept
    {
        _data.swap( rhs._data );
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Returns a byte array containing the specified number of bytes from this byte
     * array, starting at the specified index position. If the specified range
     * exceeds the size of this byte array, the resulting byte array will be cropped
     * accordingly.
     */
    bytearray bytearray::mid(size_t i, size_t size) const
    {
        const size_t _size = _data.size();

        if ( i >= _size )
            return bytearray();

        if ( i + size >= _size )
            size = _size - i;

        return bytearray( _data.data() + i, size );
    }

    /**
     * Sets every byte in this byte array to the specified value. The byte array is
     * resized beforehand (if the specified size differs).
     */
    constexpr void bytearray::fill(std::byte value, size_t size)
    {
        if ( size != _data.size() )
            resize( size );

        std::fill_n( _data.begin(), size, value );
    }

    /**
     * Sets the bytes from the specified begin position up to (but not including)
     * the specified end position. If the specified range exceeds the end of this
     * byte array, the byte array is resized beforehand. The specified range must be
     * valid (i.e., begin <= end).
     */
    constexpr void bytearray::fill(std::byte value, size_t begin, size_t end)
    {
        assert( begin <= end );

        if ( end > _data.size() )
            resize( end );

        std::fill_n( _data.begin() + begin, end - begin, value );
    }

    /**
     * Sets the bytes of this byte array to the values of the specified data buffer.
     * The byte array is resized beforehand (if the specified size differs).
     */
    constexpr void bytearray::fill(const std::byte* data, size_t size)
    {
        if ( size != _data.size() )
            resize( size );

        std::copy_n( data, size, _data.begin() );
    }

    /**
     * Appends the specified byte array to this byte array.
     */
    constexpr void bytearray::append(const bytearray& ba)
    {
        _data.insert( _data.end(), ba._data.begin(), ba._data.end() );
    }

    /**
     * Appends the specified number of bytes to this byte array.
     */
    constexpr void bytearray::append(std::byte value, size_t count)
    {
        _data.insert( _data.end(), count, value );
    }

    /**
     * Appends the specified number of bytes to this byte array.
     */
    constexpr void bytearray::append(const std::byte* data, size_t size)
    {
        _data.insert( _data.end(), data, data + size );
    }

    /**
     * Appends the specified byte to this byte array.
     */
    constexpr void bytearray::append(std::byte value)
    {
        _data.push_back( value );
    }

    /**
     * Prepends the specified byte array to this byte array.
     */
    constexpr void bytearray::prepend(const bytearray& ba)
    {
        _data.insert( _data.begin(), ba._data.begin(), ba._data.end() );
    }

    /**
     * Prepends the specified number of bytes to this byte array.
     */
    constexpr void bytearray::prepend(std::byte value, size_t count)
    {
        _data.insert( _data.begin(), count, value );
    }

    /**
     * Prepends the specified number of bytes to this byte array.
     */
    constexpr void bytearray::prepend(const std::byte* data, size_t size)
    {
        _data.insert( _data.begin(), data, data + size );
    }

    /**
     * Prepends the specified byte to this byte array.
     */
    constexpr void bytearray::prepend(std::byte value)
    {
        _data.insert( _data.begin(), value );
    }

    /**
     * Inserts another byte array at the specified index position.
     */
    constexpr void bytearray::insert(size_t i, const bytearray& ba)
    {
        if ( i > _data.size() )
            resize( i );

        _data.insert( _data.begin() + i, ba._data.begin(), ba._data.end() );
    }

    /**
     * Inserts a number of bytes at the specified index position.
     */
    constexpr void bytearray::insert(size_t i, std::byte value, size_t count)
    {
        if ( i > _data.size() )
            resize( i );

        _data.insert( _data.begin() + i, count, value );
    }

    /**
     * Inserts a number of bytes at the specified index position.
     */
    constexpr void bytearray::insert(size_t i, const std::byte* data, size_t size)
    {
        if ( i > _data.size() )
            resize( i );

        _data.insert( _data.begin() + i, data, data + size );
    }

    /**
     * Inserts one byte at the specified index position.
     */
    constexpr void bytearray::insert(size_t i, std::byte value)
    {
        if ( i > _data.size() )
            resize( i );

        _data.insert( _data.begin() + i, value );
    }

    /**
     * Removes the specified number of bytes from this byte array, starting at the
     * specified index position.
     */
    constexpr void bytearray::remove(size_t i, size_t count) noexcept
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
     * Removes the specified byte from this byte array and returns an iterator
     * following the removed byte.
     */
    constexpr bytearray::iterator bytearray::remove(const_iterator pos) noexcept
    {
        return _data.erase( pos );
    }

    /**
     * Removes the specified range of bytes from this byte array and returns an
     * iterator following the last removed byte.
     */
    constexpr bytearray::iterator bytearray::remove(const_iterator begin, const_iterator end) noexcept
    {
        return _data.erase( begin, end );
    }

    /**
     * Reverses the order of the bytes in this byte array.
     */
    constexpr void bytearray::reverse() noexcept
    {
        std::reverse( _data.begin(), _data.end() );
    }



    // #############################################################################
    // Access Operators
    // #############################################################################

    /**
     * Returns the byte at the specified index position as a modifiable reference.
     * If an assignment is made beyond the end of the byte array, the array is
     * extended beforehand.
     */
    constexpr std::byte& bytearray::operator [] (size_t i)
    {
        if ( i >= _data.size() )
            resize( i + 1 );

        return _data[ i ];
    }



} // namespace cl7
