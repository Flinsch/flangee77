#include "Surface.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::surfaces {



    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool Surface::_check_data_impl(const resources::DataProvider& data_provider)
    {
        if (data_provider.get_offset() > 0 || data_provider.get_size() > 0)
        {
            assert(false);
            LOG_WARNING(TEXT("Providing data for requesting/acquiring a surface makes no sense."));
            //return false;
        }

        return true;
    }

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool Surface::_acquire_impl(const resources::DataProvider& data_provider)
    {
        

        assert(false);
        return false;
    }



} // namespace xl7::graphics::surfaces
