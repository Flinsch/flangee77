#ifndef CL7_PLATFORM_DETAIL_LINUX_MEMORY_H
#define CL7_PLATFORM_DETAIL_LINUX_MEMORY_H
#include <CoreLabs/platform/memory.h>
#if F77_IS_LINUX

#include <optional>



namespace cl7::platform::detail::linux::memory {



    std::optional<cl7::platform::memory::MemoryStatus> get_memory_status();



} // namespace cl7::platform::detail::linux::memory

#endif // F77_IS_LINUX
#endif // CL7_PLATFORM_DETAIL_LINUX_MEMORY_H
