#ifndef CL7_SYSTEM_MEMORYSTATUS_H
#define CL7_SYSTEM_MEMORYSTATUS_H

#include <CoreLabs/root.h>



namespace cl7::system {



struct MemoryStatus
{

    /** The total amount of usable physical memory, in bytes. */
    unsigned long long total_physical_memory;
    /** The amount of physical memory currently available, in bytes. This is the amount of physical memory that can be immediately reused without having to write its contents to disk first. */
    unsigned long long available_physical_memory;
    /** The amount of physical memory currently in use, in bytes. */
    unsigned long long used_physical_memory;

    // We'll leave the rest out for now. I couldn't find any reliable approaches to
    // determine the values. And actually nobody cares, right?
#if false
    /** The (potential) maximum size of the swap space, in bytes. */
    unsigned long long maximum_swap_space;
    /** The potentially additional size of the swap space available, in bytes. This is the potential maximum size of the swap space minus the currently committed size (used or unused). */
    unsigned long long available_swap_space;
    /** The currently committed size of the swap space (used or unused), in bytes. */
    unsigned long long current_swap_space;
    /** The current amount of committed but unused swap space. */
    unsigned long long free_swap_space;
    /** The amount of swap space currently in use, in bytes. */
    unsigned long long used_swap_space;

    /** The size of the virtual address space, in bytes. */
    unsigned long long total_virtual_memory;
    /** The amount of unreserved and uncommitted memory currently in the virtual address space, in bytes. */
    unsigned long long available_virtual_memory;
    /** The amount of virtual memory currently in use, in bytes. This is the sum of currently used physical memory and swap space. */
    unsigned long long used_virtual_memory;
#endif



    MemoryStatus();

    /**
     * Retrieves information about the system's current usage of both physical and
     * virtual memory.
     */
    bool capture();

}; // struct MemoryStatus



} // namespace cl7::system

#endif // CL7_SYSTEM_MEMORYSTATUS_H
