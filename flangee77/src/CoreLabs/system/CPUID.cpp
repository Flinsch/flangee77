#include "CPUID.h"

#ifdef _WIN32
#include <intrin.h>
#else // => Unix-like systems (such as Linux)
#endif

#include <thread>



namespace cl7::system {



    CPUID::CPUID()
    {
        std::memset(this, 0, sizeof(CPUID));
    }

    /**
     * Retrieves information about the system's CPU.
     */
    bool CPUID::capture()
    {
        *this = {}; // Reinitialize via default constructor.

        regs regs;

        regs = cpuid(0);
        std::memcpy(&vendor_name[0], &regs.ebx, 4);
        std::memcpy(&vendor_name[4], &regs.edx, 4);
        std::memcpy(&vendor_name[8], &regs.ecx, 4);

        regs = cpuid(0x80000000);

        if (regs.eax != 0x80000000) // is extended?
        {
            regs = cpuid(0x80000001);
            bitness = (regs.edx >> 29) & 0x1 ? 64 : 32;

            regs = cpuid(0x80000002);
            std::memcpy(&processor_name[ 0], &regs, 16);
            regs = cpuid(0x80000003);
            std::memcpy(&processor_name[16], &regs, 16);
            regs = cpuid(0x80000004);
            std::memcpy(&processor_name[32], &regs, 16);
        }
        else // => not extended
        {
            bitness = 0;
            std::memset(processor_name, 0, sizeof(processor_name));
        }

        // Extract frequency (in MHz) from the processor name.
        // Format is either "x.xxyHz" or "xxxxyHz", where y is M, G, or T and x is digits.
        // Start at index 4 to find y and stop at index -2 to prevent buffer overrun.
        for (unsigned i = 4; i < 48 - 2; ++i)
        {
            // Set multiplier so frequency is in MHz.
            unsigned multiplier = 0;
            if (processor_name[i + 1] == 'H' && processor_name[i + 2] == 'z')
            {
                if (processor_name[i] == 'M')
                    multiplier = 1;
                else if (processor_name[i] == 'G')
                    multiplier = 1'000;
                else if (processor_name[i] == 'T')
                    multiplier = 1'000'000;
            }

            // Not found. Maybe 'H' and 'z' were given just randomly/coincidentally.
            if (!multiplier)
                continue;

            // Found.
            if (processor_name[i-3] == '.')
            {
                frequency = static_cast<unsigned>(processor_name[i - 4] - '0') * multiplier;
                frequency += static_cast<unsigned>(processor_name[i - 2] - '0') * (multiplier / 10);
                frequency += static_cast<unsigned>(processor_name[i - 1] - '0') * (multiplier / 100);
            }
            else
            {
                frequency = static_cast<unsigned>(processor_name[i - 4] - '0') * 1000;
                frequency += static_cast<unsigned>(processor_name[i - 3] - '0') * 100;
                frequency += static_cast<unsigned>(processor_name[i - 2] - '0') * 10;
                frequency += static_cast<unsigned>(processor_name[i - 1] - '0');
            }

            // Done.
            break;
        } // for i ...

        hardware_concurrency = std::thread::hardware_concurrency();
        if (!hardware_concurrency)
        {
#ifdef _WIN32
            // Any ideas?
#else // => Unix-like systems (such as Linux)
            std::ifstream cpuinfo("/proc/cpuinfo");
            hardware_concurrency = std::count(std::istream_iterator<std::string>(cpuinfo), std::istream_iterator<std::string>(), std::string("processor"));
#endif
        }

        return true;
    }

    CPUID::regs CPUID::cpuid(unsigned eax, unsigned ecx)
    {
        regs regs = {.eax = 0, .ebx = 0, .ecx = 0, .edx = 0};

#ifdef _WIN32
        if (ecx)
            __cpuidex(reinterpret_cast<int*>(&regs), static_cast<int>(eax), static_cast<int>(ecx));
        else
            __cpuid(reinterpret_cast<int*>(&regs), static_cast<int>(eax));
#else // => Unix-like systems (such as Linux)
        if (ecx)
            __get_cpuid_count(eax, ecx, &regs.eax, &regs.ebx, &regs.ecx, &regs.edx);
        else
            __get_cpuid(eax, &regs.eax, &regs.ebx, &regs.ecx, &regs.edx);
#endif

        return regs;
    }



} // namespace cl7::system
