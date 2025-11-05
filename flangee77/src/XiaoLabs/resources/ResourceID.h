#ifndef XL7_RESOURCES_RESOURCEID_H
#define XL7_RESOURCES_RESOURCEID_H

#include <CoreLabs/VersionedId.h>



namespace xl7::resources {



class ResourceID;

#define XL7_DECLARE_RESOURCE_ID()           class ID : public xl7::resources::ResourceID    { using TBaseID = xl7::resources::ResourceID; using TBaseID::TBaseID; }; ID get_id() const { return xl7::resources::Resource::get_id<ID>(); }
#define XL7_DERIVE_RESOURCE_ID(TBase)       class ID : public TBase::ID                     { using TBaseID = TBase::ID;                  using TBaseID::TBaseID; }; ID get_id() const { return xl7::resources::Resource::get_id<ID>(); }



class ResourceID
    : public cl7::VersionedId<size_t, 32, ResourceID>
{

public:
    using VersionedId::VersionedId;



private:
    static_assert(sizeof(value_type) == 4 || sizeof(value_type) == 8);
    static_assert(INDEX_SIZE_BITS == (sizeof(value_type) == 4 ? 24 : TOTAL_SIZE_BITS / 2));
    static_assert(INDEX_SIZE_BITS >= 24);
    static_assert(INDEX_SIZE_BITS <= 32);
    static_assert(VERSION_SIZE_BITS > 0);

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
