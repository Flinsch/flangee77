#include "Resource.h"

#include <CoreLabs/logging.h>



namespace xl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Resource::Resource(ResourceManager* manager, const cl7::string_view& identifier)
        : Resource( manager, identifier, {} )
    {
    }

    /**
     * Explicit constructor.
     */
    Resource::Resource(ResourceManager* manager, const cl7::string_view& identifier, const cl7::byte_span& data)
        : _manager( manager )
        , _identifier( identifier )
        , _managed( true )
        , _acquired( false )
        , _data( data.begin(), data.end() )
    {
        assert( _manager );
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
    bool Resource::acquire()
    {
        if ( !_check_managed() )
            return false;

        if ( _acquired )
            return true;

        if ( !_acquire_impl() )
        {
            release();
            return false;
        }

        _acquired = true;
        return true;
    }

    /**
     * Releases/"unacquires" the resource.
     */
    bool Resource::release()
    {
        if ( !_check_managed() )
            return false;

        bool b = _release_impl();

        _acquired = false;
        return b;
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
