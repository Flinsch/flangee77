#pragma once
#ifndef CL7_BYTEARRAY_H
#define CL7_BYTEARRAY_H

#include <CoreLabs/root.h>

#include <vector>



namespace cl7 {



class bytearray
{

public:
    typedef std::vector<std::byte> container_type;

    typedef container_type::iterator iterator;
    typedef container_type::const_iterator const_iterator;
    typedef container_type::reverse_iterator reverse_iterator;
    typedef container_type::const_reverse_iterator const_reverse_iterator;



    // #############################################################################
    // Construction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    bytearray() noexcept = default;

    /**
     * Explicit constructor.
     * Constructs a byte array containing the specified number of bytes.
     */
    bytearray(const std::byte* data, size_t size);

    /**
     * Explicit constructor.
     * Constructs a byte array of specified size with every byte set to the
     * specified value.
     */
    bytearray(size_t size, std::byte value);

    /**
     * Swap operation.
     */
    void swap(bytearray& rhs) noexcept;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The actual data stored in this byte array.
     */
    container_type _data;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns a pointer to the data stored in this byte array.
     */
    constexpr std::byte* data() noexcept { return _data.data(); }

    /**
     * Returns a pointer to the data stored in this byte array.
     */
    constexpr const std::byte* data() const noexcept { return _data.data(); }

    /**
     * Returns true if this byte array has size 0; otherwise returns false.
     */
    constexpr bool empty() const noexcept { return _data.empty(); }

    /**
     * Returns the number of bytes in this byte array.
     */
    constexpr size_t size() const noexcept { return _data.size(); }

    /**
     * Returns the maximum number of bytes that this byte array can theoretically
     * hold due to known system or library implementation limitations. The container
     * is by no means guaranteed to be able to reach that size: it can still fail to
     * allocate storage at any point before that size is reached.
     */
    constexpr size_t max_size() const noexcept { return _data.max_size(); }

    /**
     * Returns the maximum number of bytes that can be stored in the byte array
     * without forcing a reallocation.
     */
    constexpr size_t capacity() const noexcept { return _data.capacity(); }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Clears the contents of this byte array.
     */
    constexpr void clear() noexcept { _data.clear(); }

    /**
     * Sets the size of this byte array to the specified value. If the new size is
     * greater than the current size, the byte array is extended with new extra
     * uninitialized bytes added to the end. If the new size is less than the
     * current size, bytes are removed from the end.
     */
    constexpr void resize(size_t size) { _data.resize( size ); }

    /**
     * Sets the size of this byte array to the specified value. If the new size is
     * greater than the current size, the byte array is extended with new extra
     * bytes added to the end, initialized to the specified value. If the new size
     * is less than the current size, bytes are removed from the end.
     */
    constexpr void resize(size_t size, std::byte value) { _data.resize( size, value ); }

    /**
     * Allocates memory for at least the specified number of bytes.
     */
    constexpr void reserve(size_t size) { _data.reserve( size ); }

    /**
     * Releases any memory not required to store this byte array's data.
     */
    constexpr void shrink_to_fit() { _data.shrink_to_fit(); }

    /**
     * Returns the first byte of this byte array. The byte array must not be empty.
     */
    constexpr std::byte front() const { return _data.front(); }

    /**
     * Returns the first byte of this byte array as a modifiable reference. The byte
     * array must not be empty.
     */
    constexpr std::byte& front() { return _data.front(); }

    /**
     * Returns the last byte of this byte array. The byte array must not be empty.
     */
    constexpr std::byte back() const { return _data.back(); }

    /**
     * Returns the last byte of this byte array as a modifiable reference. The byte
     * array must not be empty.
     */
    constexpr std::byte& back() { return _data.back(); }

    /**
     * Returns the byte at the specified index position. The index position must be
     * valid (i.e., 0 <= i < size()).
     */
    constexpr std::byte at(size_t i) const { return _data.at( i ); }

    /**
     * Returns the byte at the specified index position as a modifiable reference.
     * The index position must be valid (i.e., 0 <= i < size()).
     */
    constexpr std::byte& at(size_t i) { return _data.at( i ); }

    /**
     * Returns a byte array containing the specified number of bytes from this byte
     * array, starting at the specified index position. If the specified range
     * exceeds the size of this byte array, the resulting byte array will be cropped
     * accordingly.
     */
    bytearray mid(size_t i, size_t size) const;

    /**
     * Sets every byte in this byte array to the specified value. The byte array is
     * resized beforehand (if the specified size differs).
     */
    constexpr bytearray& fill(std::byte value, size_t size);

    /**
     * Sets the bytes from the specified begin position up to (but not including)
     * the specified end position. If the specified range exceeds the end of this
     * byte array, the byte array is resized beforehand. The specified range must be
     * valid (i.e., begin <= end).
     */
    constexpr bytearray& fill(std::byte value, size_t begin, size_t end);

    /**
     * Sets the bytes of this byte array to the values of the specified data buffer.
     * The byte array is resized beforehand (if the specified size differs).
     */
    constexpr bytearray& fill(const std::byte* data, size_t size);

    /**
     * Appends the specified byte array to this byte array.
     */
    constexpr bytearray& append(const bytearray& ba);

    /**
     * Appends the specified number of bytes to this byte array.
     */
    constexpr bytearray& append(std::byte value, size_t count);

    /**
     * Appends the specified number of bytes to this byte array.
     */
    constexpr bytearray& append(const std::byte* data, size_t size);

    /**
     * Appends the specified byte to this byte array.
     */
    constexpr bytearray& append(std::byte value);

    /**
     * Prepends the specified byte array to this byte array.
     */
    constexpr bytearray& prepend(const bytearray& ba);

    /**
     * Prepends the specified number of bytes to this byte array.
     */
    constexpr bytearray& prepend(std::byte value, size_t count);

    /**
     * Prepends the specified number of bytes to this byte array.
     */
    constexpr bytearray& prepend(const std::byte* data, size_t size);

    /**
     * Prepends the specified byte to this byte array.
     */
    constexpr bytearray& prepend(std::byte value);

    /**
     * Inserts another byte array at the specified index position.
     */
    constexpr bytearray& insert(size_t i, const bytearray& ba);

    /**
     * Inserts a number of bytes at the specified index position.
     */
    constexpr bytearray& insert(size_t i, std::byte value, size_t count);

    /**
     * Inserts a number of bytes at the specified index position.
     */
    constexpr bytearray& insert(size_t i, const std::byte* data, size_t size);

    /**
     * Inserts one byte at the specified index position.
     */
    constexpr bytearray& insert(size_t i, std::byte value);

    /**
     * Removes the specified number of bytes from this byte array, starting at the
     * specified index position.
     */
    constexpr bytearray& remove(size_t i, size_t count = 1) noexcept;

    /**
     * Removes the specified byte from this byte array.
     */
    constexpr iterator remove(const_iterator pos) noexcept;

    /**
     * Removes the specified range of bytes from this byte array.
     */
    constexpr iterator remove(const_iterator begin, const_iterator end) noexcept;

    /**
     * Reverses the order of the bytes in this byte array.
     */
    constexpr void reverse() noexcept;



    // #############################################################################
    // Access Operators
    // #############################################################################
public:
    /**
     * Returns the byte at the specified index position. The index position must be
     * valid (i.e., 0 <= i < size()). Same as at(i).
     */
    constexpr std::byte operator [] (size_t i) const { return _data[ i ]; }

    /**
     * Returns the byte at the specified index position as a modifiable reference.
     * If an assignment is made beyond the end of the byte array, the array is
     * extended beforehand.
     */
    constexpr std::byte& operator [] (size_t i);



    // #############################################################################
    // Comparison Operator(s)
    // #############################################################################
public:
    /**
     * Compares the contents of this byte array and the specified byte array
     * lexicographically.
     */
    constexpr auto operator <=> (const bytearray& rhs) const { return _data <=> rhs._data; }



    // #############################################################################
    // Iterators
    // #############################################################################
public:
    constexpr auto begin() noexcept { return _data.begin(); }
    constexpr auto begin() const noexcept { return _data.begin(); }
    constexpr auto end() noexcept { return _data.end(); }
    constexpr auto end() const noexcept { return _data.end(); }

    constexpr auto rbegin() noexcept { return _data.rbegin(); }
    constexpr auto rbegin() const noexcept { return _data.rbegin(); }
    constexpr auto rend() noexcept { return _data.rend(); }
    constexpr auto rend() const noexcept { return _data.rend(); }

    constexpr auto cbegin() const noexcept { return begin(); }
    constexpr auto cend() const noexcept { return end(); }
    constexpr auto crbegin() const noexcept { return rbegin(); }
    constexpr auto crend() const noexcept { return rend(); }

}; // class bytearray



} // namespace cl7

#endif // CL7_BYTEARRAY_H
