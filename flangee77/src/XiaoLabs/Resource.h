#pragma once
#ifndef XL7_RESOURCE_H
#define XL7_RESOURCE_H

#include <CoreLabs/byte_span.h>
#include <CoreLabs/byte_vector.h>
#include <CoreLabs/string.h>



namespace xl7 {



class Resource;
typedef std::shared_ptr<Resource> ResourcePtr;



class ResourceManager;



class Resource
{

public:
    class Attorney
    {
        static void unmanage(Resource* resource) { resource->_unmanage(); }
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

    struct DataSource
    {
        virtual size_t get_data_size() const { return 0; };
        virtual bool fill_data(cl7::byte_vector& data) const { return true; }
    };

    struct DefaultSource
        : public DataSource
    {
        DefaultSource(const cl7::byte_span& data) : _data( data ) {}
        virtual size_t get_data_size() const override { return _data.size(); }
        virtual bool fill_data(cl7::byte_vector& data) const override;
        const cl7::byte_span _data;
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
    Resource(ResourceManager* manager, const cl7::string_view& identifier, const DataSource& data_source);

    /**
     * Explicit constructor.
     */
    template <class TDesc>
    Resource(const CreateParams<TDesc>& params)
        : Resource( params.manager, params.identifier )
    {
    }

    /**
     * Explicit constructor.
     */
    template <class TDesc>
    Resource(const CreateParams<TDesc>& params, const DataSource& data_source)
        : Resource( params.manager, params.identifier, data_source )
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
     * The flag specifying whether this resource is (still) managed by its owning
     * manager.
     */
    bool _managed;

    /**
     * The flag specifying whether this resource has been acquired.
     */
    bool _acquired;

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
     * Returns the flag specifying whether this resource is (still) managed by its
     * owning manager.
     */
    bool is_managed() const { return _managed; }

    /**
     * Returns the flag specifying whether this resource has been acquired.
     */
    bool is_acquired() const { return _acquired; }

    /**
     * Indicates whether the resource is ready for use, i.e. it continues to be
     * managed by its owning manager and has been successfully acquired.
     */
    bool is_operational() const { return _acquired && _managed; }

    /**
     * Returns the (optional) local copy of the resource data.
     */
    const cl7::byte_vector& get_data() const { return _data; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool acquire();

    /**
     * Releases/"unacquires" the resource.
     */
    bool release();



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    virtual bool _acquire_impl() = 0;

    /**
     * Releases/"unacquires" the resource.
     */
    virtual bool _release_impl() = 0;



    // #############################################################################
    // Management Functions
    // #############################################################################
protected:
    /**
    * Checks whether the resource is still managed by its owning manager and fires
    * an error message if not.
    */
    bool _check_managed() const;

private:
    /**
     * Informs the resource that it will no longer be managed by its owning manager.
     */
    void _unmanage();

}; // class Resource



} // namespace xl7

#endif // XL7_RESOURCE_H
