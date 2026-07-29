#ifndef CL7_BEHAVIORAL_SIGNAL_H
#define CL7_BEHAVIORAL_SIGNAL_H

#include <CoreLabs/root.h>

#include <algorithm>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>



namespace cl7::behavioral {



/**
 * RAII handle for a Signal subscription. Disconnects automatically when destroyed
 * (unless released or already disconnected). Deliberately not nested inside
 * Signal<Args...>, so a caller can hold connections to signals of differing
 * argument signatures uniformly (e.g., in a single std::vector<Connection>) for
 * one-shot mass-disconnect.
 *
 * Safe to outlive the Signal it was connected to: disconnecting then is a no-op
 * (see Signal's Impl/weak_ptr design).
 */
class Connection
{

public:
    Connection() noexcept = default;
    explicit Connection(std::function<void()> disconnector) noexcept
        : _disconnector(std::move(disconnector))
    {
    }

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&&) noexcept = default;

    Connection& operator=(Connection&& other) noexcept
    {
        if (this != &other)
        {
            disconnect();
            _disconnector = std::move(other._disconnector);
        }
        return *this;
    }

    ~Connection()
    {
        disconnect();
    }



    /**
     * Disconnects the subscription. Safe to call multiple times, on an empty
     * Connection, or after the originating Signal has already been destroyed
     * (all no-ops).
     */
    void disconnect()
    {
        if (_disconnector)
        {
            _disconnector();
            _disconnector = nullptr;
        }
    }

    /**
     * Returns whether this Connection currently holds a live subscription.
     */
    bool is_connected() const { return static_cast<bool>(_disconnector); }

    /**
     * Detaches from the subscription without disconnecting it: the slot stays
     * connected for the Signal's lifetime (or until the Signal itself removes it),
     * no longer tied to this Connection object.
     */
    void release() { _disconnector = nullptr; }



private:
    std::function<void()> _disconnector;

}; // class Connection



/**
 * A generic, reusable multicast signal: any number of slots (callables of matching
 * signature) can subscribe via connect(), and emit() (or operator()) invokes all of
 * them with the given arguments.
 *
 * Not thread-safe (matches the rest of the codebase: single-threaded per
 * frame/Shell throughout).
 */
template <typename... Args>
class Signal
{

public:
    using Slot = std::function<void(Args...)>;



    Signal()
        : _impl(std::make_shared<Impl>())
    {
    }

    Signal(const Signal&) = delete;
    Signal& operator=(const Signal&) = delete;
    Signal(Signal&&) = delete;
    Signal& operator=(Signal&&) = delete;

    ~Signal() = default;



    /**
     * Subscribes the specified slot, returning a Connection that disconnects it
     * when destroyed (unless released beforehand).
     */
    Connection connect(Slot slot)
    {
        const std::uint64_t id = _impl->next_id++;
        _impl->slots.push_back({.id = id, .slot = std::move(slot)});

        std::weak_ptr<Impl> weak_impl = _impl;
        return Connection([weak_impl, id]() {
            if (auto impl = weak_impl.lock())
                impl->remove(id);
        });
    }

    /**
     * Invokes all currently connected slots with the given arguments, in the order
     * they were connected. Iterates a copy of the slot list, so a slot
     * connecting/disconnecting other slots during emission is safe (just doesn't
     * affect this emit() call).
     */
    void emit(Args... args) const
    {
        const auto slots_copy = _impl->slots;
        for (const auto& entry : slots_copy)
            entry.slot(args...);
    }

    void operator()(Args... args) const { emit(args...); }



    /**
     * Returns the number of currently connected slots.
     */
    size_t slot_count() const { return _impl->slots.size(); }

    /**
     * Disconnects all currently connected slots. Outstanding Connection objects
     * remain safe to destroy/disconnect afterward (no-ops).
     */
    void disconnect_all() { _impl->slots.clear(); }



private:
    struct Entry
    {
        std::uint64_t id;
        Slot slot;
    };

    struct Impl
    {
        std::vector<Entry> slots;
        std::uint64_t next_id = 0;

        void remove(std::uint64_t id)
        {
            std::erase_if(slots, [id](const Entry& entry) {
                return entry.id == id;
            });
        }
    };

    std::shared_ptr<Impl> _impl;

}; // class Signal



} // namespace cl7::behavioral

#endif // CL7_BEHAVIORAL_SIGNAL_H
