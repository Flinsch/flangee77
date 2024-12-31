#ifndef CL7_BYTEVIEW_H
#define CL7_BYTEVIEW_H

#include <CoreLabs/root.h>

#include <span>



namespace cl7 {



    using byte_view = std::span<const std::byte>;



} // namespace cl7

#endif // CL7_BYTEVIEW_H
