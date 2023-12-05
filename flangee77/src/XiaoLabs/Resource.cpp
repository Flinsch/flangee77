#include "Resource.h"



namespace xl7 {



    void Resource::Attorney::destroy(Resource* resource)
    {
        delete resource;
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Resource::Resource(ResourceManager* manager)
        : _manager( manager )
        , _identifier()
        , _acquired( false )
        , _resigned( false )
    {
    }

    /**
     * Explicit constructor.
     */
    Resource::Resource(ResourceManager* manager, const cl7::string& identifier)
        : _manager( manager )
        , _identifier( identifier )
        , _acquired( false )
        , _resigned( false )
    {
    }

    /**
     * Destructor.
     */
    Resource::~Resource()
    {
        assert( !_acquired );
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool Resource::request()
    {
        assert( !_resigned );
        if ( _acquired )
            return true;

        if ( !_request_impl() )
        {
            release();
            return false;
        }

        _acquired = true;
        return true;
    }

    /**
     * Releases the resource.
     */
    bool Resource::release()
    {
        bool b = _release_impl();

        _acquired = false;
        return b;
    }

    /**
     * Temporarily resigns some stuff to free up some (hardware) memory etc.
     * This can be used to free up capacity for other resources that are urgently
     * needed right now, but it is known that this resource will be reused later, so
     * it should not be fully released yet.
     */
    bool Resource::resign()
    {
        assert( _acquired );
        assert( !_resigned );

        if ( !_resign_impl() )
            return false;

        _resigned = true;
        return true;
    }

    /**
     * Restores/reacquires the resource after it has been (temporarily) resigned,
     * returning it to a usable state.
     */
    bool Resource::restore()
    {
        assert( _acquired );
        assert( _resigned );

        if ( !_restore_impl() )
            return false;

        _resigned = false;
        return true;
    }



} // namespace xl7
