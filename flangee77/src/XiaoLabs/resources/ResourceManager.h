#ifndef XL7_RESOURCES_RESOURCEMANAGER_H
#define XL7_RESOURCES_RESOURCEMANAGER_H

#include "./Resource.h"

#include <CoreLabs/string.h>

#include <vector>
#include <unordered_map>
#include <functional>



namespace xl7::resources {



class ResourceManager
{

public:
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;



    /**
     * Returns the number of contained resources in this resource manager.
     */
    size_t get_resource_count() const;

    /**
     * Checks whether a given resource is contained in this resource manager.
     * Time complexity: constant.
     */
    bool contains_resource(const Resource* resource) const;

    /**
     * Checks whether a resource with the given resource ID is contained in this
     * resource manager.
     * Time complexity: constant.
     */
    bool contains_resource(ResourceID id) const;

    /**
     * Checks whether a resource with the given identifier is contained in this
     * resource manager.
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    bool contains_resource(cl7::u8string_view identifier) const;

    /**
     * Returns the resource identified by the given index.
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    Resource* get_resource(size_t index) const;

    /**
     * Returns the resource of the given ID.
     * Time complexity: constant.
     */
    Resource* find_resource(ResourceID id) const;

    /**
     * Returns the resource of the given identifier.
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    Resource* find_resource(cl7::u8string_view identifier) const;

    /**
     * Returns the resource identified by the given index.
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    template <class TResource>
        requires(std::is_base_of_v<Resource, TResource>)
    TResource* get_resource(size_t index) const
    {
        Resource* resource = get_resource(index);
        assert(static_cast<TResource*>(resource) == dynamic_cast<TResource*>(resource));
        return static_cast<TResource*>(resource);
    }

    /**
     * Returns the resource of the given ID.
     * Time complexity: constant.
     */
    template <class TResource>
        requires(std::is_base_of_v<Resource, TResource>)
    TResource* find_resource(ResourceID id) const
    {
        Resource* resource = find_resource(id);
        assert(static_cast<TResource*>(resource) == dynamic_cast<TResource*>(resource));
        return static_cast<TResource*>(resource);
    }

    /**
     * Returns the resource of the given identifier.
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    template <class TResource>
        requires(std::is_base_of_v<Resource, TResource>)
    TResource* find_resource(cl7::u8string_view identifier) const
    {
        Resource* resource = find_resource(identifier);
        assert(static_cast<TResource*>(resource) == dynamic_cast<TResource*>(resource));
        return static_cast<TResource*>(resource);
    }

    /**
     * Releases the specified resource. If its reference count reaches zero, the
     * resource is actually disposed/"unacquired" (and removed from this resource
     * manager).
     * Time complexity: constant.
     */
    bool release_resource(Resource* resource);

    /**
     * Releases the specified resource. If its reference count reaches zero, the
     * resource is actually disposed/"unacquired" (and removed from this resource
     * manager).
     * Time complexity: constant.
     */
    bool release_resource(ResourceID id);

    /**
     * Releases the specified resource and invalidates the given ID. If the
     * reference count reaches zero, the resource is actually disposed/"unacquired"
     * (and removed from this resource manager).
     * Time complexity: constant.
     */
    bool release_resource_and_invalidate(ResourceID& id);

    /**
     * Releases the specified resource. If its reference count reaches zero, the
     * resource is actually disposed/"unacquired" (and removed from this resource
     * manager).
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    bool release_resource(cl7::u8string_view identifier);

    /**
     * Disposes/"unacquires" all managed resources (and removes them from this
     * resource manager).
     */
    void dispose_resources();



protected:
    using ResourcePtr = std::unique_ptr<Resource, std::add_pointer_t<void (Resource*)>>;



    ResourceManager() = default;

    virtual ~ResourceManager();



    /**
     * Generates and returns the next resource ID to use when adding a new resource.
     */
    ResourceID _next_id() const;

    /**
     * Adds the given resource to this resource manager (and returns the ID of the
     * resource), but only if it can be acquired in this process. Returns an invalid
     * ID otherwise.
     */
    ResourceID _try_acquire_and_add_resource(ResourcePtr resource_ptr, const DataProvider& data_provider);

    /**
     * Adds the given resource to this resource manager (and returns the ID of the
     * resource), but only if it can be acquired in this process. Returns an invalid
     * ID otherwise.
     */
    template <class TResourceID>
        requires(std::is_base_of_v<ResourceID, TResourceID>)
    TResourceID _try_acquire_and_add_resource(ResourcePtr resource_ptr, const DataProvider& data_provider)
    {
        return id_cast<TResourceID>(_try_acquire_and_add_resource(std::move(resource_ptr), data_provider));
    }



    /**
     * Destroys the resource via `Resource::Attorney::destroy`.
     */
    static void _destroy_resource(Resource* resource);



private:
    struct ResourceEntry
    {
        ResourceID id;
        ResourcePtr ptr;
    };



    /**
     * Adds the given resource to this resource manager. This operation does not
     * request/acquire the resource. This must have happened successfully before.
     * Returns the ID of the resource.
     */
    ResourceID _add_resource(ResourcePtr resource_ptr);



    /**
     * A "linear list" of the managed resources.
     */
    std::vector<ResourceEntry> _resources;

    /**
     * A lookup table that maps a given resource identifier to the corresponding
     * resource ID.
     */
    std::unordered_map<cl7::u8string_view, ResourceID> _resource_lookup;

    /**
     * A "list" of indices that mark free entries in the "linear list".
     */
    std::vector<size_t> _free_indices;

}; // class ResourceManager



} // namespace xl7::resources

#endif // XL7_RESOURCES_RESOURCEMANAGER_H
