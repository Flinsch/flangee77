#ifndef CL7_VERSIONEDID_H
#define CL7_VERSIONEDID_H

#include <CoreLabs/root.h>



namespace cl7 {



template <std::unsigned_integral T = size_t, unsigned VersionSizeBits = sizeof(T) / 2 * 8, typename TagT = void>
class VersionedId
{
    static_assert(VersionSizeBits > 0 && VersionSizeBits <= sizeof(T) / 2 * 8);

public:
    using value_type = T;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Creates an invalid ID.
     */
    constexpr VersionedId() noexcept
        : _id(INVALID_ID_VALUE)
    {
    }

    constexpr VersionedId(size_t index, unsigned version)
        : _id((static_cast<value_type>(index) & INDEX_MASK) | ((static_cast<value_type>(version) & VERSION_MASK) << VERSION_SHIFT))
    {
        assert(static_cast<value_type>(index) <= INDEX_MASK);
        assert(static_cast<value_type>(version) <= VERSION_MASK);
    }

    void swap(VersionedId& other) noexcept
    {
        std::swap(_id, other._id);
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns true if the ID is valid, false otherwise.
     */
    constexpr bool is_valid() const noexcept
    {
        return _id != INVALID_ID_VALUE;
    }

    /**
     * Returns the technical unsigned-integer representation of the ID.
     */
    constexpr value_type value() const noexcept
    {
        return _id;
    }

    /**
     * Returns the index part of the ID.
     */
    constexpr size_t index() const noexcept
    {
        return static_cast<size_t>(_id & INDEX_MASK);
    }

    /**
     * Extracts the version of the ID.
     */
    constexpr unsigned version() const noexcept
    {
        return static_cast<unsigned>((_id >> VERSION_SHIFT) & VERSION_MASK);
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Invalidates this ID.
     */
    void invalidate() noexcept
    {
        _id = INVALID_ID_VALUE;
    }



    // #############################################################################
    // Conversion Operators
    // #############################################################################

    constexpr operator bool() const noexcept { return is_valid(); }



    // #############################################################################
    // Hash Function
    // #############################################################################

    constexpr size_t hash() const noexcept { return static_cast<size_t>(_id); }



protected:
    static constexpr auto TOTAL_SIZE_BITS = sizeof(value_type) * 8;
    static constexpr auto VERSION_SIZE_BITS = VersionSizeBits;
    static constexpr auto INDEX_SIZE_BITS = TOTAL_SIZE_BITS - VERSION_SIZE_BITS;
    static_assert(VERSION_SIZE_BITS > 0);
    static_assert(VERSION_SIZE_BITS <= INDEX_SIZE_BITS);
    static constexpr value_type INDEX_MASK = (value_type{1} << INDEX_SIZE_BITS) - 1;
    static constexpr value_type VERSION_MASK = (value_type{1} << VERSION_SIZE_BITS) - 1;
    static constexpr auto VERSION_SHIFT = INDEX_SIZE_BITS;



private:
    static constexpr value_type INVALID_ID_VALUE = static_cast<value_type>(-1);



    /**
     * The ID composed of index and version.
     */
    value_type _id;

}; // class VersionedId



} // namespace cl7

#endif // CL7_VERSIONEDID_H
