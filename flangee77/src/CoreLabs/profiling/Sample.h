#pragma once
#ifndef CL7_PROFILING_SAMPLE_H
#define CL7_PROFILING_SAMPLE_H

#include <CoreLabs/string.h>



namespace cl7 {
namespace profiling {



struct Sample
{

    /** The name of this sample. */
    const cl7::astring name;
    /** The immediate parent of this sample. */
    Sample* const parent_sample;
    /** The number of parent samples. */
    const unsigned parent_count;

    struct
    {
        /** The flag that indicates whether this sample is "open", meaning it is currently being profiled. */
        bool is_open;
        /** The flag specifying whether this sample has been profiled during the current frame. */
        bool is_profiled;

        /** The total time this sample has been executed during the current frame, in microseconds. */
        unsigned frame_usecs;
    } frame;

    struct
    {
        /** The minimum frame time, in microseconds. */
        unsigned min_usecs;
        /** The maximum frame time, in microseconds. */
        unsigned max_usecs;
        /** The average frame time, in microseconds. */
        unsigned avg_usecs;

        /** The minimum percentage of frame time taken up. */
        float min_pct;
        /** The maximum percentage offrame time taken up. */
        float max_pct;
        /** The average percentage of frame time taken up. */
        float avg_pct;

        /** The number of times values have been stored since sample creation/reset. */
        unsigned data_count;
        /** The number of times this sample has been executed (profiled). */
        unsigned call_count;
    } stats;



    Sample(cl7::astring_view name, Sample* parent_sample = nullptr)
        : name( name )
        , parent_sample( parent_sample )
        , parent_count( parent_sample ? parent_sample->parent_count + 1 : 0 )
        , frame( { false, false, 0 } )
        , stats( { 0, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0 } )
    {
    }

}; // struct Sample



} // namespace profiling
} // namespace cl7

#endif // CL7_PROFILING_SAMPLE_H
