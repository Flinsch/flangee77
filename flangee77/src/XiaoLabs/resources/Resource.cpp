#include "Resource.h"

#include "./ResourceManager.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace resources {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Resource::Resource(ResourceManager* manager, ResourceID id, const cl7::string_view& identifier)
        : _manager( manager )
        , _id( id )
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

    /**
     * Releases/"unacquires" the resource and removes it from its owning manager,
     * thereby rendering it unusable.
     * Time complexity: linear in the number of contained resources of the owning
     * manager.
     */
    void Resource::release()
    {
        if ( !_is_usable )
        {
            LOG_WARNING( TEXT("The ") + get_typed_identifier_string() + TEXT(" appears to have already been released.") );
            return;
        }

        assert( _manager );
        _manager->release_resource( this );
    }



    // #############################################################################
    // Lifetime Management
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state (or not).
     */
    bool Resource::_acquire(const DataProvider& data_provider)
    {
        assert( !_is_usable );
        if ( _is_usable )
        {
            LOG_WARNING( TEXT("The ") + get_typed_identifier_string() + TEXT(" appears to have already been acquired.") );
            return false;
        }

        if ( !_try_fill_data( data_provider ) )
        {
            return false;
        }

        if ( !_acquire_impl( data_provider ) )
        {
            LOG_ERROR( TEXT("The ") + get_typed_identifier_string() + TEXT(" could not be acquired.") );
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
            LOG_WARNING( TEXT("The ") + get_typed_identifier_string() + TEXT(" could not be released without problems.") );
        }

        _is_usable = false;
    }



    // #############################################################################
    // Helpers
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

        LOG_ERROR( TEXT("The ") + get_typed_identifier_string() + TEXT(" is not usable anymore.") );
        return false;
    }

    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource and, if so, (re)populates the local data buffer.
     */
    bool Resource::_try_fill_data(const DataProvider& data_provider)
    {
        if ( !_check_impl( data_provider ) )
        {
            return false;
        }

        data_provider.fill( _data );

        return true;
    }

    /**
     * Checks whether the given data provider complies with the specified total data
     * size and fires an error message if not.
     */
    bool Resource::_check_against_size(const DataProvider& data_provider, size_t size) const
    {
        if ( data_provider.get_offset() + data_provider.get_size() > size )
        {
            LOG_ERROR( TEXT("The data offset and size provided for ") + get_typed_identifier_string() + TEXT(" would exceed the total size of the ") + cl7::string(get_type_string()) + TEXT(".") );
            return false;
        }

        return true;
    }

    /**
     * Checks whether the given data provider complies with the specified data
     * element/chunk size and fires an error message if not.
     */
    bool Resource::_check_against_stride(const DataProvider& data_provider, size_t stride) const
    {
        assert( stride != 0 );

        bool is_valid = true;

        if ( data_provider.get_offset() % stride != 0 )
        {
            LOG_ERROR( TEXT("The data offset provided for ") + get_typed_identifier_string() + TEXT(" does not match the element size of the ") + cl7::string(get_type_string()) + TEXT(".") );
            is_valid = false;
        }

        if ( data_provider.get_size() % stride != 0 )
        {
            LOG_ERROR( TEXT("The data size provided for ") + get_typed_identifier_string() + TEXT(" does not match the element size of the ") + cl7::string(get_type_string()) + TEXT(".") );
            is_valid = false;
        }

        return is_valid;
    }



} // namespace resources
} // namespace xl7
