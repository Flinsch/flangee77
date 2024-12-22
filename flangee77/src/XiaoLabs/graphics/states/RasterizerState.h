#pragma once
#ifndef XL7_GRAPHICS_STATES_RASTERIZERSTATE_H
#define XL7_GRAPHICS_STATES_RASTERIZERSTATE_H
#include "./AbstractState.h"



namespace xl7::graphics::states {



class RasterizerState // NOLINT(*-virtual-class-destructor)
    : public AbstractState
{

public:
    /** Determines the fill mode to use when rendering. */
    enum class FillMode
    {
        /** Do not draw anything at all. */
        None,
        /** Draw the vertices as points. */
        Point,
        /** Draw lines connecting the vertices. */
        Wireframe,
        /** Fill the triangles formed by the vertices. */
        Solid,
    };

    /** Indicates which triangles should be discarded (culled). */
    enum class CullMode
    {
        /** Always draw all triangles. */
        None,
        /** Do not draw triangles that are front-facing. */
        Front,
        /** Do not draw triangles that are back-facing. */
        Back,
    };

    /** Determines the orientation of the triangles. */
    enum class WindingOrder
    {
        /** Consider a triangle front-facing if its vertices are clockwise on the render target (consider back-facing if they are counter-clockwise). */
        Clockwise,
        /** Consider a triangle front-facing if its vertices are counter-clockwise on the render target (consider back-facing if they are clockwise). */
        CounterClockwise,
    };

    struct Desc
    {
        /** Determines the fill mode to use when rendering. */
        FillMode fill_mode  = FillMode::Solid;
        /** Indicates which triangles should be discarded (culled). */
        CullMode cull_mode  = CullMode::Back;
        /** Determines the orientation of the triangles. */
        WindingOrder winding_order  = WindingOrder::Clockwise;
    };



    RasterizerState() = delete;

    RasterizerState(const RasterizerState&) = delete;
    RasterizerState& operator=(const RasterizerState&) = delete;
    RasterizerState(RasterizerState&&) = delete;
    RasterizerState& operator=(RasterizerState&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::string_view get_type_string() const override { return TEXT("rasterizer state"); }

    /**
     * Returns the descriptor of the rasterizer state.
     */
    const Desc& get_desc() const { return _desc; }



protected:
    RasterizerState(const CreateParams<Desc>& params)
        : AbstractState(params)
        , _desc(params.desc)
    {
    }

    ~RasterizerState() override = default;



private:
    /**
     * The descriptor of the rasterizer state.
     */
    const Desc _desc;

}; // class RasterizerState



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_RASTERIZERSTATE_H
