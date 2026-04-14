#ifndef XL7_RESOURCES_RESOURCE_H
#define XL7_RESOURCES_RESOURCE_H

#include "./ResourceId.h"

#include <CoreLabs/string.h>



namespace xl7::resources {



class ResourceManager;



class Resource
{

public:
    using Id = ResourceId;

    class Attorney
    {
        static bool acquire(Resource* resource) { return resource->_acquire(); }
        static void dispose(Resource* resource) { resource->_dispose(); }
        static void release(Resource* resource) { resource->_release(); }
        static void destroy(Resource* resource) { delete resource; }
        friend class ResourceManager;
    };

    struct CreateContext
    {
        /** The owning manager of the resource to create (the manager that creates the resource). */
        ResourceManager* manager;
        /** The (new) ID of the resource to create. */
        ResourceId id;
        /** The textual identifier of the resource to create (can be empty). */
        cl7::u8string_view identifier;
    };



    Resource() = delete;

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
    Resource(Resource&&) = delete;
    Resource& operator=(Resource&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::u8string_view get_type_string() const { return u8"resource"; }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Returns a "human-friendly" string that combines the resource type with the
     * identifier of this resource. If the resource has no identifier, only the type
     * is returned.
     */
    cl7::u8string get_qualified_identifier() const;

    /**
     * Increases the reference count. A call to this function should be paired with
     * a call to the release function (or one of the manager's release functions).
     */
    void add_reference();

    /**
     * Releases the resource. If the reference count reaches zero, the resource is
     * actually disposed/"unacquired" (and removed from its owning manager), thereby
     * rendering it unusable.
     * Time complexity: linear in the number of contained resources of the owning
     * manager.
     */
    void release();



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    template <class T>
    T* get_raw_resource() const
    {
        void* raw_resource = _get_raw_resource_impl();
        //assert(static_cast<T*>(raw_resource) == dynamic_cast<T*>(raw_resource)); // Unfortunately not possible with void*.
        return static_cast<T*>(raw_resource);
    }

    /**
     * Returns the resource manager responsible for this resource.
     */
    ResourceManager* get_manager() const { return _manager; }

    /**
     * Returns the ID of this resource.
     */
    ResourceId get_id() const { return _id; }

    /**
     * Returns the ID of this resource.
     */
    template <class TResourceId>
    TResourceId get_id() const { return id_cast<TResourceId>(_id); }

    /**
     * Returns the identifier of this resource (if specified, empty otherwise).
     */
    const cl7::u8string& get_identifier() const { return _identifier; }

    /**
     * Returns the reference count to determine whether the resource should actually
     * be disposed/"unacquired" when released.
     */
    unsigned get_reference_count() const { return _reference_count; }

    /**
     * Indicates whether this resource is ready for use (i.e., it is managed by its
     * owning manager and has been successfully acquired).
     */
    bool is_usable() const { return _is_usable; }



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    explicit Resource(const CreateContext& ctx);

    virtual ~Resource();



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Checks whether this resource is ready for use (i.e., it is managed by its
     * owning manager and has been successfully acquired) and fires an error message
     * if not.
     */
    bool _check_is_usable() const;

    /**
     * Checks whether the specified flag indicates that this supposedly updatable
     * resource is actually mutable and fires an error message if not.
     */
    bool _check_is_mutable(bool is_mutable) const;



private:

    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    virtual void* _get_raw_resource_impl() const { return nullptr; }

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    virtual bool _acquire_impl() = 0;

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state before this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    virtual bool _dispose_impl() = 0;



    // #############################################################################
    // Lifetime Management
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state (or not).
     */
    bool _acquire();

    /**
     * Disposes/"unacquires" the resource, thereby rendering it unusable, indicating
     * that it is no longer managed by its owning manager.
     */
    void _dispose();

    /**
     * Decreases the reference count. If it reaches zero, the resource is actually
     * disposed/"unacquired", thereby rendering it unusable, indicating that it is
     * no longer managed by its owning manager.
     */
    void _release();



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The resource manager that owns this resource.
     */
    ResourceManager* const _manager;

    /**
     * The ID of this resource.
     */
    const ResourceId _id;

    /**
     * The textual identifier of this resource (if specified, empty otherwise).
     */
    const cl7::u8string _identifier;

    /**
     * The reference count to determine whether the resource should actually be
     * disposed/"unacquired" when released.
     */
    unsigned _reference_count;

    /**
     * The flag that indicates whether this resource is ready for use (i.e., it is
     * managed by its owning manager and has been successfully acquired).
     */
    bool _is_usable;

}; // class Resource



} // namespace xl7::resources

#endif // XL7_RESOURCES_RESOURCE_H
