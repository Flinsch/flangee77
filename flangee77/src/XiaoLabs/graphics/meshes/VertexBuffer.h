#pragma once
#ifndef XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
#define XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
#include "./MeshBuffer.h"

#include "./VertexLayout.h"
#include "./VertexDataProvider.h"



namespace xl7::graphics::meshes {



class VertexBuffer // NOLINT(*-virtual-class-destructor)
    : public MeshBuffer
{

public:
    struct Desc
        : public MeshBuffer::Desc
    {
        /** The size of each vertex, in bytes. */
        unsigned stride;
        /** The layout specification of the vertices. */
        VertexLayout vertex_layout;

        /** The number of instances to draw using the same per-instance data before advancing in the buffer by one element, or simply 0 if the buffer contains "regular" per-vertex data. */
        unsigned instance_data_step_rate;
    };



    VertexBuffer() = delete;

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&&) = delete;
    VertexBuffer& operator=(VertexBuffer&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"vertex buffer"; }

    /**
     * Returns the descriptor of the vertex buffer.
     */
    const Desc& get_desc() const { return _desc; }



protected:
    VertexBuffer(const CreateParams<Desc>& params);
    ~VertexBuffer() override = default;



private:
    /**
     * The descriptor of the vertex buffer.
     */
    const Desc _desc;

}; // class VertexBuffer



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
