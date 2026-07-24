#ifndef XL7_DRIVERBASEDCOMPONENT_H
#define XL7_DRIVERBASEDCOMPONENT_H
#include "./Component.h"

#include "./BackendRegistry.h"

#include <CoreLabs/string.h>



namespace xl7 {



template <class TSingleton>
class DriverBasedComponent
    : public Component<TSingleton>
{

public:

    /**
     * Returns the name of the driver/implementation.
     */
    virtual cl7::u8string_view get_driver_name() const = 0;

    /**
     * Returns whether any backend has been compiled in for this component at all.
     * Must be checked before the first call to instance(), since instance() has no
     * way to report a missing backend (it always returns a valid reference).
     */
    static bool has_available_backend() { return backend_registry().has_backends(); }



protected:
    using BackendRegistry = BackendRegistry<TSingleton>;

    static BackendRegistry& backend_registry()
    {
        static BackendRegistry backend_registry;
        return backend_registry;
    }

}; // class DriverBasedComponent



} // namespace xl7

#endif // XL7_DRIVERBASEDCOMPONENT_H
