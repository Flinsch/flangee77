#ifndef XL7_RESOURCES_RESOURCEID_H
#define XL7_RESOURCES_RESOURCEID_H

#include <CoreLabs/VersionedId.h>



namespace xl7::resources {



class ResourceId
    : public cl7::VersionedId<size_t, 32, ResourceId>
{

public:
    using VersionedId::VersionedId;



private:
    static_assert(sizeof(value_type) == 4 || sizeof(value_type) == 8);
    static_assert(INDEX_SIZE_BITS == (sizeof(value_type) == 4 ? 24 : TOTAL_SIZE_BITS / 2));
    static_assert(INDEX_SIZE_BITS >= 24);
    static_assert(INDEX_SIZE_BITS <= 32);
    static_assert(VERSION_SIZE_BITS > 0);

}; // class ResourceId



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    template <class T1, class T2>
        requires(std::derived_from<T1, ResourceId> && std::derived_from<T2, ResourceId>)
    auto operator==(T1 a, T2 b) noexcept
    {
        if (!std::derived_from<T1, T2> && !std::derived_from<T2, T1>)
            return false;
        return a.value() == b.value();
    }

    template <class T1, class T2>
        requires(std::derived_from<T1, ResourceId> && std::derived_from<T2, ResourceId>)
    auto operator!=(T1 a, T2 b) noexcept
    {
        return !(a == b);
    }



    // #############################################################################
    // Conversions
    // #############################################################################

    template <class T1, class T2>
        requires(std::derived_from<T1, ResourceId> && std::derived_from<T2, ResourceId>)
    T1 id_cast(T2 other)
    {
        static_assert(std::derived_from<T1, T2> || std::derived_from<T2, T1>, "invalid id_cast");
        return T1(other.index(), other.version());
    }



} // namespace xl7::resources



template <>
struct std::hash<xl7::resources::ResourceId>
{
    size_t operator()(const xl7::resources::ResourceId& resource_id) const noexcept
    {
        return resource_id.hash();
    }
};



#endif // XL7_RESOURCES_RESOURCEID_H
