#ifndef XL7_GRAPHICS_STATES_RASTERIZERSTATE_H
#define XL7_GRAPHICS_STATES_RASTERIZERSTATE_H
#include "./AbstractState.h"

#include "./RasterizerStateDesc.h"



namespace xl7::graphics::states {



class RasterizerState
    : public resources::detail::ResourceBase<RasterizerState, AbstractState>
{

public:
    RasterizerState() = delete;

    RasterizerState(const RasterizerState&) = delete;
    RasterizerState& operator=(const RasterizerState&) = delete;
    RasterizerState(RasterizerState&&) = delete;
    RasterizerState& operator=(RasterizerState&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"rasterizer state"; }

    /**
     * Returns the descriptor of the rasterizer state.
     */
    const RasterizerStateDesc& get_desc() const { return _desc; }



protected:

    explicit RasterizerState(const CreateParams<RasterizerStateDesc>& params)
        : ResourceBase(params)
        , _desc(params.desc)
    {
    }

    ~RasterizerState() override = default;



private:
    /**
     * The descriptor of the rasterizer state.
     */
    const RasterizerStateDesc _desc;

}; // class RasterizerState



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_RASTERIZERSTATE_H
