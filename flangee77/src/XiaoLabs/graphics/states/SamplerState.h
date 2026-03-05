#ifndef XL7_GRAPHICS_STATES_SAMPLERSTATE_H
#define XL7_GRAPHICS_STATES_SAMPLERSTATE_H
#include "./AbstractState.h"

#include "./SamplerStateDesc.h"



namespace xl7::graphics::states {



class SamplerState
    : public resources::detail::ResourceBase<SamplerState, AbstractState>
{

public:
    SamplerState() = delete;

    SamplerState(const SamplerState&) = delete;
    SamplerState& operator=(const SamplerState&) = delete;
    SamplerState(SamplerState&&) = delete;
    SamplerState& operator=(SamplerState&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"sampler state"; }

    /**
     * Returns the descriptor of the sampler state.
     */
    const SamplerStateDesc& get_desc() const { return _desc; }



protected:

    explicit SamplerState(const CreateParams<SamplerStateDesc>& params)
        : ResourceBase(params)
        , _desc(params.desc)
    {
    }

    ~SamplerState() override = default;



private:
    /**
     * The descriptor of the sampler state.
     */
    const SamplerStateDesc _desc;

}; // class SamplerState



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_SAMPLERSTATE_H
