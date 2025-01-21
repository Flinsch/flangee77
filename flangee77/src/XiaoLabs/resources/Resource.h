#ifndef XL7_RESOURCES_RESOURCE_H
#define XL7_RESOURCES_RESOURCE_H

#include "./ResourceID.h"
#include "./ResourceUsage.h"
#include "./DefaultDataProvider.h"

#include <CoreLabs/byte_view.h>
#include <CoreLabs/byte_vector.h>
#include <CoreLabs/string.h>



namespace xl7::resources {



class ResourceManager;



class Resource
{

public:
    class Attorney
    {
        static bool acquire(Resource* resource, const DataProvider& data_provider) { return resource->_acquire(data_provider); }
        static void dispose(Resource* resource) { resource->_dispose(); }
        static void release(Resource* resource) { resource->_release(); }
        static void destroy(Resource* resource) { delete resource; }
        friend class ResourceManager;
    };

    template <class TDesc>
    struct CreateParams
    {
        /** The owning manager of the resource to create (the manager that creates the resource). */
        ResourceManager* manager;
        /** The (new) ID of the resource to create. */
        ResourceID id;
        /** The textual identifier of the resource to create (can be empty). */
        cl7::u8string_view identifier;
        /** The descriptor of the resource to create. */
        TDesc desc;
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
     * Returns the identifier of this resource (if specified, empty otherwise).
     */
    cl7::u8string get_identifier_string() const;

    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string get_typed_identifier_string() const { return cl7::u8string(get_type_string()) + u8" \"" + get_identifier_string() + u8"\""; }

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
    ResourceID get_id() const { return _id; }

    /**
     * Returns the ID of this resource.
     */
    template <class TResourceID>
    TResourceID get_id() const { return id_cast<TResourceID>(_id); }

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

    /**
     * Returns the (optional) local copy of the resource data.
     */
    cl7::byte_view get_data() const { return _data; }



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Resource(ResourceManager* manager, ResourceID id, cl7::u8string_view identifier);

    template <class TDesc>
    Resource(const CreateParams<TDesc>& params)
        : Resource(params.manager, params.id, params.identifier)
    {
    }

    virtual ~Resource();



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Provides mutable access to the (optional) local copy of the resource data.
     */
    cl7::byte_vector& _access_data() { return _data; }

    /**
     * Checks whether this resource is ready for use (i.e., it is managed by its
     * owning manager and has been successfully acquired) and fires an error message
     * if not.
     */
    bool _check_is_usable() const;

    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource and, if so, (re)populates the local data buffer.
     */
    bool _try_fill_data(const DataProvider& data_provider);

    /**
     * Checks whether the given data provider complies with the specified total data
     * size and fires an error message if not.
     */
    bool _check_against_size(const DataProvider& data_provider, size_t size) const;

    /**
     * Checks whether the given data provider complies with the specified data
     * element/chunk size and fires an error message if not.
     */
    bool _check_against_stride(const DataProvider& data_provider, size_t stride) const;



private:

    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * Returns the "raw" resource interface/accessor, if applicable, otherwise NULL.
     */
    virtual void* _get_raw_resource_impl() const { return nullptr; }

    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    virtual bool _check_data_impl(const DataProvider& data_provider) = 0;

    /**
     * (Re)populates the local data buffer based on the given data provider.
     */
    virtual bool _fill_data_impl(const DataProvider& data_provider);

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const DataProvider& data_provider) = 0;

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    virtual bool _dispose_impl() = 0;



    // #############################################################################
    // Lifetime Management
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state (or not).
     */
    bool _acquire(const DataProvider& data_provider);

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
    const ResourceID _id;

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

    /**
     * The (optional) local copy of the resource data.
     */
    cl7::byte_vector _data;

}; // class Resource



} // namespace xl7::resources

#endif // XL7_RESOURCES_RESOURCE_H
