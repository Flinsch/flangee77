#include "ConstantBuffer.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::shaders {



    /**
     * Updates the contents of this constant buffer.
     */
    bool ConstantBuffer::update(const ConstantDataProvider& constant_data_provider)
    {
        if (!_try_fill_data(constant_data_provider))
            return false;

        bool discard = true;
        bool no_overwrite = false;

        return _update_impl( constant_data_provider, discard, no_overwrite );
    }



    ConstantBuffer::ConstantBuffer(const CreateParams<Desc>& params)
        : Resource(params)
        , _desc(params.desc)
    {
    }



    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool ConstantBuffer::_check_data_impl(const resources::DataProvider& data_provider)
    {
        

        return true;
    }

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool ConstantBuffer::_acquire_impl(const resources::DataProvider& data_provider)
    {
        assert(typeid(data_provider) == typeid(const ConstantDataProvider&));
        auto constant_data_provider = static_cast<const ConstantDataProvider&>(data_provider); // NOLINT(*-pro-type-static-cast-downcast)

        return _acquire_impl(constant_data_provider);
    }



} // namespace xl7::graphics::shaders
