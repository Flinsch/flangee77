#ifndef XL7_DRIVERRESOLVER_H
#define XL7_DRIVERRESOLVER_H

#include <CoreLabs/string.h>

#include <vector>



namespace xl7 {



struct DriverResolver
{

    struct DriverEntry
    {
        cl7::u8string name;
        std::vector<cl7::u8string> aliases;
    };

    using DriverEntries = std::vector<DriverEntry>;

    /**
     * Returns the driver name of the specified entries that most closely matches
     * the requested configuration name. A name is always returned; in case of
     * doubt, the first entry wins.
     */
    static cl7::u8string resolve_driver_name(const DriverEntries& driver_entries, cl7::u8string_view config_name);

}; // class DriverResolver



} // namespace xl7

#endif // XL7_DRIVERRESOLVER_H
