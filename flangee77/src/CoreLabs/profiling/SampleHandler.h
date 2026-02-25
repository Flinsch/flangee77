#ifndef CL7_PROFILING_SAMPLEHANDLER_H
#define CL7_PROFILING_SAMPLEHANDLER_H

#include "./Sample.h"
#include "./Stopwatch.h"



namespace cl7::profiling {



struct SampleHandler
{

    Sample* sample;

    Stopwatch stopwatch{};



    void open_sample();
    void close_sample();
    void update_sample(unsigned frame_usecs);

}; // struct SampleHandler



} // namespace cl7::profiling

#endif // CL7_PROFILING_SAMPLEHANDLER_H
