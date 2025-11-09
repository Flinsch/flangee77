#ifndef CL7_PLATFORM_DETAIL_WINDOWS_MEMORY_H
#define CL7_PLATFORM_DETAIL_WINDOWS_MEMORY_H
#include <CoreLabs/platform/memory.h>
#if F77_IS_WINDOWS

#include <optional>



namespace cl7::platform::detail::windows::memory {



    std::optional<cl7::platform::memory::MemoryStatus> get_memory_status();



} // namespace cl7::platform::detail::windows::memory

#endif // F77_IS_WINDOWS
#endif // CL7_PLATFORM_DETAIL_WINDOWS_MEMORY_H
