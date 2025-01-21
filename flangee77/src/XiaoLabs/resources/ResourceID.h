#ifndef XL7_RESOURCES_RESOURCEID_H
#define XL7_RESOURCES_RESOURCEID_H

#include <CoreLabs/root.h>



namespace xl7::resources {



#define XL7_DECLARE_RESOURCE_ID()           class ID : public xl7::resources::ResourceID    { using TBaseID = xl7::resources::ResourceID; using TBaseID::TBaseID; }; ID get_id() const { return xl7::resources::Resource::get_id<ID>(); }
#define XL7_DERIVE_RESOURCE_ID(TBase)       class ID : public TBase::ID                     { using TBaseID = TBase::ID;                  using TBaseID::TBaseID; }; ID get_id() const { return xl7::resources::Resource::get_id<ID>(); }



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
    // Conversion Operators
    // #############################################################################

    operator bool() const noexcept { return is_valid(); }



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



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    template <class T1, class T2>
        requires(std::is_base_of_v<ResourceID, T1> && std::is_base_of_v<ResourceID, T2>)
    auto operator==(T1 a, T2 b) noexcept
    {
        if (!std::is_base_of_v<T1, T2> && !std::is_base_of_v<T2, T1>)
            return false;
        return a.value() == b.value();
    }

    template <class T1, class T2>
        requires(std::is_base_of_v<ResourceID, T1> && std::is_base_of_v<ResourceID, T2>)
    auto operator!=(T1 a, T2 b) noexcept
    {
        return !(a == b);
    }



    // #############################################################################
    // Conversions
    // #############################################################################

    template <class T1, class T2>
        requires(std::is_base_of_v<ResourceID, T1> && std::is_base_of_v<ResourceID, T2>)
    T1 id_cast(T2 other)
    {
        static_assert(std::is_base_of_v<T1, T2> || std::is_base_of_v<T2, T1>, "invalid id_cast");
        return T1(other.index(), other.version());
    }



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
