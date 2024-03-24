#pragma once
#ifndef XL7_GRAPHICS_PIPELINE_INPUTASSEMBLERSTAGE_H
#define XL7_GRAPHICS_PIPELINE_INPUTASSEMBLERSTAGE_H
#include "./AbstractStage.h"

#include "../meshes/VertexBuffer.h"
#include "../meshes/IndexBuffer.h"



namespace xl7 {
namespace graphics {
namespace pipeline {



class InputAssemblerStage
    : public AbstractStage
{

public:
    /** The maximum number of concurrent vertex streams (ignoring hardware/driver capabilities). */
    static constexpr unsigned MAX_VERTEX_STREAMS = 8;

private:
    static constexpr unsigned DIRTY_FLAG_VERTEX_BUFFER_ID   = 0x1;
    static constexpr unsigned DIRTY_FLAG_INDEX_BUFFER_ID    = 0x2 << (MAX_VERTEX_STREAMS - 1);
    static constexpr unsigned DIRTY_FLAG_TOPOLOGY           = 0x4 << (MAX_VERTEX_STREAMS - 1);



    // #############################################################################
    // States
    // #############################################################################
private:
    _XL7_GRAPHICS_PIPELINE_STATE_ARRAY_DEFAULT0( vertex_buffer_id, MAX_VERTEX_STREAMS, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_FLAG_VERTEX_BUFFER_ID );
    _XL7_GRAPHICS_PIPELINE_SINGLE_STATE( index_buffer_id, resources::ResourceID, resources::ResourceID::INVALID_ID, DIRTY_FLAG_INDEX_BUFFER_ID );
    _XL7_GRAPHICS_PIPELINE_SINGLE_STATE( topology, meshes::Topology, meshes::Topology::Undefined, DIRTY_FLAG_TOPOLOGY );

    meshes::Topology get_topology(meshes::Topology default_topology = meshes::Topology::Undefined) const { return _topology.get_value( default_topology ); }

}; // class InputAssemblerStage



} // namespace pipeline
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_PIPELINE_INPUTASSEMBLERSTAGE_H
