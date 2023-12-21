#pragma once
#ifndef XL7_GRAPHICS_STATES_STREAMSTATES_H
#define XL7_GRAPHICS_STATES_STREAMSTATES_H
#include "./AbstractStates.h"

#include "../meshes/VertexBuffer.h"
#include "../meshes/IndexBuffer.h"



namespace xl7 {
namespace graphics {
namespace states {



class StreamStates
    : public AbstractStates
{

public:
    /** The maximum number of concurrent vertex streams (ignoring hardware capabilities). */
    static constexpr unsigned MAX_VERTEX_STREAMS = 16;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    StateArray<meshes::VertexBuffer*, MAX_VERTEX_STREAMS> _vertex_buffers;
    SingleState<meshes::IndexBuffer*> _index_buffer;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    meshes::VertexBuffer* get_vertex_buffer() const { return get_vertex_buffer( 0 ); }
    meshes::VertexBuffer* get_vertex_buffer(unsigned index) const;

    meshes::IndexBuffer* get_index_buffer() const { return _index_buffer.get_value(); }

public:
    void set_vertex_buffer(meshes::VertexBuffer* vertex_buffer) { set_vertex_buffer( 0, vertex_buffer ); }
    void set_vertex_buffer(unsigned index, meshes::VertexBuffer* vertex_buffer);

    void set_index_buffer(meshes::IndexBuffer* index_buffer) { _index_buffer.set_value( index_buffer ); }

}; // class StreamStates



} // namespace states
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_STATES_STREAMSTATES_H
