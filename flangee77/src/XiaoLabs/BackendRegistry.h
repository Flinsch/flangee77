#ifndef XL7_BACKENDREGISTRY_H
#define XL7_BACKENDREGISTRY_H

#include "./DriverResolver.h"

#include <CoreLabs/static_vector.h>

#include <algorithm>
#include <functional>
#include <initializer_list>



namespace xl7 {



template <class TBackendBase>
class BackendRegistry
{

public:
    using Factory = std::function<TBackendBase*()>;

    static constexpr size_t MAX_BACKEND_ENTRIES = 10;
    static constexpr size_t MAX_ALIASES_PER_ENTRY = 20;

    struct RegistryEntry
    {
        cl7::u8string_view driver_name;
        cl7::static_vector<cl7::u8string_view, MAX_ALIASES_PER_ENTRY> aliases;
        Factory factory;
        int priority;
    };

    void register_backend(cl7::u8string_view driver_name, std::initializer_list<cl7::u8string_view> aliases, Factory factory, int priority)
    {
        assert(!_resolved);
        assert(!_entries.full());
        _entries.emplace_back(driver_name, aliases, factory, priority);
    }

    TBackendBase* resolve(cl7::u8string_view requested_name)
    {
        _resolved = true;
        assert(!_entries.empty());

        std::ranges::sort(_entries, [](const auto& a, const auto& b) {
            return a.priority > b.priority;
        });

        DriverResolver::DriverEntries driver_entries;
        for (const auto& entry : _entries)
        {
            driver_entries.push_back({cl7::u8string{entry.driver_name}, {}});
            for (const auto& alias : entry.aliases)
                driver_entries.back().aliases.push_back(cl7::u8string{alias});
        }

        const auto resolved_name = DriverResolver::resolve_driver_name(driver_entries, requested_name);

        for (const auto& entry : _entries)
        {
            if (resolved_name == entry.driver_name)
                return entry.factory();
        }

        assert(false);
        return nullptr;
    }

private:
    cl7::static_vector<RegistryEntry, MAX_BACKEND_ENTRIES> _entries;
    bool _resolved = false;

}; // class BackendRegistry



} // namespace xl7

#endif // XL7_BACKENDREGISTRY_H
