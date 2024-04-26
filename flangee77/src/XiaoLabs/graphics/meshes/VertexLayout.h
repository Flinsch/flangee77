#pragma once
#ifndef XL7_GRAPHICS_MESHES_VERTEXLAYOUT_H
#define XL7_GRAPHICS_MESHES_VERTEXLAYOUT_H

#include <CoreLabs/root.h>

#include <vector>



namespace xl7 {
namespace graphics {
namespace meshes {



struct VertexLayout
{

    enum class Semantic
    {
        /** Vertex position in object space. */
        POSITION,
        /** Transformed vertex position. */
        POSITIONT,
        /** Normal vector. */
        NORMAL,
        /** Tangent vector. */
        TANGENT,
        /** Binormal vector. */
        BINORMAL,
        /** Diffuse/specular color etc. */
        COLOR,
        /** Texture coordinates. */
        TEXCOORD,
        /** Blending indices. */
        BLENDINDICES,
        /** Blending weight. */
        BLENDWEIGHT,
        /** Point size. */
        PSIZE,
    };

    enum class DataType
    {
        /** One-component float expanded to (value, 0, 0, 1). */
        FLOAT1,
        /** Two-component float expanded to (value, value, 0, 1). */
        FLOAT2,
        /** Three-component float expanded to (value, value, value, 1). */
        FLOAT3,
        /** Four-component float expanded to (value, value, value, value). */
        FLOAT4,
        /** Four-component unsigned byte mapped to 0 to 1 range. */
        COLOR,
        /** Four-component unsigned byte. */
        UBYTE4,
    };

    struct Element
    {
        /** The semantic associated with this element. */
        Semantic semantic;
        /** The 0-based semantic index for the element (only needed in case where there is more than one element with the same semantic; default: 0). */
        unsigned semantic_index;
        /** The data type of the element data. */
        DataType data_type;



        /** Returns the size of the given data type, in bytes. */
        static unsigned get_size(DataType data_type);
        /** Returns the data size of this element, in bytes. */
        unsigned get_size() const;
    };

    /** The "ordered list" of vertex elements. */
    std::vector<Element> elements;



    /** Calculates the total size of the vertex layout, in bytes. */
    unsigned calculate_size() const;

}; // struct VertexLayout



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_VERTEXLAYOUT_H
