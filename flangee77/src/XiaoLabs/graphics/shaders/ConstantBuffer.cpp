#include "ConstantBuffer.h"

#include "../GraphicsSystem.h"
#include "../RenderingDevice.h"

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

        return _update_impl(constant_data_provider, discard, no_overwrite);
    }



    ConstantBuffer::ConstantBuffer(const CreateContext& ctx, const ConstantBufferDesc& desc)
        : ResourceBase(ctx)
        , _desc(desc)
    {
        const size_t data_size = desc.layout.calculate_size();
        assert(data_size > 0);

        const RenderingDevice::Capabilities& capabilities = GraphicsSystem::instance().get_rendering_device()->get_capabilities();

        if (capabilities.buffers.max_constant_buffer_size && data_size > capabilities.buffers.max_constant_buffer_size)
            LOG_WARNING(u8"The " + get_qualified_identifier() + u8" to be created has a size of " + cl7::to_string(data_size) + u8" bytes, but a maximum of " + cl7::to_string(capabilities.buffers.max_constant_buffer_size) + u8" bytes is supported.");
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
