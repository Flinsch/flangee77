#ifndef XL7_RESOURCES_RESOURCEUSAGE_H
#define XL7_RESOURCES_RESOURCEUSAGE_H

#include <CoreLabs/root.h>



namespace xl7::resources {



/**
 * A good read for this:
 * https://developer.nvidia.com/sites/default/files/akamai/gamedev/files/gdc12/Efficient_Buffer_Management_McDonald.pdf
 */



enum struct ResourceUsage
{
    /** A non-static/"reusable" resource that is expected to last for "a while". */
    Default,
    /** A static resource whose data cannot be changed once it has been acquired. */
    Immutable,
    /** A dynamic/volatile resource that will be updated very frequently (e.g., once per frame). */
    Dynamic,
};



} // namespace xl7::resources

#endif // XL7_RESOURCES_RESOURCEUSAGE_H
