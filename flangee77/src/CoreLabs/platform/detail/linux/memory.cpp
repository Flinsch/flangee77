#include "memory.h"
#if F77_IS_LINUX

#include <CoreLabs/logging.h>
#include <CoreLabs/platform/errors.h>

#include <sys/sysinfo.h>



namespace cl7::platform::detail::linux::memory {



    std::optional<cl7::platform::memory::MemoryStatus> get_memory_status()
    {
        cl7::platform::memory::MemoryStatus memory_status;

        struct sysinfo sys_info;
        std::memset(&sys_info, 0, sizeof(sys_info));
        if (::sysinfo(&sys_info) != 0)
        {
            LOG_WARNING(cl7::platform::errors::system_result(errno, u8"::sysinfo"));
            return {};
        }

        struct rlimit r_limit;
        std::memset(&r_limit, 0, sizeof(r_limit));
        if (::getrlimit(RLIMIT_AS, &r_limit) != 0)
        {
            LOG_WARNING(cl7::platform::errors::system_result(errno, u8"::getrlimit"));
            //return {};
        }

        memory_status.total_physical_memory = sys_info.totalram * sys_info.mem_unit;
        memory_status.available_physical_memory = sys_info.freeram * sys_info.mem_unit;

        assert(memory_status.total_physical_memory >= memory_status.available_physical_memory);
        memory_status.used_physical_memory = memory_status.total_physical_memory - memory_status.available_physical_memory;

#if false
        memory_status.current_swap_space = sys_info.totalswap * sys_info.mem_unit;
        memory_status.free_swap_space = sys_info.freeswap * sys_info.mem_unit;

        assert(memory_status.total_virtual_memory >= memory_status.total_physical_memory);
        memory_status.maximum_swap_space = memory_status.total_virtual_memory - memory_status.total_physical_memory;
        assert(memory_status.current_swap_space >= memory_status.free_swap_space);
        memory_status.used_swap_space = memory_status.current_swap_space - memory_status.free_swap_space;
        assert(memory_status.maximum_swap_space >= memory_status.current_swap_space);
        memory_status.available_swap_space = memory_status.maximum_swap_space - memory_status.current_swap_space;

        memory_status.total_virtual_memory = r_limit.rlim_cur;

        memory_status.used_virtual_memory = memory_status.used_physical_memory + memory_status.used_swap_space;
        assert(memory_status.total_virtual_memory >= memory_status.used_virtual_memory);
        memory_status.available_virtual_memory = memory_status.total_virtual_memory - memory_status.used_virtual_memory;
#endif

        return memory_status;
    }



} // namespace cl7::platform::detail::linux::memory

#endif // F77_IS_LINUX
