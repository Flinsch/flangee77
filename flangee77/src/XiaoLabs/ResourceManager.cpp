#include "ResourceManager.h"



namespace xl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    ResourceManager::ResourceManager()
        : _resources()
        , _resource_pointer_lookup()
        , _resource_identifier_lookup()
    {
    }

    /**
     * Destructor.
     */
    ResourceManager::~ResourceManager()
    {
        assert( _resources.size() == 0 );
        assert( _resource_pointer_lookup.size() == 0 );
        assert( _resource_identifier_lookup.size() == 0 );
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Returns the number of contained resources in this resource manager.
     */
    size_t ResourceManager::get_resource_count() const
    {
        assert( _resources.size() == _resource_pointer_lookup.size() );
        assert( _resources.size() == _resource_identifier_lookup.size() );
        return _resources.size();
    }

    /**
     * Checks whether a given resource is contained in this resource manager.
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    bool ResourceManager::contains_resource(const Resource* resource) const
    {
        auto it = _resource_pointer_lookup.find( resource );
        if ( it == _resource_pointer_lookup.end() )
            return false;

        return get_resource( it->second ) != nullptr;
    }

    /**
     * Checks whether a resource with the given identifier is contained in this
     * resource manager.
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    bool ResourceManager::contains_resource(const cl7::string_view& identifier) const
    {
        auto it = _resource_identifier_lookup.find( identifier );
        if ( it == _resource_identifier_lookup.end() )
            return false;

        return get_resource( it->second ) != nullptr;
    }

    /**
     * Returns the resource identified by the given index.
     */
    Resource* ResourceManager::get_resource(size_t index) const
    {
        assert( index < _resources.size() );
        if ( index >= _resources.size() )
            return nullptr;

        assert( _resources[ index ] );
        return _resources[ index ].get();
    }

    /**
     * Returns the resource of the given identifier.
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    Resource* ResourceManager::find_resource(const cl7::string_view& identifier) const
    {
        auto it = _resource_identifier_lookup.find( identifier );
        if ( it == _resource_identifier_lookup.end() )
            return nullptr;

        return get_resource( it->second );
    }

    /**
     * Releases the given resource
     * (deletes it and removes it from the internal list).
     */
    void ResourceManager::release_resource(Resource* resource)
    {
        auto it = _resource_pointer_lookup.find( resource );
        if ( it == _resource_pointer_lookup.end() )
            return;

        assert( it->second < _resources.size() );
        assert( it->second == _resource_identifier_lookup.find( resource->get_identifier() )->second );
        assert( _resources[ it->second ] );

        auto& resource_ptr = _resources[ it->second ];
        assert( resource_ptr );

        //if ( resource_ptr->is_acquired() )
            resource_ptr->release();

        _resource_identifier_lookup.erase( resource->get_identifier() );
        _resource_pointer_lookup.erase( it );
        _resources.erase( _resources.begin() + it->second );
    }

    /**
     * Releases the contained resources
     * (deletes them and removes them from the internal list).
     */
    void ResourceManager::release_resources()
    {
        for ( auto& resource_ptr : _resources )
        {
            assert( resource_ptr );
            //if ( resource_ptr->is_acquired() )
                resource_ptr->release();
        }

        _resource_identifier_lookup.clear();
        _resource_pointer_lookup.clear();
        _resources.clear();
    }

    /**
     * Temporarily releases/resigns the contained resources.
     */
    void ResourceManager::resign_resources()
    {
        for ( auto& resource_ptr : _resources )
        {
            assert( resource_ptr );
            if ( resource_ptr->is_acquired() && !resource_ptr->is_resigned() )
                resource_ptr->resign();
        }
    }

    /**
     * Reaquires/restores the contained resources.
     */
    void ResourceManager::restore_resources()
    {
        for ( auto& resource_ptr : _resources )
        {
            assert( resource_ptr );
            if ( resource_ptr->is_acquired() && resource_ptr->is_resigned() )
                resource_ptr->restore();
        }
    }



    // #############################################################################
    // Protected Methods
    // #############################################################################

    /**
     * Adds the given resource to this resource manager.
     * This operation does not request/acquire the resource.
     */
    void ResourceManager::_add_resource(ResourcePtr resource_ptr)
    {
        assert( resource_ptr );
        assert( _resource_pointer_lookup.find( resource_ptr.get() ) == _resource_pointer_lookup.end() );
        assert( _resource_identifier_lookup.find( resource_ptr->get_identifier() ) == _resource_identifier_lookup.end() );

        _resource_identifier_lookup.insert( std::pair( resource_ptr->get_identifier(), _resources.size() ) );
        _resource_pointer_lookup.insert( std::pair( resource_ptr.get(), _resources.size() ) );
        _resources.emplace_back( std::move(resource_ptr) );
    }

    /**
     * Adds the given resource to this resource manager.
     * This operation does not request/acquire the resource.
     */
    void ResourceManager::_add_resource(Resource* resource)
    {
        assert( resource );
        assert( _resource_pointer_lookup.find( resource ) == _resource_pointer_lookup.end() );
        assert( _resource_identifier_lookup.find( resource->get_identifier() ) == _resource_identifier_lookup.end() );

        _resource_identifier_lookup.insert( std::pair( resource->get_identifier(), _resources.size() ) );
        _resource_pointer_lookup.insert( std::pair( resource, _resources.size() ) );
        _resources.emplace_back( resource, Resource::Attorney::destroy );
    }

    /**
     * Removes the given resource from this resource manager.
     * This operation does not release the resource.
     */
    void ResourceManager::_remove_resource(Resource* resource)
    {
        auto it = _resource_pointer_lookup.find( resource );
        if ( it == _resource_pointer_lookup.end() )
            return;

        assert( it->second < _resources.size() );
        assert( it->second == _resource_identifier_lookup.find( resource->get_identifier() )->second );

        _resource_identifier_lookup.erase( resource->get_identifier() );
        _resource_pointer_lookup.erase( it );
        _resources.erase( _resources.begin() + it->second );
    }



} // namespace xl7
