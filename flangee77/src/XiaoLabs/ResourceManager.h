#pragma once
#ifndef XL7_RESOURCEMANAGER_H
#define XL7_RESOURCEMANAGER_H

#include "./Resource.h"

#include <CoreLabs/string.h>

#include <vector>
#include <unordered_map>
#include <functional>



namespace xl7 {



class ResourceManager
{

protected:
    typedef std::unique_ptr<Resource, std::function<void(Resource*)>> ResourcePtr;



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
     * A lookup table that maps a given resource (by its pointer hash) to its
     * corresponding index within the "linear list".
     */
    std::unordered_map<const Resource*, size_t> _resource_lookup;



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
     * Time complexity: linear in the number of contained resources.
     */
    bool contains_resource(const cl7::string_view& identifier) const;

    /**
     * Returns the resource identified by the given index.
     */
    Resource* get_resource(size_t index) const;

    /**
     * Returns the resource of the given identifier.
     * Time complexity: linear in the number of contained resources.
     */
    Resource* find_resource(const cl7::string_view& identifier) const;

    /**
     * Releases the given resource
     * (deletes it and removes it from the internal list).
     */
    void release_resource(Resource* resource);

    /**
     * Releases the contained resources
     * (deletes them and removes them from the internal list).
     */
    void release_resources();

    /**
     * Temporarily releases/resigns the contained resources.
     */
    void resign_resources();

    /**
     * Reaquires/restores the contained resources.
     */
    void restore_resources();



    // #############################################################################
    // Protected Methods
    // #############################################################################
protected:
    /**
     * Adds the given resource to this resource manager.
     * This operation does not request/acquire the resource.
     */
    void _add_resource(ResourcePtr resource_ptr);

    /**
     * Adds the given resource to this resource manager.
     * This operation does not request/acquire the resource.
     */
    void _add_resource(Resource* resource);

    /**
     * Removes the given resource from this resource manager.
     * This operation does not release the resource.
     */
    void _remove_resource(Resource* resource);

}; // class ResourceManager



} // namespace xl7

#endif // XL7_RESOURCEMANAGER_H
