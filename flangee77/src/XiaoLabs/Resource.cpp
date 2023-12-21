#include "Resource.h"

#include <CoreLabs/logging.h>



namespace xl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Resource::Resource(ResourceManager* manager, const cl7::string& identifier)
        : _manager( manager )
        , _identifier( identifier )
        , _managed( true )
        , _acquired( false )
        , _resigned( false )
    {
        assert( manager );
    }

    /**
     * Destructor.
     */
    Resource::~Resource()
    {
        assert( !_managed );
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
        if ( !_check_managed() )
            return false;

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
        if ( !_check_managed() )
            return false;

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
        if ( !_check_managed() )
            return false;

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
        if ( !_check_managed() )
            return false;

        assert( _acquired );
        assert( _resigned );

        if ( !_restore_impl() )
            return false;

        _resigned = false;
        return true;
    }



    // #############################################################################
    // Management Functions
    // #############################################################################

    /**
     * Checks whether the resource is still managed by its owning manager and fires
     * an error message if not.
     */
    bool Resource::_check_managed() const
    {
        if ( _managed )
            return true;

        LOG_ERROR( TEXT("The resource \"") + _identifier + TEXT("\" is not managed anymore.") );
        return false;
    }

    /**
     * Informs the resource that it will no longer be managed by its owning manager.
     */
    void Resource::_unmanage()
    {
        if ( !_check_managed() )
            return;

        if ( _acquired )
            release();
    }



} // namespace xl7
