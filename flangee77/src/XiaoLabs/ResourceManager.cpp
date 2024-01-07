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
        , _resource_lookup()
    {
    }

    /**
     * Destructor.
     */
    ResourceManager::~ResourceManager()
    {
        assert( _resources.size() == 0 );
        assert( _resource_lookup.size() == 0 );
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Returns the number of contained resources in this resource manager.
     */
    size_t ResourceManager::get_resource_count() const
    {
        assert( _resources.size() == _resource_lookup.size() );
        return _resources.size();
    }

    /**
     * Checks whether a given resource is contained in this resource manager.
     * Time complexity: constant on average, worst case linear in the number of
     * contained resources.
     */
    bool ResourceManager::contains_resource(const Resource* resource) const
    {
        auto it = _resource_lookup.find( resource );
        if ( it == _resource_lookup.end() )
            return false;

        return get_resource( it->second ).get() != nullptr;
    }

    /**
     * Checks whether a resource with the given identifier is contained in this
     * resource manager.
     * Time complexity: linear in the number of contained resources.
     */
    bool ResourceManager::contains_resource(const cl7::string_view& identifier) const
    {
        return find_resource( identifier ).get() != nullptr;
    }

    /**
     * Returns the resource identified by the given index.
     */
    ResourcePtr ResourceManager::get_resource(size_t index) const
    {
        assert( index < _resources.size() );
        if ( index >= _resources.size() )
            return ResourcePtr();

        assert( _resources[ index ] );
        return _resources[ index ];
    }

    /**
     * Returns the resource of the given identifier.
     * Time complexity: linear in the number of contained resources.
     */
    ResourcePtr ResourceManager::find_resource(const cl7::string_view& identifier) const
    {
        for ( auto& resource_ptr : _resources )
        {
            assert( resource_ptr );
            if ( resource_ptr->get_identifier() == identifier )
                return resource_ptr;
        }

        return ResourcePtr();
    }

    /**
     * Releases the given resource
     * (deletes it and removes it from the internal list).
     */
    void ResourceManager::release_resource(Resource* resource)
    {
        auto it = _resource_lookup.find( resource );
        if ( it == _resource_lookup.end() )
            return;

        assert( it->second < _resources.size() );
        assert( _resources[ it->second ] );

        auto& resource_ptr = _resources[ it->second ];
        assert( resource_ptr );

        //if ( resource_ptr->is_acquired() )
            resource_ptr->release();

        Resource::Attorney::unmanage( resource );

        _resource_lookup.erase( it );
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

            Resource::Attorney::unmanage( resource_ptr.get() );
        }

        _resource_lookup.clear();
        _resources.clear();
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
        assert( _resource_lookup.find( resource_ptr.get() ) == _resource_lookup.end() );

        _resource_lookup.insert( std::pair( resource_ptr.get(), _resources.size() ) );
        _resources.emplace_back( std::move(resource_ptr) );
    }

    /**
     * Adds the given resource to this resource manager.
     * This operation does not request/acquire the resource.
     */
    void ResourceManager::_add_resource(Resource* resource)
    {
        assert( resource );
        assert( _resource_lookup.find( resource ) == _resource_lookup.end() );

        _resource_lookup.insert( std::pair( resource, _resources.size() ) );
        _resources.emplace_back( resource, Resource::Attorney::destroy );
    }

    /**
     * Removes the given resource from this resource manager.
     * This operation does not release the resource.
     */
    void ResourceManager::_remove_resource(Resource* resource)
    {
        auto it = _resource_lookup.find( resource );
        if ( it == _resource_lookup.end() )
            return;

        assert( it->second < _resources.size() );

        Resource::Attorney::unmanage( resource );

        _resource_lookup.erase( it );
        _resources.erase( _resources.begin() + it->second );
    }



} // namespace xl7
