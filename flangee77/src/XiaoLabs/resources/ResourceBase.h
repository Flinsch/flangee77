#ifndef XL7_RESOURCES_RESOURCEBASE_H
#define XL7_RESOURCES_RESOURCEBASE_H
#include "./Resource.h"



namespace xl7::resources {



template <class TDerived, class TBase = Resource>
    requires(std::derived_from<TBase, Resource>)
class ResourceBase
    : public TBase
{

public:
    class Id :
        public TBase::Id
    {
        using TBase::Id::Id;
    };



    ResourceBase() = delete;

    ResourceBase(const ResourceBase&) = delete;
    ResourceBase& operator=(const ResourceBase&) = delete;
    ResourceBase(ResourceBase&&) = delete;
    ResourceBase& operator=(ResourceBase&&) = delete;



    /** Returns the ID of this resource. */
    Id get_id() const { return Resource::get_id<Id>(); }



protected:
    template <typename ...Args>
    ResourceBase(const Resource::CreateContext& ctx, Args&&... args)
        : TBase(ctx, std::forward<Args>(args)...)
    {
    }

    ~ResourceBase() override = default;

}; // class ResourceBase



} // namespace xl7::resources

#endif // XL7_RESOURCES_RESOURCEBASE_H
