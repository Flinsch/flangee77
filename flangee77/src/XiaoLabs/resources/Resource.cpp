#include "Resource.h"

#include "./ResourceManager.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/string.h>



namespace xl7::resources {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Returns a "human-friendly" string that combines the resource type with the
     * identifier of this resource. If the resource has no identifier, only the type
     * is returned.
     */
    cl7::u8string Resource::get_qualified_identifier() const
    {
        auto type_string = get_type_string();
        auto identifier = get_identifier();

        if (identifier.empty())
            return cl7::u8string(type_string);

        cl7::u8string result;
        result.reserve(type_string.size() + identifier.size() + 3); // 1 space + 2 quotes
        result.append(type_string);
        result.append(u8" \"");
        result.append(identifier);
        result.append(u8"\"");

        return result;
    }

    /**
     * Increases the reference count. A call to this function should be paired with
     * a call to the release function (or one of the manager's release functions).
     */
    void Resource::add_reference()
    {
        if (!_is_usable)
        {
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" appears to have already been released.");
            return;
        }

        assert(_reference_count > 0);
        ++_reference_count;
    }

    /**
     * Releases the resource. If the reference count reaches zero, the resource is
     * actually disposed/"unacquired" (and removed from its owning manager), thereby
     * rendering it unusable.
     * Time complexity: linear in the number of contained resources of the owning
     * manager.
     */
    void Resource::release()
    {
        if (!_is_usable)
        {
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" appears to have already been released.");
            return;
        }

        assert(_manager);
        _manager->release_resource(this);
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Resource::Resource(const CreateContext& ctx)
        : _manager(ctx.manager)
        , _id(ctx.id)
        , _identifier(ctx.identifier)
        , _reference_count(1)
        , _is_usable(false)
    {
        assert(_manager);
    }

    Resource::~Resource()
    {
        assert(_reference_count <= 1);
        assert(!_is_usable);
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
        if (_is_usable)
            return true;

        LOG_ERROR(u8"The " + get_qualified_identifier() + u8" is not in a usable state.");
        return false;
    }

    /**
     * Checks whether the specified flag indicates that this supposedly updatable
     * resource is actually mutable and fires an error message if not.
     */
    bool Resource::_check_is_mutable(bool is_mutable) const
    {
        if (is_mutable)
            return true;

        LOG_ERROR(u8"The supposedly updatable " + get_qualified_identifier() + u8" is immutable and therefore cannot be updated.");
        return false;
    }



    // #############################################################################
    // Lifetime Management
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state (or not).
     */
    bool Resource::_acquire()
    {
        assert(!_is_usable);
        if (_is_usable)
        {
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" appears to have already been acquired.");
            return false;
        }

        if (!_acquire_impl())
        {
            LOG_ERROR(u8"The " + get_qualified_identifier() + u8" could not be acquired.");
            _dispose();
            return false;
        }

        _is_usable = true;
        return true;
    }

    /**
     * Disposes/"unacquires" the resource, thereby rendering it unusable, indicating
     * that it is no longer managed by its owning manager.
     */
    void Resource::_dispose()
    {
        if (!_dispose_impl())
        {
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" could not be released without problems.");
        }

        _is_usable = false;
    }

    /**
     * Decreases the reference count. If it reaches zero, the resource is actually
     * disposed/"unacquired", thereby rendering it unusable, indicating that it is
     * no longer managed by its owning manager.
     */
    void Resource::_release()
    {
        assert(_reference_count > 0);
        if (--_reference_count > 0)
            return;

        _dispose();
    }



} // namespace xl7::resources
