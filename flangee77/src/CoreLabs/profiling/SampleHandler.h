#pragma once
#ifndef CL7_PROFILING_SAMPLEHANDLER_H
#define CL7_PROFILING_SAMPLEHANDLER_H

#include "./Sample.h"
#include "./Stopwatch.h"



namespace cl7 {
namespace profiling {



struct SampleHandler
{

    /** The sample to be "handled". */
    Sample* sample;

    /** . */
    Stopwatch stopwatch;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * 
     */
    void open_sample();

    /**
     * 
     */
    void close_sample();

    /**
     * 
     */
    void update_sample(unsigned frame_usecs);

}; // struct SampleHandler



} // namespace profiling
} // namespace cl7

#endif // CL7_PROFILING_SAMPLEHANDLER_H
