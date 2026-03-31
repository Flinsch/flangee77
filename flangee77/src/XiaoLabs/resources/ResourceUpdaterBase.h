#ifndef XL7_RESOURCES_RESOURCEUPDATER_H
#define XL7_RESOURCES_RESOURCEUPDATER_H
#include "./AbstractResourceUpdater.h"

#include <CoreLabs/byte_span.h>
#include <CoreLabs/byte_view.h>



namespace xl7::resources {



template <class TResourceDesc, class TDirtyState>
    requires(requires {
        { TDirtyState{}.set_dirty() };
    })
class ResourceUpdaterBase
    : public AbstractResourceUpdater
{
public:
    using DirtyState = TDirtyState;



    ResourceUpdaterBase() = default;

    ResourceUpdaterBase(const TResourceDesc& desc, cl7::byte_span data, TDirtyState* dirty_state)
        : AbstractResourceUpdater(data)
        , _desc(desc)
        , _dirty_state(dirty_state ? dirty_state : &_dummy_state)
    {
    }

    template <class TResource>
        requires(std::derived_from<TResource, Resource> && requires (const TResource* resource) {
            { resource->get_desc() } -> std::convertible_to<TResourceDesc>;
        })
    ResourceUpdaterBase(const TResource* resource, cl7::byte_span data, TDirtyState* dirty_state)
        : AbstractResourceUpdater(data, resource)
        , _desc(resource->get_desc())
        , _dirty_state(dirty_state ? dirty_state : &_dummy_state)
    {
        assert(resource);
    }

    template <class TResource>
        requires(std::derived_from<TResource, Resource> && requires (const TResource* resource) {
            { resource->get_desc() } -> std::convertible_to<TResourceDesc>;
        })
    ResourceUpdaterBase(const TResource* resource, const TResourceDesc& desc, cl7::byte_span data, TDirtyState* dirty_state)
        : AbstractResourceUpdater(data, resource)
        , _desc(desc)
        , _dirty_state(dirty_state ? dirty_state : &_dummy_state)
    {
        assert(!resource || std::memcmp(&resource->get_desc(), &desc, sizeof(TResourceDesc)) == 0);
    }

    ResourceUpdaterBase(const ResourceUpdaterBase&) = delete;
    ResourceUpdaterBase& operator=(const ResourceUpdaterBase&) = delete;
    ResourceUpdaterBase(ResourceUpdaterBase&&) = delete;
    ResourceUpdaterBase& operator=(ResourceUpdaterBase&&) = delete;

    ~ResourceUpdaterBase() noexcept override
    {
        // If necessary, clear up the dummy state object.
        if (!has_custom_dirty_state())
            _dirty_state->clear();
    }



    /**
     * Replaces the entire contents of the internal buffer with the provided data.
     * Returns true if the update succeeded, false otherwise.
     *
     * This is a convenience function for full-buffer updates. The size of the
     * provided data must exactly match the size of the underlying buffer.
     *
     * On success, the internal buffer is marked as "fully dirty".
     */
    bool write_full(cl7::byte_view data)
    {
        if (!_check_exact_size(get_data().size(), data.size()))
            return false;

        if (!_write(data))
            return false;

        _dirty_state->set_dirty();
        return true;
    }



    /** Returns the descriptor of the supposedly updatable resource. */
    const TResourceDesc& get_desc() const { return _desc; }
    /** Returns the "dirty" state of the data to be updated. */
    const DirtyState& get_dirty_state() const { return *_dirty_state; }
    /** Checks if a custom dirty-state object has been explicitly specified. */
    bool has_custom_dirty_state() const { return _dirty_state != &_dummy_state; }



protected:
    /** Returns the "dirty" state of the data to be updated. */
    DirtyState& _access_dirty_state() { return *_dirty_state; }

    template <typename ...Args>
        requires(requires (Args&&... args) {
            DirtyState{}.update(std::forward<Args>(args)...);
        })
    void _update_dirty_state(Args&&... args) { _dirty_state->update(std::forward<Args>(args)...); }



private:
    static DirtyState _dummy_state;

    /** The descriptor of the supposedly updatable resource. */
    TResourceDesc _desc;
    /** The "dirty" state of the data to be updated. */
    DirtyState* _dirty_state = &_dummy_state;

}; // class ResourceUpdaterBase



} // namespace xl7::resources

#endif // XL7_RESOURCES_RESOURCEUPDATER_H
