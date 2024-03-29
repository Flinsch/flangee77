#include "ResourceManager.h"



namespace xl7 {
namespace resources {



    void ResourceManager::_destroy_resource(Resource* resource)
    {
        Resource::Attorney::destroy( resource );
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    ResourceManager::ResourceManager()
        : _resources()
        , _resource_lookup()
        , _free_indices()
    {
    }

    /**
     * Destructor.
     */
    ResourceManager::~ResourceManager()
    {
        assert( get_resource_count() == 0 );
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Returns the number of contained resources in this resource manager.
     */
    size_t ResourceManager::get_resource_count() const
    {
        assert( _resources.size() - _free_indices.size() == _resource_lookup.size() );
        return _resource_lookup.size();
    }

    /**
     * Checks whether a given resource is contained in this resource manager.
     * Time complexity: constant.
     */
    bool ResourceManager::contains_resource(const Resource* resource) const
    {
        if ( !resource )
            return false;

        return contains_resource( resource->get_id() );
    }

    /**
     * Checks whether a resource with the given resource ID is contained in this
     * resource manager.
     * Time complexity: constant.
     */
    bool ResourceManager::contains_resource(ResourceID id) const
    {
        return find_resource( id ) != nullptr;
    }

    /**
     * Checks whether a resource with the given identifier is contained in this
     * resource manager.
     * Time complexity: linear in the number of contained resources.
     */
    bool ResourceManager::contains_resource(cl7::astring_view identifier) const
    {
        return find_resource( identifier ) != nullptr;
    }

    /**
     * Returns the resource identified by the given index.
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    Resource* ResourceManager::get_resource(size_t index) const
    {
        assert( index < get_resource_count() );
        if ( index >= get_resource_count() )
            return nullptr;

        if ( _free_indices.empty() )
            return _resources[ index ].ptr.get();

        for ( const auto& entry : _resources )
        {
            if ( !entry.ptr )
                continue;
            if ( index == 0 )
                return entry.ptr.get();
            --index;
        }

        return nullptr;
    }

    /**
     * Returns the resource of the given ID.
     * Time complexity: constant.
     */
    Resource* ResourceManager::find_resource(ResourceID id) const
    {
        size_t index = id.index();
        if ( index >= _resources.size() )
            return nullptr;

        auto &entry = _resources[ index ];
        if ( entry.id != id )
            return nullptr;

        Resource* resource = entry.ptr.get();
        if ( !resource )
            return nullptr;

        assert( resource->get_id() == id );
        return resource;
    }

    /**
     * Returns the resource of the given identifier.
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    Resource* ResourceManager::find_resource(cl7::astring_view identifier) const
    {
        auto it = _resource_lookup.find( identifier );
        if ( it == _resource_lookup.end() )
            return nullptr;

        return find_resource( it->second );
    }

    /**
     * Releases the specified resource. If its reference count reaches zero, the
     * resource is actually disposed/"unacquired" (and removed from this resource
     * manager).
     * Time complexity: constant.
     */
    bool ResourceManager::release_resource(Resource* resource)
    {
        if ( !resource )
            return false;

        return release_resource( resource->get_id() );
    }

    /**
     * Releases the specified resource. If its reference count reaches zero, the
     * resource is actually disposed/"unacquired" (and removed from this resource
     * manager).
     * Time complexity: constant.
     */
    bool ResourceManager::release_resource(ResourceID id)
    {
        size_t index = id.index();
        if ( index >= _resources.size() )
            return false;

        auto &entry = _resources[ index ];
        if ( entry.id != id )
            return false;

        auto& resource_ptr = entry.ptr;
        Resource* resource = resource_ptr.get();
        if ( !resource )
            return false;

        assert( resource->get_id() == id );

        auto it = _resource_lookup.find( resource->get_identifier() );
        assert( it != _resource_lookup.end() );

        Resource::Attorney::release( resource );
        if ( resource->get_reference_count() > 0 )
            return true;

        _resource_lookup.erase( it );
        _free_indices.push_back( index );

        resource_ptr.reset();

        return true;
    }

    /**
     * Releases the specified resource and invalidates the given ID. If the
     * reference count reaches zero, the resource is actually disposed/"unacquired"
     * (and removed from this resource manager).
     * Time complexity: constant.
     */
    bool ResourceManager::release_resource_and_invalidate(ResourceID& id)
    {
        if ( !release_resource( id ) )
            return false;

        id.invalidate();
        return true;
    }

    /**
     * Releases the specified resource. If its reference count reaches zero, the
     * resource is actually disposed/"unacquired" (and removed from this resource
     * manager).
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    bool ResourceManager::release_resource(cl7::astring_view identifier)
    {
        return release_resource( find_resource( identifier ) );
    }

    /**
     * Disposes/"unacquires" all managed resources (and removes them from this
     * resource manager).
     */
    void ResourceManager::dispose_resources()
    {
        for ( auto& entry : _resources )
        {
            Resource::Attorney::dispose( entry.ptr.get() );
        }

        _free_indices.clear();
        _resource_lookup.clear();
        _resources.clear();
    }



    // #############################################################################
    // Management Functions
    // #############################################################################

    /**
     * Generates and returns the next resource ID to use when adding a new resource.
     */
    ResourceID ResourceManager::_next_id()
    {
        if ( _free_indices.empty() )
            return ResourceID( _resources.size(), 0 );

        size_t index = _free_indices.back();
        assert( index < _resources.size() );
        return ResourceID( index, _resources[ index ].id.version() + 1 );
    }

    /**
     * Adds the given resource to this resource manager (and returns the ID of the
     * resource), but only if it can be acquired in this process. Returns an invalid
     * ID otherwise.
     */
    ResourceID ResourceManager::_try_acquire_and_add_resource(ResourcePtr resource_ptr, const DataProvider& data_provider)
    {
        assert( resource_ptr );
        if ( !resource_ptr )
            return ResourceID();

        assert( !resource_ptr->is_usable() );
        if ( !Resource::Attorney::acquire( resource_ptr.get(), data_provider ) )
            return ResourceID();

        return _add_resource( std::move(resource_ptr) );
    }

    /**
     * Adds the given resource to this resource manager. This operation does not
     * request/acquire the resource. This must have happened successfully before.
     * Returns the ID of the resource.
     */
    ResourceID ResourceManager::_add_resource(ResourcePtr resource_ptr)
    {
        assert( resource_ptr );
        assert( resource_ptr->is_usable() );
        assert( _resource_lookup.find( resource_ptr->get_identifier() ) == _resource_lookup.end() );

        ResourceID id = resource_ptr->get_id();
        size_t index = id.index();
        assert( index <= _resources.size() );

        _resource_lookup.insert( { resource_ptr->get_identifier(), id } );

        if ( index == _resources.size() )
        {
            _resources.emplace_back( id, std::move(resource_ptr) );
        }
        else
        {
            assert( _resources[ index ].id.index() == index );
            assert( _resources[ index ].id.version() + 1 == id.version() );
            assert( !_resources[ index ].ptr );

            assert( _free_indices.size() > 0 );
            assert( _free_indices.back() == index );

            _resources[ index ].id = id;
            _resources[ index ].ptr.swap( resource_ptr );
            _free_indices.pop_back();
        }

        assert( get_resource_count() > 0 );
        return id;
    }



} // namespace resources
} // namespace xl7
