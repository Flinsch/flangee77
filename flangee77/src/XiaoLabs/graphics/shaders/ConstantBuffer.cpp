#include "ConstantBuffer.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace shaders {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    ConstantBuffer::ConstantBuffer(const CreateParams<Desc>& params)
        : Resource( params )
        , _desc( params.desc )
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool ConstantBuffer::_check_data_impl(const resources::DataProvider& data_provider)
    {
        

        return true;
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7
