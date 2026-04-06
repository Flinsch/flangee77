#ifndef XL7_RESOURCES_RESOURCEUPDATEMIXIN_H
#define XL7_RESOURCES_RESOURCEUPDATEMIXIN_H
#include "./Resource.h"



namespace xl7::resources {



template <class TDerived, class TUpdater>
    requires(requires (TDerived* resource, cl7::byte_span data, TUpdater::DirtyState dirty_state) {
        { TUpdater{{}, data, &dirty_state} };
    } && requires {
        { typename TUpdater::DirtyState{}.is_dirty() } -> std::convertible_to<bool>;
        { typename TUpdater::DirtyState{}.clear() };
    })
class ResourceUpdateMixin
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
            if (_resource && _resource->_dirty_state.is_dirty())
                _resource->_flush_data();
        }

    private:
        explicit Edit(TDerived* resource)
            : TUpdater(resource->get_desc(), resource->_access_data(), &resource->_dirty_state)
            , _resource(resource)
        {
        }

        TDerived* _resource = nullptr;

        friend class ResourceUpdateMixin;
    };



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
        if (is_immutable())
        {
            //LOG_ERROR(u8"The immutable " + get_qualified_identifier() + u8" cannot be updated. Subsequent 'write' calls will not have the otherwise expected effect.");
            return {};
        }

        assert(static_cast<TDerived*>(this) == dynamic_cast<TDerived*>(this));
        return Edit(static_cast<TDerived*>(this));
    }



protected:
    explicit ResourceUpdateMixin(bool immutable)
        : _immutable(immutable)
    {
    }



    /**
     * Flushes recent changes made to the local data copy (if any) by transferring
     * them to the hardware and, after successful transfer, clears the "dirty" state.
     */
    void _flush_data()
    {
        if (!_dirty_state.is_dirty())
            return;

        assert(static_cast<TDerived*>(this) == dynamic_cast<TDerived*>(this));
        if (static_cast<TDerived*>(this)->_flush_data_impl())
            _dirty_state.clear();
    }



private:
    /** True if this supposedly updatable resource is actually immutable. */
    bool _immutable;

    /** The "dirty" state of the local data copy of this updatable resource. */
    DirtyState _dirty_state;

};



} // namespace xl7::resources

#endif // XL7_RESOURCES_RESOURCEUPDATEMIXIN_H
