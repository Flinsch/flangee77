#include "Profiler.h"

#include "./StandardRegistry.h"



namespace cl7 {
namespace profiling {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Profiler::Profiler(const cl7::astring& sample_name)
        : Profiler( sample_name, &StandardRegistry::instance() )
    {
    }

    /**
     * Explicit constructor.
     */
    Profiler::Profiler(const cl7::astring& sample_name, Registry* registry)
        : _registry( registry )
        , _sample( Registry::Attorney::begin_sample( registry, sample_name ) )
    {
        assert( _sample );
    }

    /**
     * Destructor.
     */
    Profiler::~Profiler()
    {
        Registry::Attorney::end_sample( _registry, _sample );
    }



} // namespace profiling
} // namespace cl7
