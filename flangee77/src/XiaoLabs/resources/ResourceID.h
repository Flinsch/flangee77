#pragma once
#ifndef XL7_RESOURCES_RESOURCEID_H
#define XL7_RESOURCES_RESOURCEID_H

#include <CoreLabs/root.h>



namespace xl7::resources {



class ResourceID
{

public:
    static const ResourceID INVALID_ID;

    using id_t = size_t;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Creates an invalid resource ID.
     */
    ResourceID() noexcept;

    ResourceID(size_t index, unsigned version);

    void swap(ResourceID& other) noexcept;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns true if the resource ID is valid, false otherwise.
     */
    bool is_valid() const noexcept;

    /**
     * Returns the technical unsigned-integer representation of the resource ID.
     */
    id_t value() const noexcept { return _id; }

    /**
     * Returns the index part of the resource ID.
     */
    size_t index() const noexcept;

    /**
     * Extracts the version from the resource ID.
     */
    unsigned version() const noexcept;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Invalidates this resource ID.
     */
    void invalidate() noexcept;



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    auto operator==(ResourceID other) const noexcept { return _id == other._id; }
    auto operator!=(ResourceID other) const noexcept { return _id != other._id; }



    // #############################################################################
    // Conversion Operators
    // #############################################################################

    operator bool () const noexcept { return is_valid(); }



    // #############################################################################
    // Hash Function
    // #############################################################################

    size_t hash() const noexcept { return static_cast<size_t>(_id); };



private:
    static_assert(sizeof(id_t) == 4 || sizeof(id_t) == 8);
    static constexpr auto TOTAL_SIZE_BITS = sizeof(id_t) * 8;
    static constexpr auto INDEX_SIZE_BITS = sizeof(id_t) == 4 ? 24 : TOTAL_SIZE_BITS / 2;
    static constexpr auto VERSION_SIZE_BITS = sizeof(id_t) * 8 - INDEX_SIZE_BITS;
    static_assert(INDEX_SIZE_BITS >= 24);
    static_assert(INDEX_SIZE_BITS <= 32);
    static_assert(VERSION_SIZE_BITS > 0);
    static constexpr id_t INDEX_MASK = (id_t{1} << INDEX_SIZE_BITS) - 1;
    static constexpr id_t VERSION_MASK = (id_t{1} << VERSION_SIZE_BITS) - 1;
    static constexpr auto VERSION_SHIFT = INDEX_SIZE_BITS;



    /**
     * The ID composed of index and version.
     */
    id_t _id;

}; // class ResourceID



} // namespace xl7::resources



template <>
struct std::hash<xl7::resources::ResourceID>
{
    size_t operator()(const xl7::resources::ResourceID& resource_id) const noexcept
    {
        return resource_id.hash();
    }
};



#endif // XL7_RESOURCES_RESOURCEID_H
