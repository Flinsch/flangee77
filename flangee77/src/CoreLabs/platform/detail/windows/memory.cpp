#include "memory.h"
#if F77_IS_WINDOWS

#include <CoreLabs/logging.h>
#include <CoreLabs/platform/errors.h>

#include <Windows.h>



namespace cl7::platform::detail::windows::memory {



    std::optional<cl7::platform::memory::MemoryStatus> get_memory_status()
    {
        cl7::platform::memory::MemoryStatus memory_status;

        MEMORYSTATUSEX memory_status_ex = {};
        memory_status_ex.dwLength = sizeof(memory_status_ex);
        if (!::GlobalMemoryStatusEx(&memory_status_ex))
        {
            LOG_WARNING(cl7::platform::errors::system_result(::GetLastError(), u8"::GlobalMemoryStatusEx"));
            return {};
        }

        memory_status.total_physical_memory = memory_status_ex.ullTotalPhys;
        memory_status.available_physical_memory = memory_status_ex.ullAvailPhys;

        assert(memory_status.total_physical_memory >= memory_status.available_physical_memory);
        memory_status.used_physical_memory = memory_status.total_physical_memory - memory_status.available_physical_memory;

#if false
        memory_status.total_virtual_memory = memory_status_ex.ullTotalVirtual;
        memory_status.available_virtual_memory = memory_status_ex.ullAvailVirtual;

        assert(memory_status.total_virtual_memory >= memory_status.available_virtual_memory);
        memory_status.used_virtual_memory = memory_status.total_virtual_memory - memory_status.available_virtual_memory;

        memory_status.current_swap_space = memory_status_ex.ullTotalPageFile;
        memory_status.free_swap_space = memory_status_ex.ullAvailPageFile;

        assert(memory_status.total_virtual_memory >= memory_status.total_physical_memory);
        memory_status.maximum_swap_space = memory_status.total_virtual_memory - memory_status.total_physical_memory;
        assert(memory_status.maximum_swap_space >= memory_status.current_swap_space);
        memory_status.available_swap_space = memory_status.maximum_swap_space - memory_status.current_swap_space;
        assert(memory_status.current_swap_space >= memory_status.free_swap_space);
        memory_status.used_swap_space = memory_status.current_swap_space - memory_status.free_swap_space;
#endif

        return memory_status;
    }



} // namespace cl7::platform::detail::windows::memory

#endif // F77_IS_WINDOWS
