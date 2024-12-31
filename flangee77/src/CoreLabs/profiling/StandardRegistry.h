#ifndef CL7_PROFILING_STANDARDREGISTRY_H
#define CL7_PROFILING_STANDARDREGISTRY_H
#include <CoreLabs/creational/Singleton.h>

#include "./Registry.h"



namespace cl7::profiling {



class StandardRegistry final
    : public creational::Singleton<StandardRegistry>
{
    friend class creational::Singleton<StandardRegistry>;

public:
    StandardRegistry(const StandardRegistry&) = delete;
    StandardRegistry& operator=(const StandardRegistry&) = delete;
    StandardRegistry(StandardRegistry&&) = delete;
    StandardRegistry& operator=(StandardRegistry&&) = delete;

    static Registry& instance() { return creational::Singleton<StandardRegistry>::instance()._registry; }

    Registry* operator->() { return &_registry; }



private:
    StandardRegistry() = default;
    ~StandardRegistry() override = default;

    Registry _registry;

}; // class StandardRegistry final



} // namespace cl7::profiling

#endif // CL7_PROFILING_STANDARDREGISTRY_H
