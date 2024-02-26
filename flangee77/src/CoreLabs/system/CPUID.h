#pragma once
#ifndef CL7_SYSTEM_CPUID_H
#define CL7_SYSTEM_CPUID_H

#include <CoreLabs/string.h>



namespace cl7 {
namespace system {



struct CPUID
{

    struct regs
    {
        unsigned eax;
        unsigned ebx;
        unsigned ecx;
        unsigned edx;
    };
    static_assert( sizeof(regs) == 16 );

    static regs cpuid(unsigned eax, unsigned ecx = 0);



    /** The vendor name. */
    cl7::achar_type vendor_name[ 12 + 1 ];
    /** The processor name. */
    cl7::achar_type processor_name[ 48 + 1 ];

    /** The bitness of the CPU. */
    unsigned bitness;
    /** The frequency of the master clock, in MHz. */
    unsigned frequency;

    /**
     * The number of concurrent threads supported by the implementation (or 0 if the
     * value is not well defined or not computable). The value should be considered
     * only a hint.
     */
    unsigned hardware_concurrency;



    /** Default constructor. */
    CPUID();



    /**
     * Retrieves information about the system's CPU.
     */
    bool capture();

}; // struct CPUID



} // namespace system
} // namespace cl7

#endif // CL7_SYSTEM_CPUID_H
