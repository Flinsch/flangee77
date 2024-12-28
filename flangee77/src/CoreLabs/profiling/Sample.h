#pragma once
#ifndef CL7_PROFILING_SAMPLE_H
#define CL7_PROFILING_SAMPLE_H

#include <CoreLabs/string.h>



namespace cl7::profiling {



struct Sample
{

    // NOLINTBEGIN(cppcoreguidelines-avoid-const-or-ref-data-members)
    /** The name of this sample. */
    const cl7::u8string name;
    /** The immediate parent of this sample. */
    Sample* const parent_sample;
    /** The number of parent samples. */
    const unsigned parent_count;
    // NOLINTEND(cppcoreguidelines-avoid-const-or-ref-data-members)

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
        /** The maximum percentage of frame time taken up. */
        float max_pct;
        /** The average percentage of frame time taken up. */
        float avg_pct;

        /** The number of times values have been stored since sample creation/reset. */
        unsigned data_count;
        /** The number of times this sample has been executed (profiled). */
        unsigned call_count;
    } stats;



    Sample(cl7::u8string_view name, Sample* parent_sample = nullptr)
        : name(name)
        , parent_sample(parent_sample)
        , parent_count(parent_sample ? parent_sample->parent_count + 1 : 0)
        , frame({.is_open=false, .is_profiled=false, .frame_usecs=0})
        , stats({.min_usecs=0, .max_usecs=0, .avg_usecs=0, .min_pct=0.0f, .max_pct=0.0f, .avg_pct=0.0f, .data_count=0, .call_count=0})
    {
    }

}; // struct Sample



} // namespace cl7::profiling

#endif // CL7_PROFILING_SAMPLE_H
