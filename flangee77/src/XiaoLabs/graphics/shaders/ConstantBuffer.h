#ifndef XL7_GRAPHICS_SHADERS_CONSTANTBUFFER_H
#define XL7_GRAPHICS_SHADERS_CONSTANTBUFFER_H
#include "../../resources/Resource.h"

#include "./ConstantBufferLayout.h"
#include "./ConstantDataProvider.h"



namespace xl7::graphics::shaders {



class ShaderManager;



class ConstantBuffer
    : public resources::detail::ResourceBase<ConstantBuffer>
{

public:
    struct Desc
    {
        /** The layout specification of the constant buffer. */
        ConstantBufferLayout layout;
    };



    ConstantBuffer() = delete;

    ConstantBuffer(const ConstantBuffer&) = delete;
    ConstantBuffer& operator=(const ConstantBuffer&) = delete;
    ConstantBuffer(ConstantBuffer&&) = delete;
    ConstantBuffer& operator=(ConstantBuffer&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"constant buffer"; }

    /**
     * Returns the descriptor of the constant buffer.
     */
    const Desc& get_desc() const { return _desc; }

    /**
     * Updates the contents of this constant buffer.
     */
    bool update(const ConstantDataProvider& constant_data_provider);



protected:

    ConstantBuffer(const CreateParams<Desc>& params)
        : ResourceBase(params)
        , _desc(params.desc)
    {
    }

    ~ConstantBuffer() override = default;



private:

    /**
     * Requests/acquires the constant buffer resource.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const ConstantDataProvider& constant_data_provider) = 0;

    /**
     * Updates the contents of this constant buffer.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _update_impl(const ConstantDataProvider& constant_data_provider, bool discard, bool no_overwrite) = 0;



    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool _check_data_impl(const resources::DataProvider& data_provider) override;

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool _acquire_impl(const resources::DataProvider& data_provider) override;



    /**
     * The descriptor of the constant buffer.
     */
    const Desc _desc;

}; // class ConstantBuffer



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_CONSTANTBUFFER_H
