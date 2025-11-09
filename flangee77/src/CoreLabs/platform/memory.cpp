#include "memory.h"

#if F77_IS_WINDOWS
#include "./detail/windows/memory.h"
namespace {
namespace impl = cl7::platform::detail::windows::memory;
}
#elif F77_IS_LINUX
#include "./detail/linux/memory.h"
namespace {
namespace impl = cl7::platform::detail::linux::memory;
}
#endif



namespace cl7::platform::memory {



    MemoryStatus::MemoryStatus()
    {
        std::memset(this, 0, sizeof(MemoryStatus));
    }

    /**
     * Retrieves information about the system's current usage of both physical and
     * virtual memory.
     */
    bool MemoryStatus::capture()
    {
        auto memory_status_result = impl::get_memory_status();
        if (!memory_status_result.has_value())
            return false;

        *this = *memory_status_result;
        return true;
    }



} // namespace cl7::platform::memory
