#pragma once
#ifndef XL7_RESOURCE_H
#define XL7_RESOURCE_H

#include <CoreLabs/string.h>



namespace xl7 {



class Resource;
typedef std::weak_ptr<Resource> ResourcePtr;



class ResourceManager;



class Resource
{

public:
    class Attorney
    {
        static void destroy(Resource* resource) { delete resource; }
        friend class ResourceManager;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    explicit Resource(ResourceManager* manager);

    /**
     * Explicit constructor.
     */
    Resource(ResourceManager* manager, const cl7::string& identifier);

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
     * The flag specifying whether this resource has been acquired.
     */
    bool _acquired;

    /**
     * The flag specifying whether this resource has been (temporarily) resigned.
     */
    bool _resigned;



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
     * Returns the flag specifying whether this resource has been acquired.
     */
    bool is_acquired() const { return _acquired; }

    /**
     * Returns the flag specifying whether this resource has been (temporarily)
     * resigned.
     */
    bool is_resigned() const { return _resigned; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool request();

    /**
     * Releases the resource.
     */
    bool release();

    /**
     * Temporarily resigns some stuff to free up some (hardware) memory etc.
     * This can be used to free up capacity for other resources that are urgently
     * needed right now, but it is known that this resource will be reused later, so
     * it should not be fully released yet.
     */
    bool resign();

    /**
     * Restores/reacquires the resource after it has been (temporarily) resigned,
     * returning it to a usable state.
     */
    bool restore();



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    virtual bool _request_impl() = 0;

    /**
     * Releases the resource.
     */
    virtual bool _release_impl() = 0;

    /**
     * Temporarily resigns some stuff to free up some (hardware) memory etc.
     */
    virtual bool _resign_impl() = 0;

    /**
     * Restores the resource after it has been (temporarily) resigned, returning it
     * to a usable state.
     */
    virtual bool _restore_impl() = 0;

}; // class Resource



} // namespace xl7

#endif // XL7_RESOURCE_H
