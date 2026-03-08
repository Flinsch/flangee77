#ifndef XL7_GRAPHICS_STATES_DEPTHSTENCILSTATE_H
#define XL7_GRAPHICS_STATES_DEPTHSTENCILSTATE_H
#include "./AbstractState.h"

#include "./DepthStencilStateDesc.h"



namespace xl7::graphics::states {



class DepthStencilState
    : public resources::detail::ResourceBase<DepthStencilState, AbstractState>
{

public:
    DepthStencilState() = delete;

    DepthStencilState(const DepthStencilState&) = delete;
    DepthStencilState& operator=(const DepthStencilState&) = delete;
    DepthStencilState(DepthStencilState&&) = delete;
    DepthStencilState& operator=(DepthStencilState&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"depth/stencil state"; }

    /**
     * Returns the descriptor of the depth/stencil state.
     */
    const DepthStencilStateDesc& get_desc() const { return _desc; }



protected:

    explicit DepthStencilState(const CreateContext& ctx, const DepthStencilStateDesc& desc)
        : ResourceBase(ctx, desc)
        , _desc(desc)
    {
    }

    ~DepthStencilState() override = default;



private:
    /**
     * The descriptor of the depth/stencil state.
     */
    const DepthStencilStateDesc _desc;

}; // class DepthStencilState



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_DEPTHSTENCILSTATE_H
