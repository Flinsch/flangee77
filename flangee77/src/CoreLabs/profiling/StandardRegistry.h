#pragma once
#ifndef CL7_PROFILING_STANDARDREGISTRY_H
#define CL7_PROFILING_STANDARDREGISTRY_H
#include <CoreLabs/creational/Singleton.h>

#include "./Registry.h"



namespace cl7 {
namespace profiling {



class StandardRegistry final
    : public creational::Singleton<StandardRegistry>
{
    friend class creational::Singleton<StandardRegistry>;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
private:
    /**
     * Default constructor.
     */
    StandardRegistry();

    /**
     * Destructor.
     */
    ~StandardRegistry() = default;

private:
    /** Copy constructor. */
    StandardRegistry(const StandardRegistry&) = delete;
    /** Copy assignment operator. */
    StandardRegistry& operator = (const StandardRegistry&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The actual registry.
     */
    Registry _registry;



    // #############################################################################
    // Dereferenciation
    // #############################################################################
public:
    /**
     * Returns the actual registry.
     */
    static Registry& instance() { return creational::Singleton<StandardRegistry>::instance()._registry; }

    /**
     * Returns the actual registry.
     */
    Registry* operator -> () { return &_registry; }

}; // class StandardRegistry final



} // namespace profiling
} // namespace cl7

#endif // CL7_PROFILING_STANDARDREGISTRY_H
