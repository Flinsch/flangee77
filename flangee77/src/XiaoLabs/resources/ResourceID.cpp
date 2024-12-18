#include "ResourceID.h"



namespace xl7::resources {



    const ResourceID ResourceID::INVALID_ID = ResourceID();



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Creates an invalid resource ID.
     */
    ResourceID::ResourceID() noexcept
        : _id(static_cast<id_t>(-1))
    {
    }

    ResourceID::ResourceID(size_t index, unsigned version)
        : _id((static_cast<id_t>(index) & INDEX_MASK) | ((static_cast<id_t>(version) & VERSION_MASK) << VERSION_SHIFT))
    {
        assert(static_cast<id_t>(index) <= INDEX_MASK);
        assert(static_cast<id_t>(version) <= VERSION_MASK);
    }

    void ResourceID::swap(ResourceID& other) noexcept
    {
        std::swap(_id, other._id);
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns true if the resource ID is valid, false otherwise.
     */
    bool ResourceID::is_valid() const noexcept
    {
        return _id != INVALID_ID._id;
    }

    /**
     * Returns the index part of the resource ID.
     */
    size_t ResourceID::index() const noexcept
    {
        return static_cast<size_t>(_id & INDEX_MASK);
    }

    /**
     * Extracts the version from the resource ID.
     */
    unsigned ResourceID::version() const noexcept
    {
        return static_cast<unsigned>((_id & VERSION_MASK) >> VERSION_SHIFT);
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Invalidates this resource ID.
     */
    void ResourceID::invalidate() noexcept
    {
        _id = INVALID_ID._id;
    }



} // namespace xl7::resources
