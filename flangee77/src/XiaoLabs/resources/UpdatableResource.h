#ifndef XL7_RESOURCES_UPDATABLERESOURCE_H
#define XL7_RESOURCES_UPDATABLERESOURCE_H
#include "./ResourceWithData.h"



namespace xl7::resources {



template <class TDerived, class TUpdater, class TBase>
    requires(requires (TDerived* resource, cl7::byte_span data, TUpdater::DirtyState dirty_state) {
        { TUpdater{{}, data, &dirty_state} };
    } && requires {
        { typename TUpdater::DirtyState{}.is_dirty() } -> std::convertible_to<bool>;
        { typename TUpdater::DirtyState{}.clear() };
    } && std::derived_from<TBase, Resource>)
class UpdatableResource
    : public ResourceBase<TDerived, TBase>
{

public:
    using DirtyState = TUpdater::DirtyState;



    class Edit
        : public TUpdater
    {
    public:
        Edit() = default;

        Edit(const Edit&) = delete;
        Edit& operator=(const Edit&) = delete;
        Edit(Edit&& other) = default;
        Edit& operator=(Edit&& other) = default;

        ~Edit()
        {
            // If the resource exists and its "dirty" state is actually "dirty",
            // then flush the data, but only if the resource is usable (i.e., acquired).
            // Otherwise, it is the responsibility of the resource implementation
            // to utilize the "dirty" data during the acquisition process.
            if (_resource && _resource->_dirty_state.is_dirty() && _resource->is_usable())
                _resource->_flush_data();
        }

    private:
        explicit Edit(TDerived* resource)
            : TUpdater(resource->get_desc(), resource->_access_data(), &resource->_dirty_state)
            , _resource(resource)
        {
        }

        TDerived* _resource = nullptr;

        friend class UpdatableResource;
    };



    UpdatableResource() = delete;

    UpdatableResource(const UpdatableResource&) = delete;
    UpdatableResource& operator=(const UpdatableResource&) = delete;
    UpdatableResource(UpdatableResource&&) = delete;
    UpdatableResource& operator=(UpdatableResource&&) = delete;



    /** Returns true if this supposedly updatable resource is actually mutable. */
    bool is_mutable() const { return !_immutable; }

    /** Returns true if this supposedly updatable resource is actually immutable. */
    bool is_immutable() const { return _immutable; }

    /** Returns the "dirty" state of the local data copy of this updatable resource. */
    const DirtyState& get_dirty_state() const { return _dirty_state; }

    /** Returns true if the local data copy of this updatable resource is "dirty". */
    bool is_dirty() const { return _dirty_state.is_dirty(); }

    /**
     * Begins a scoped modification of the resource's local data copy. All changes
     * performed through the returned RAII object are accumulated, and once the
     * object goes out of scope, the resource automatically commits the modified
     * data if any changes were made.
     *
     * If the supposedly updatable resource is actually immutable, subsequent
     * modifications will not have the otherwise expected effect.
     */
    Edit edit()
    {
        // If the resource has not yet been fully acquired (not usable),
        // it can of course be edited, despite any immutability,
        // in order to provide initial data for acquisition.
        if (is_immutable() && this->is_usable())
        {
            // But if the resource has already been acquired and is immutable,
            // then of course it must not be edited.
            this->_check_is_mutable(false);
            assert(false);
            return {};
        }

        assert(static_cast<TDerived*>(this) == dynamic_cast<TDerived*>(this));
        return Edit(static_cast<TDerived*>(this));
    }



    /**
     * Checks whether the specified flag indicates that this supposedly updatable
     * resource is actually mutable and fires an error message if not.
     */
    bool check_is_mutable() const { return this->_check_is_mutable(is_mutable()); }



protected:
    template <typename ...Args>
    UpdatableResource(const Resource::CreateContext& ctx, bool immutable, Args&&... args)
        : ResourceBase<TDerived, TBase>(ctx, std::forward<Args>(args)...)
        , _immutable(immutable)
    {
    }

    ~UpdatableResource() override = default;



    /**
     * Flushes recent changes made to the local data copy (if any) by transferring
     * them to the hardware, and after successful transfer clears the "dirty" state
     * and returns true (false otherwise).
     */
    bool _flush_data()
    {
        if (!_dirty_state.is_dirty())
            return true;

        if (!_flush_data_impl())
            return false;

        _dirty_state.clear();
        return true;
    }



private:
    /**
     * Flushes recent changes made to the local data copy by transferring them
     * "dirty" parts to the hardware and returns true after a successful transfer.
     */
    virtual bool _flush_data_impl() = 0;



    /** True if this supposedly updatable resource is actually immutable. */
    bool _immutable;

    /** The "dirty" state of the local data copy of this updatable resource. */
    DirtyState _dirty_state;

}; // class UpdatableResource



} // namespace xl7::resources

#endif // XL7_RESOURCES_UPDATABLERESOURCE_H
