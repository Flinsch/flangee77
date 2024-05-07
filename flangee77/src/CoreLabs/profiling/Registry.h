#pragma once
#ifndef CL7_PROFILING_REGISTRY_H
#define CL7_PROFILING_REGISTRY_H

#include "./Sample.h"
#include "./SampleHandler.h"

#include "../logging/Logger.h"

#include <vector>
#include <map>



namespace cl7 {
namespace profiling {



class Profiler;



class Registry
{

public:
    class Attorney
    {
        static Sample* begin_sample(Registry* registry, const cl7::astring& sample_name) { return registry->_begin_sample( sample_name ); }
        static void end_sample(Registry* registry, Sample* sample) { registry->_end_sample( sample ); }
        friend class Profiler;
    };

private:
    typedef std::pair<cl7::astring, Sample*> SampleKey;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    Registry() = default;

    /**
     * Destructor.
     */
    ~Registry() = default;

private:
    /** Copy constructor. */
    Registry(const Registry&) = delete;
    /** Copy assignment operator. */
    Registry& operator = (const Registry&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * A "linear list" of all registered samples.
     */
    std::vector<std::unique_ptr<Sample>> _samples;

    /**
     * A lookup map to find samples and their handlers.
     */
    std::map<SampleKey, SampleHandler> _sample_lookup;

    /**
     * The last opened sample (or NULL).
     */
    Sample* _last_opened_sample;

    /**
     * The number of currently open samples.
     */
    unsigned _open_sample_count;



    // #############################################################################
    // Methods
    // #############################################################################
private:
    /**
     * Begins and returns the sample with the specified name.
     */
    Sample* _begin_sample(const cl7::astring& sample_name);

    /**
     * Ends the specified sample.
     */
    void _end_sample(Sample* sample);

public:
    /**
     * "Closes" the current frame by updating the samples accordingly, implicitly
     * starting a new frame.
     */
    void end_frame_and_update_stats(unsigned frame_usecs);

    /**
     * "Dumps" the profiling/sample stats to the specified logger.
     */
    void dump(cl7::logging::Logger* logger) const;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * 
     */
    void _log(cl7::logging::Logger* logger, const Sample* sample, unsigned indent) const;

}; // class Registry



} // namespace profiling
} // namespace cl7

#endif // CL7_PROFILING_REGISTRY_H
