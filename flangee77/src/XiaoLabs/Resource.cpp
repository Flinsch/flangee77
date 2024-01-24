#include "Resource.h"

#include <CoreLabs/logging.h>



namespace xl7 {



    bool Resource::DefaultDataProvider::fill_data(cl7::byte_vector& data) const
    {
        data.resize( this->data.size() );
        assert( data.size() == this->data.size() );
        std::copy( this->data.begin(), this->data.end(), data.begin() );
        return true;
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Resource::Resource(ResourceManager* manager, const cl7::string_view& identifier)
        : _manager( manager )
        , _identifier( identifier )
        , _managed( true )
        , _acquired( false )
        , _data()
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
    bool Resource::acquire(const DataProvider& data_provider)
    {
        if ( !_check_managed() )
            return false;

        if ( _acquired )
        {
            LOG_WARNING( TEXT("The resource \"") + _identifier + TEXT("\" has already been acquired.") );
            return false;
        }

        if ( data_provider.get_data_size() == 0 )
        {
            _data.clear();
        }
        else if ( !data_provider.fill_data( _data ) )
        {
            LOG_ERROR( TEXT("The given data provider was not able to fill the local data buffer of the resource \"") + _identifier + TEXT("\".") );
            return false;
        }

        if ( !_acquire_impl( data_provider ) )
        {
            LOG_ERROR( TEXT("The resource \"") + _identifier + TEXT("\" could not be acquired.") );
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
        if ( !b )
        {
            LOG_WARNING( TEXT("The resource \"") + _identifier + TEXT("\" could not be released without problems.") );
        }

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
