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
    /** The maximum number of concurrent vertex streams (ignoring hardware/driver capabilities). */
    static constexpr unsigned MAX_VERTEX_STREAMS = 8;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    StateArray<resources::ResourceID, MAX_VERTEX_STREAMS> _vertex_buffer_ids;
    SingleState<resources::ResourceID> _index_buffer_id;
    SingleState<meshes::Topology> _topology;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    resources::ResourceID get_vertex_buffer_id() const { return get_vertex_buffer_id( 0 ); }
    resources::ResourceID get_vertex_buffer_id(unsigned index) const;

    resources::ResourceID get_index_buffer_id() const { return _index_buffer_id.get_value(); }

    meshes::Topology get_topology(meshes::Topology default_topology = meshes::Topology::Unknown) const { return _topology.get_value( default_topology ); }

public:
    void set_vertex_buffer_id(resources::ResourceID vertex_buffer_id) { set_vertex_buffer_id( 0, vertex_buffer_id ); }
    void set_vertex_buffer_id(unsigned index, resources::ResourceID vertex_buffer_id);

    void set_index_buffer_id(resources::ResourceID index_buffer_id) { _index_buffer_id.set_value( index_buffer_id ); }

    void set_topology(meshes::Topology topology) { _topology.set_value( topology ); }
    void reset_topology() { _topology.reset_value(); }

}; // class StreamStates



} // namespace states
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_STATES_STREAMSTATES_H
