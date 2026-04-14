#ifndef XL7_RESOURCES_RESOURCEWITHDATA_H
#define XL7_RESOURCES_RESOURCEWITHDATA_H
#include "./ResourceBase.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>
#include <CoreLabs/byte_span.h>



namespace xl7::resources {



template <class TDerived, class TBase = Resource, bool resizable = false>
    requires(std::derived_from<TBase, Resource>)
class ResourceWithData
    : public ResourceBase<TDerived, TBase>
{

public:
    ResourceWithData() = delete;

    ResourceWithData(const ResourceWithData&) = delete;
    ResourceWithData& operator=(const ResourceWithData&) = delete;
    ResourceWithData(ResourceWithData&&) = delete;
    ResourceWithData& operator=(ResourceWithData&&) = delete;



    /** Returns the local copy of the resource data. */
    cl7::byte_view get_data() const { return _data; }

    /** Returns the size of the resource data, in bytes. */
    size_t get_data_size() const { return _data.size(); }



protected:
    using data_accessor = std::conditional_t<resizable, cl7::byte_vector&, cl7::byte_span>;



    template <typename ...Args>
    ResourceWithData(const Resource::CreateContext& ctx, size_t data_size, Args&&... args)
        : ResourceBase<TDerived, TBase>(ctx, std::forward<Args>(args)...)
        , _data(data_size)
    {
        assert(_data.size() <= 0xffffffff);
    }

    template <typename ...Args>
    ResourceWithData(const Resource::CreateContext& ctx, cl7::byte_vector data, Args&&... args)
        : ResourceBase<TDerived, TBase>(ctx, std::forward<Args>(args)...)
        , _data(std::move(data))
    {
        assert(_data.size() <= 0xffffffff);
    }

    ~ResourceWithData() override = default;



    /** Provides mutable access to the local copy of the resource data. */
    data_accessor _access_data() { return _data; }



private:
    /** The local copy of the resource data. */
    cl7::byte_vector _data;

}; // class ResourceWithData



} // namespace xl7::resources

#endif // XL7_RESOURCES_RESOURCEWITHDATA_H
