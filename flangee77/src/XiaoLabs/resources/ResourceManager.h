#pragma once
#ifndef XL7_RESOURCES_RESOURCEMANAGER_H
#define XL7_RESOURCES_RESOURCEMANAGER_H

#include "./Resource.h"

#include <CoreLabs/string.h>

#include <vector>
#include <unordered_map>
#include <functional>



namespace xl7 {
namespace resources {



class ResourceManager
{

protected:
    static void _destroy_resource(Resource* resource);

protected:
    typedef std::unique_ptr<Resource, decltype(&_destroy_resource)> ResourcePtr;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    ResourceManager();

    /**
     * Destructor.
     */
    virtual ~ResourceManager();

private:
    /** Copy constructor. */
    ResourceManager(const ResourceManager&) = delete;
    /** Copy assignment operator. */
    ResourceManager& operator = (const ResourceManager&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * A "linear list" of the managed resources.
     */
    struct ResourceEntry
    {
        ResourceID id;
        ResourcePtr ptr;
    };
    std::vector<ResourceEntry> _resources;

    /**
     * A lookup table that maps a given resource identifier to the corresponding
     * resource ID.
     */
    std::unordered_map<cl7::astring_view, ResourceID> _resource_lookup;

    /**
     * A "list" of indices that mark free entries in the "linear list".
     */
    std::vector<size_t> _free_indices;



    // #############################################################################
    // Methods
    // #############################################################################
public:
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
    bool contains_resource(cl7::astring_view identifier) const;

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
    Resource* find_resource(cl7::astring_view identifier) const;

    /**
     * Returns the resource identified by the given index.
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    template <class TResource>
    TResource* get_resource(size_t index) const
    {
        Resource* resource = get_resource( index );
        assert( static_cast<TResource*>( resource ) == dynamic_cast<TResource*>( resource ) );
        return static_cast<TResource*>( resource );
    }

    /**
     * Returns the resource of the given ID.
     * Time complexity: constant.
     */
    template <class TResource>
    TResource* find_resource(ResourceID id) const
    {
        Resource* resource = find_resource( id );
        assert( static_cast<TResource*>( resource ) == dynamic_cast<TResource*>( resource ) );
        return static_cast<TResource*>( resource );
    }

    /**
     * Returns the resource of the given identifier.
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    template <class TResource>
    TResource* find_resource(cl7::astring_view identifier) const
    {
        Resource* resource = find_resource( identifier );
        assert( static_cast<TResource*>( resource ) == dynamic_cast<TResource*>( resource ) );
        return static_cast<TResource*>( resource );
    }

    /**
     * Releases the specified resource (and removes it from this resource manager).
     * Time complexity: constant.
     */
    bool release_resource(Resource* resource);

    /**
     * Releases the specified resource (and removes it from this resource manager).
     * Time complexity: constant.
     */
    bool release_resource(ResourceID id);

    /**
     * Releases the specified resource (and removes it from this resource manager)
     * and invalidates the given ID.
     * Time complexity: constant.
     */
    bool release_resource_and_invalidate(ResourceID& id);

    /**
     * Releases the specified resource (and removes it from this resource manager).
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    bool release_resource(cl7::astring_view identifier);

    /**
     * Releases all managed resources (and removes them from this resource manager).
     */
    void release_resources();



    // #############################################################################
    // Management Functions
    // #############################################################################
protected:
    /**
     * Generates and returns the next resource ID to use when adding a new resource.
     */
    ResourceID _next_id();

    /**
     * Adds the given resource to this resource manager (and returns the ID of the
     * resource), but only if it can be acquired in this process. Returns an invalid
     * ID otherwise.
     */
    ResourceID _try_acquire_and_add_resource(ResourcePtr resource_ptr, const DataProvider& data_provider);

private:
    /**
     * Adds the given resource to this resource manager. This operation does not
     * request/acquire the resource. This must have happened successfully before.
     * Returns the ID of the resource.
     */
    ResourceID _add_resource(ResourcePtr resource_ptr);

}; // class ResourceManager



} // namespace resources
} // namespace xl7

#endif // XL7_RESOURCES_RESOURCEMANAGER_H
