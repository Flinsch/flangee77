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
    std::vector<ResourcePtr> _resources;

    /**
     * A lookup table that maps a given resource identifier to the corresponding
     * resource.
     */
    std::unordered_map<cl7::string_view, ResourcePtr> _resource_lookup;



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
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    bool contains_resource(const Resource* resource) const;

    /**
     * Checks whether a resource with the given identifier is contained in this
     * resource manager.
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    bool contains_resource(const cl7::string_view& identifier) const;

    /**
     * Returns the resource identified by the given index.
     */
    ResourcePtr get_resource(size_t index) const;

    /**
     * Returns the resource of the given identifier.
     * Time complexity: linear in the number of contained resources.
     */
    ResourcePtr find_resource(const cl7::string_view& identifier) const;

    /**
     * Releases the given resource (and removes it from this resource manager).
     * Time complexity: linear in the number of contained resources.
     */
    void release_resource(Resource* resource);

    /**
     * Releases the specified resource (and removes it from this resource manager).
     * Also resets the given resource pointer object.
     * Time complexity: linear in the number of contained resources.
     */
    template <class TResourcePtr>
    void release_resource(TResourcePtr& resource_ptr)
    {
        release_resource( resource_ptr.get() );
        resource_ptr.reset();
    }

    /**
     * Releases all managed resources (and removes them from this resource manager).
     */
    void release_resources();



    // #############################################################################
    // Management Functions
    // #############################################################################
protected:
    /**
     * Adds (and returns) the given resource to this resource manager, but only if
     * it can be acquired in this process. Returns NULL otherwise.
     */
    template <class TResourcePtr>
    TResourcePtr _try_acquire_and_add_resource(TResourcePtr resource_ptr, const DataProvider& data_provider)
    {
        assert( resource_ptr );
        if ( !resource_ptr )
            return {};

        assert( !resource_ptr->is_usable() );
        if ( !Resource::Attorney::acquire( resource_ptr.get(), data_provider ) )
            return {};

        _add_resource( resource_ptr );
        return resource_ptr;
    }

private:
    /**
     * Adds the given resource to this resource manager. This operation does not
     * request/acquire the resource. This must have happened successfully before.
     */
    void _add_resource(ResourcePtr resource_ptr);

}; // class ResourceManager



} // namespace resources
} // namespace xl7

#endif // XL7_RESOURCES_RESOURCEMANAGER_H
