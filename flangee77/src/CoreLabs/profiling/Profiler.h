#pragma once
#ifndef CL7_PROFILING_PROFILER_H
#define CL7_PROFILING_PROFILER_H

#include "./Sample.h"



namespace cl7::profiling {



class Registry;



class Profiler
{

public:
    Profiler() = delete;

    Profiler(const cl7::u8string& sample_name);
    Profiler(const cl7::u8string& sample_name, Registry* registry);

    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;
    Profiler(Profiler&&) = delete;
    Profiler& operator=(Profiler&&) = delete;

    ~Profiler();



private:
    /** The registry that stores the sample. */
    Registry* const _registry;
    /** The sample "attended" by this profiler. */
    Sample* const _sample;

}; // class Profiler



} // namespace cl7::profiling

#endif // CL7_PROFILING_PROFILER_H
