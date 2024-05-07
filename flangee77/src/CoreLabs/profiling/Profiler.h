#pragma once
#ifndef CL7_PROFILING_PROFILER_H
#define CL7_PROFILING_PROFILER_H

#include "./Sample.h"



namespace cl7 {
namespace profiling {



class Registry;



class Profiler
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Explicit constructor.
     */
    Profiler(const cl7::astring& sample_name);

    /**
     * Explicit constructor.
     */
    Profiler(const cl7::astring& sample_name, Registry* registry);

    /**
     * Destructor.
     */
    ~Profiler();

private:
    /** Default constructor. */
    Profiler() = delete;
    /** Copy constructor. */
    Profiler(const Profiler&) = delete;
    /** Copy assignment operator. */
    Profiler& operator = (const Profiler&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The registry that stores the sample.
     */
    Registry* const _registry;

    /**
     * The sample "attended" by this profiler.
     */
    Sample* const _sample;

}; // class Profiler



} // namespace profiling
} // namespace cl7

#endif // CL7_PROFILING_PROFILER_H
