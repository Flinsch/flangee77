#pragma once
#ifndef XL7_GRAPHICS_STATES_RASTERIZERSTATE_H
#define XL7_GRAPHICS_STATES_RASTERIZERSTATE_H
#include "./AbstractState.h"



namespace xl7 {
namespace graphics {
namespace states {



class RasterizerState
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

public:
    struct Desc
    {
        /** Determines the fill mode to use when rendering. */
        FillMode fill_mode  = FillMode::Solid;
        /** Indicates which triangles should be discarded (culled). */
        CullMode cull_mode  = CullMode::Back;
        /** Determines the orientation of the triangles. */
        WindingOrder winding_order  = WindingOrder::Clockwise;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    RasterizerState(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~RasterizerState() = default;

private:
    /** Default constructor. */
    RasterizerState() = delete;
    /** Copy constructor. */
    RasterizerState(const RasterizerState&) = delete;
    /** Copy assignment operator. */
    RasterizerState& operator = (const RasterizerState&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:
    /**
     * The descriptor of the sampler state.
     */
    const Desc _desc;

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the descriptor of the sampler state.
     */
    const Desc& get_desc() const { return _desc; }

public:



    // #############################################################################
    // Resource Implementations
    // #############################################################################
public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("rasterizer state"); }

}; // class RasterizerState



} // namespace states
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_STATES_RASTERIZERSTATE_H
