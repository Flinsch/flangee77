#pragma once
#ifndef XL7_RESOURCE_H
#define XL7_RESOURCE_H

#include "./ResourceUsage.h"

#include <CoreLabs/byte_view.h>
#include <CoreLabs/byte_vector.h>
#include <CoreLabs/string.h>



namespace xl7 {



class Resource;
typedef std::shared_ptr<Resource> ResourcePtr;



class ResourceManager;



class Resource
{

public:
    struct DataProvider
    {
        virtual size_t get_offset() const { return 0; }
        virtual size_t get_size() const { return 0; }
        virtual bool fill(cl7::byte_vector& data) const { return true; }
    };

    struct DefaultDataProvider
        : public DataProvider
    {
        const cl7::byte_view data;
        const size_t offset;

        DefaultDataProvider(const cl7::byte_view& data, size_t offset = 0) : data( data ), offset( offset ) {}

        virtual size_t get_size() const override { return data.size(); }
        virtual bool fill(cl7::byte_vector& data) const override;
    };

public:
    class Attorney
    {
        static bool acquire(Resource* resource, const Resource::DataProvider& data_provider) { return resource->_acquire( data_provider ); }
        static void release(Resource* resource) { resource->_release(); }
        static void destroy(Resource* resource) { delete resource; }
        friend class ResourceManager;
    };

public:
    template <class TDesc>
    struct CreateParams
    {
        /** The owning manager of the resource to create (the manager that creates the resource). */
        ResourceManager* manager;
        /** The identifier of the resource to create (may be empty). */
        cl7::string_view identifier;
        /** The descriptor of the resource to create. */
        TDesc desc;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    Resource(ResourceManager* manager, const cl7::string_view& identifier);

    /**
     * Explicit constructor.
     */
    template <class TDesc>
    Resource(const CreateParams<TDesc>& params)
        : Resource( params.manager, params.identifier )
    {
    }

    /**
     * Destructor.
     */
    virtual ~Resource();

private:
    /** Default constructor. */
    Resource() = delete;
    /** Copy constructor. */
    Resource(const Resource&) = delete;
    /** Copy assignment operator. */
    Resource& operator = (const Resource&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The resource manager that owns this resource.
     */
    ResourceManager* const _manager;

    /**
     * The identifier of this resource (if specified, empty otherwise).
     */
    const cl7::string _identifier;

private:
    /**
     * The flag that indicates whether this resource is ready for use (i.e., it is
     * managed by its owning manager and has been successfully acquired).
     */
    bool _is_usable;

protected:
    /**
     * The (optional) local copy of the resource data.
     */
    cl7::byte_vector _data;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the resource manager responsible for this resource.
     */
    ResourceManager* get_manager() const { return _manager; }

    /**
     * Returns the identifier of this resource (if specified, empty otherwise).
     */
    const cl7::string& get_identifier() const { return _identifier; }

    /**
     * Indicates whether this resource is ready for use (i.e., it is managed by its
     * owning manager and has been successfully acquired).
     */
    bool is_usable() const { return _is_usable; }

    /**
     * Returns the (optional) local copy of the resource data.
     */
    const cl7::byte_vector& get_data() const { return _data; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::string get_typed_identifier_string() const { return cl7::string(get_type_string()) + TEXT(" \"") + get_identifier() + TEXT("\""); }



    // #############################################################################
    // Lifetime Management
    // #############################################################################
private:
    /**
     * Requests/acquires the resource, bringing it into a usable state (or not).
     */
    bool _acquire(const DataProvider& data_provider);

    /**
     * Releases/"unacquires" the resource, thereby rendering it unusable, indicating
     * that it is no longer managed by its owning manager.
     */
    void _release();



    // #############################################################################
    // Helpers
    // #############################################################################
protected:
    /**
     * Checks whether this resource is ready for use (i.e., it is managed by its
     * owning manager and has been successfully acquired) and fires an error message
     * if not.
     */
    bool _check_is_usable() const;

protected:
    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource and, if so, (re)populates the local data buffer.
     */
    bool _try_fill_data(const DataProvider& data_provider);

protected:
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



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    virtual bool _check_impl(const DataProvider& data_provider) = 0;

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const DataProvider& data_provider) = 0;

    /**
     * Releases/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    virtual bool _release_impl() = 0;

public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const { return TEXT("resource"); }

}; // class Resource



} // namespace xl7

#endif // XL7_RESOURCE_H
