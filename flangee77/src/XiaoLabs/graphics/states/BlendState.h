#ifndef XL7_GRAPHICS_STATES_BLENDSTATE_H
#define XL7_GRAPHICS_STATES_BLENDSTATE_H
#include "./AbstractState.h"

#include "./BlendStateDesc.h"



namespace xl7::graphics::states {



class BlendState
    : public resources::detail::ResourceBase<BlendState, AbstractState>
{

public:
    BlendState() = delete;

    BlendState(const BlendState&) = delete;
    BlendState& operator=(const BlendState&) = delete;
    BlendState(BlendState&&) = delete;
    BlendState& operator=(BlendState&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"blend state"; }

    /**
     * Returns the descriptor of the blend state.
     */
    const BlendStateDesc& get_desc() const { return _desc; }



protected:

    explicit BlendState(const CreateParams<BlendStateDesc>& params)
        : ResourceBase(params)
        , _desc(params.desc)
    {
    }

    ~BlendState() override = default;



private:
    /**
     * The descriptor of the blend state.
     */
    const BlendStateDesc _desc;

}; // class BlendState



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_BLENDSTATE_H
