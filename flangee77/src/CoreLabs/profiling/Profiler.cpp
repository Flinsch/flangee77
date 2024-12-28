#include "Profiler.h"

#include "./StandardRegistry.h"



namespace cl7::profiling {



    Profiler::Profiler(const cl7::u8string& sample_name)
        : Profiler(sample_name, &StandardRegistry::instance())
    {
    }

    Profiler::Profiler(const cl7::u8string& sample_name, Registry* registry)
        : _registry(registry)
        , _sample(Registry::Attorney::begin_sample(registry, sample_name))
    {
        assert(_registry);
        assert(_sample);
    }

    Profiler::~Profiler()
    {
        Registry::Attorney::end_sample(_registry, _sample);
    }



} // namespace cl7::profiling
