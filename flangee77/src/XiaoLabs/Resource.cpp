#include "Resource.h"

#include <CoreLabs/logging.h>



namespace xl7 {



    bool Resource::DefaultDataProvider::fill(cl7::byte_vector& data) const
    {
        const size_t offset = get_offset();
        const size_t size = get_size();
        const size_t min_size = offset + size;
        if ( data.size() < min_size )
            data.resize( min_size );
        std::copy( this->data.begin(), this->data.end(), data.begin() + offset );
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
        , _is_usable( false )
        , _data()
    {
        assert( _manager );
    }

    /**
     * Destructor.
     */
    Resource::~Resource()
    {
        assert( !_is_usable );
    }



    // #############################################################################
    // Methods
    // #############################################################################



    // #############################################################################
    // Management Functions
    // #############################################################################

    /**
     * Checks whether this resource is ready for use (i.e., it is managed by its
     * owning manager and has been successfully acquired) and fires an error message
     * if not.
     */
    bool Resource::_check_is_usable() const
    {
        if ( _is_usable )
            return true;

        LOG_ERROR( TEXT("The resource \"") + _identifier + TEXT("\" is not usable anymore.") );
        return false;
    }

    /**
     * Requests/acquires the resource, bringing it into a usable state (or not).
     */
    bool Resource::_acquire(const DataProvider& data_provider)
    {
        assert( !_is_usable );
        if ( _is_usable )
        {
            LOG_WARNING( TEXT("The resource \"") + _identifier + TEXT("\" appears to have already been acquired.") );
            return false;
        }

        if ( data_provider.get_size() == 0 )
        {
            _data.clear();
        }
        else if ( !data_provider.fill( _data ) )
        {
            LOG_ERROR( TEXT("The given data provider was not able to fill the local data buffer of the resource \"") + _identifier + TEXT("\".") );
            return false;
        }

        if ( !_acquire_impl( data_provider ) )
        {
            LOG_ERROR( TEXT("The resource \"") + _identifier + TEXT("\" could not be acquired.") );
            _release();
            return false;
        }

        _is_usable = true;
        return true;
    }

    /**
     * Releases/"unacquires" the resource, thereby rendering it unusable, indicating
     * that it is no longer managed by its owning manager.
     */
    void Resource::_release()
    {
        if ( !_release_impl() )
        {
            LOG_WARNING( TEXT("The resource \"") + _identifier + TEXT("\" could not be released without problems.") );
        }

        _is_usable = false;
    }



} // namespace xl7
