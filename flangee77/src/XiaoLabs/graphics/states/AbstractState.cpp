#include "AbstractState.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace states {



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool AbstractState::_check_data_impl(const resources::DataProvider& data_provider)
    {
        assert( data_provider.get_offset() == 0 && data_provider.get_size() == 0 );

        return true;
    }



} // namespace states
} // namespace graphics
} // namespace xl7
