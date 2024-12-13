#include "MemoryStatus.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/errors.h>

#ifdef _WIN32
#include <Windows.h>
#else // => Unix-like systems (such as Linux)
#include <sys/sysinfo.h>
#endif



namespace cl7::system {



    MemoryStatus::MemoryStatus()
    {
        ::memset(this, 0, sizeof(MemoryStatus));
    }

    /**
     * Retrieves information about the system's current usage of both physical and
     * virtual memory.
     */
    bool MemoryStatus::capture()
    {
        *this = {}; // Reinitialize via default constructor.

#ifdef _WIN32
        MEMORYSTATUSEX memory_status_ex;
        ::memset(&memory_status_ex, 0, sizeof(memory_status_ex));
        memory_status_ex.dwLength = sizeof(memory_status_ex);
        if (!::GlobalMemoryStatusEx(&memory_status_ex))
        {
            LOG_WARNING(cl7::errors::system_result(::GetLastError(), TEXT("::GlobalMemoryStatusEx")));
            return false;
        }

        total_physical_memory = memory_status_ex.ullTotalPhys;
        available_physical_memory = memory_status_ex.ullAvailPhys;

        assert(total_physical_memory >= available_physical_memory);
        used_physical_memory = total_physical_memory - available_physical_memory;

#if false
        total_virtual_memory = memory_status_ex.ullTotalVirtual;
        available_virtual_memory = memory_status_ex.ullAvailVirtual;

        assert(total_virtual_memory >= available_virtual_memory);
        used_virtual_memory = total_virtual_memory - available_virtual_memory;

        current_swap_space = memory_status_ex.ullTotalPageFile;
        free_swap_space = memory_status_ex.ullAvailPageFile;

        assert(total_virtual_memory >= total_physical_memory);
        maximum_swap_space = total_virtual_memory - total_physical_memory;
        assert(maximum_swap_space >= current_swap_space);
        available_swap_space = maximum_swap_space - current_swap_space;
        assert(current_swap_space >= free_swap_space);
        used_swap_space = current_swap_space - free_swap_space;
#endif
#else // => Unix-like systems (such as Linux)
        struct sysinfo sys_info;
        ::memset(&sys_info, 0, sizeof(sys_info));
        if (::sysinfo(&sys_info) != 0)
        {
            LOG_WARNING(cl7::errors::system_result(errno, TEXT("::sysinfo")));
            return false;
        }

        struct rlimit r_limit;
        ::memset(&r_limit, 0, sizeof(r_limit));
        if (::getrlimit(RLIMIT_AS, &r_limit) != 0)
        {
            LOG_WARNING(cl7::errors::system_result(errno, TEXT("::getrlimit")));
            //return false;
        }

        total_physical_memory = sys_info.totalram * sys_info.mem_unit;
        available_physical_memory = sys_info.freeram * sys_info.mem_unit;

        assert(total_physical_memory >= available_physical_memory);
        used_physical_memory = total_physical_memory - available_physical_memory;

#if false
        current_swap_space = sys_info.totalswap * sys_info.mem_unit;
        free_swap_space = memory_status_exsys_info.freeswap * sys_info.mem_unit;

        assert(total_virtual_memory >= total_physical_memory);
        maximum_swap_space = total_virtual_memory - total_physical_memory;
        assert(current_swap_space >= free_swap_space);
        used_swap_space = current_swap_space - free_swap_space;
        assert(maximum_swap_space >= current_swap_space);
        available_swap_space = maximum_swap_space - current_swap_space;

        total_virtual_memory = r_limit.rlim_cur;

        used_virtual_memory = used_physical_memory + used_swap_space;
        assert(total_virtual_memory >= used_virtual_memory);
        available_virtual_memory = total_virtual_memory - used_virtual_memory;
#endif
#endif

        return true;
    }



} // namespace cl7::system
