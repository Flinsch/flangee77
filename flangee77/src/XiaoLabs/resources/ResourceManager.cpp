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
        if ( !resource )
            return false;

        auto it = _resource_lookup.find( resource->get_identifier() );
        if ( it == _resource_lookup.end() )
            return false;

        assert( it->second.get() == resource );
        return true;
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
            return {};

        assert( _resources[ index ] );
        return _resources[ index ];
    }

    /**
     * Returns the resource of the given identifier.
     * Time complexity: linear in the number of contained resources.
     */
    ResourcePtr ResourceManager::find_resource(const cl7::string_view& identifier) const
    {
        auto it = _resource_lookup.find( identifier );
        if ( it == _resource_lookup.end() )
            return {};

        assert( it->second.get()->get_identifier() == identifier );
        return it->second;
    }

    /**
     * Releases the given resource (and removes it from this resource manager).
     * Time complexity: linear in the number of contained resources.
     */
    void ResourceManager::release_resource(Resource* resource)
    {
        if ( !resource )
            return;

        auto it = _resource_lookup.find( resource->get_identifier() );
        if ( it == _resource_lookup.end() )
            return;

        assert( it->second.get() == resource );

        Resource::Attorney::release( resource );

        _resources.erase( std::find( _resources.begin(), _resources.end(), it->second ) );
        _resource_lookup.erase( it );
        assert( _resources.size() == _resource_lookup.size() );
    }

    /**
     * Releases all managed resources (and removes them from this resource manager).
     */
    void ResourceManager::release_resources()
    {
        for ( auto& resource_ptr : _resources )
        {
            Resource::Attorney::release( resource_ptr.get() );
        }

        _resource_lookup.clear();
        _resources.clear();
    }



    // #############################################################################
    // Management Functions
    // #############################################################################

    /**
     * Adds the given resource to this resource manager. This operation does not
     * request/acquire the resource. This must have happened successfully before.
     */
    void ResourceManager::_add_resource(ResourcePtr resource_ptr)
    {
        assert( resource_ptr );
        assert( resource_ptr->is_usable() );
        assert( _resource_lookup.find( resource_ptr->get_identifier() ) == _resource_lookup.end() );

        _resource_lookup.insert( { resource_ptr->get_identifier(), resource_ptr } );
        _resources.emplace_back( resource_ptr );
        assert( _resources.size() == _resource_lookup.size() );
    }



} // namespace resources
} // namespace xl7
