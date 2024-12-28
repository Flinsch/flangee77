#include "Resource.h"

#include "./ResourceManager.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/strings.h>



namespace xl7::resources {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Returns the identifier of this resource (if specified, empty otherwise).
     */
    cl7::u8string Resource::get_identifier_string() const
    {
        return get_identifier();
    }

    /**
     * Increases the reference count. A call to this function should be paired with
     * a call to the release function (or one of the manager's release functions).
     */
    void Resource::add_reference()
    {
        if (!_is_usable)
        {
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" appears to have already been released.");
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
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" appears to have already been released.");
            return;
        }

        assert(_manager);
        _manager->release_resource(this);
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Resource::Resource(ResourceManager* manager, ResourceID id, cl7::u8string_view identifier)
        : _manager(manager)
        , _id(id)
        , _identifier(identifier)
        , _reference_count(1)
        , _is_usable(false)
    {
        assert(_manager);
    }

    /**
     * Destructor.
     */
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

        LOG_ERROR(u8"The " + get_typed_identifier_string() + u8" is not usable anymore.");
        return false;
    }

    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource and, if so, (re)populates the local data buffer.
     */
    bool Resource::_try_fill_data(const DataProvider& data_provider)
    {
        if (!_check_data_impl(data_provider))
        {
            return false;
        }

        return _fill_data_impl(data_provider);
    }

    /**
     * Checks whether the given data provider complies with the specified total data
     * size and fires an error message if not.
     */
    bool Resource::_check_against_size(const DataProvider& data_provider, size_t size) const
    {
        if (data_provider.get_offset() + data_provider.get_size() > size)
        {
            LOG_ERROR(u8"The data offset and size provided for " + get_typed_identifier_string() + u8" would exceed the total size of the " + cl7::u8string(get_type_string()) + u8".");
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
        assert(stride != 0);

        bool is_valid = true;

        if (data_provider.get_offset() % stride != 0)
        {
            LOG_ERROR(u8"The data offset provided for " + get_typed_identifier_string() + u8" does not match the element size of the " + cl7::u8string(get_type_string()) + u8".");
            is_valid = false;
        }

        if (data_provider.get_size() % stride != 0)
        {
            LOG_ERROR(u8"The data size provided for " + get_typed_identifier_string() + u8" does not match the element size of the " + cl7::u8string(get_type_string()) + u8".");
            is_valid = false;
        }

        return is_valid;
    }



    // #############################################################################
    // Prototypes
    // #############################################################################

    /**
     * (Re)populates the local data buffer based on the given data provider.
     */
    bool Resource::_fill_data_impl(const DataProvider& data_provider)
    {
        data_provider.fill(_data);

        return true;
    }



    // #############################################################################
    // Lifetime Management
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state (or not).
     */
    bool Resource::_acquire(const DataProvider& data_provider)
    {
        assert(!_is_usable);
        if (_is_usable)
        {
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" appears to have already been acquired.");
            return false;
        }

        if (data_provider.get_size() > 0 && !_try_fill_data(data_provider))
        {
            return false;
        }

        if (!_acquire_impl(data_provider))
        {
            LOG_ERROR(u8"The " + get_typed_identifier_string() + u8" could not be acquired.");
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
            LOG_WARNING(u8"The " + get_typed_identifier_string() + u8" could not be released without problems.");
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
