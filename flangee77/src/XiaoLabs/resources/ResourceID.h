#pragma once
#ifndef XL7_RESOURCES_RESOURCEID_H
#define XL7_RESOURCES_RESOURCEID_H

#include <CoreLabs/root.h>



namespace xl7 {
namespace resources {



class ResourceID
{

public:
    static const ResourceID INVALID_ID;

private:
    typedef size_t id_t;
    static_assert( sizeof(id_t) == 4 || sizeof(id_t) == 8 );
    static constexpr auto TOTAL_SIZE_BITS = sizeof(id_t) * 8;
    static constexpr auto INDEX_SIZE_BITS = sizeof(id_t) == 4 ? 24 : TOTAL_SIZE_BITS / 2;
    static constexpr auto VERSION_SIZE_BITS = sizeof(id_t) * 8 - INDEX_SIZE_BITS;
    static_assert( INDEX_SIZE_BITS >= 24 );
    static_assert( INDEX_SIZE_BITS <= 32 );
    static_assert( VERSION_SIZE_BITS > 0 );
    static constexpr id_t INDEX_MASK = (id_t(1) << INDEX_SIZE_BITS) - 1;
    static constexpr id_t VERSION_MASK = (id_t(1) << VERSION_SIZE_BITS) - 1;
    static constexpr auto VERSION_SHIFT = INDEX_SIZE_BITS;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    ResourceID();

    /**
     * Explicit constructor.
     */
    ResourceID(size_t index, unsigned version);

    /**
     * Destructor.
     */
    ~ResourceID() = default;

    /**
     * Copy constructor.
     */
    ResourceID(const ResourceID& rhs) = default;

    /**
     * Copy assignment operator.
     */
    ResourceID& operator = (const ResourceID& rhs) = default;

    /**
     * Swap operation.
     */
    void swap(ResourceID& lhs);



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The ID composed of index and version.
     */
    id_t _id;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns true if the resource ID is valid, false otherwise.
     */
    bool is_valid() const;

    /**
     * Returns the technical unsigned-integer representation of the resource ID.
     */
    id_t value() const { return _id; }

    /**
     * Returns the index part of the resource ID.
     */
    size_t index() const;

    /**
     * Extracts the version from the resource ID.
     */
    unsigned version() const;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Invalidates this resource ID.
     */
    void invalidate();



    // #############################################################################
    // Comparison Operators
    // #############################################################################
public:
    auto operator == (ResourceID rhs) const { return _id == rhs._id; }
    auto operator != (ResourceID rhs) const { return _id != rhs._id; }



    // #############################################################################
    // Conversion Operators
    // #############################################################################
public:
    operator bool () const { return is_valid(); }

}; // class ResourceID



} // namespace resources
} // namespace xl7

#endif // XL7_RESOURCES_RESOURCEID_H
