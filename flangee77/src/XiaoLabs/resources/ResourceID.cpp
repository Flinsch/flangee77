#include "ResourceID.h"



namespace xl7 {
namespace resources {



    const ResourceID ResourceID::INVALID_ID = ResourceID();



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    ResourceID::ResourceID()
        : _id( id_t(-1) )
    {
    }

    /**
     * Explicit constructor.
     */
    ResourceID::ResourceID(size_t index, unsigned version)
        : _id( (static_cast<id_t>( index ) & INDEX_MASK) | ((static_cast<id_t>( version ) & VERSION_MASK) << VERSION_SHIFT) )
    {
        assert( static_cast<id_t>( index ) <= INDEX_MASK );
        assert( static_cast<id_t>( version ) <= VERSION_MASK );
    }

    /**
     * Swap operation.
     */
    void ResourceID::swap(ResourceID& rhs)
    {
        std::swap( _id, rhs._id );
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns true if the resource ID is valid, false otherwise.
     */
    bool ResourceID::is_valid() const
    {
        return _id != INVALID_ID._id;
    }

    /**
     * Returns the index part of the resource ID.
     */
    size_t ResourceID::index() const
    {
        return static_cast<size_t>( _id & INDEX_MASK );
    }

    /**
     * Extracts the version from the resource ID.
     */
    unsigned ResourceID::version() const
    {
        return static_cast<unsigned>( (_id & VERSION_MASK) >> VERSION_SHIFT );
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Invalidates this resource ID.
     */
    void ResourceID::invalidate()
    {
        _id = INVALID_ID._id;
    }



} // namespace resources
} // namespace xl7
