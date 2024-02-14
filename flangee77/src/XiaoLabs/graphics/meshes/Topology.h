#pragma once
#ifndef XL7_GRAPHICS_MESHES_TOPOLOGY_H
#define XL7_GRAPHICS_MESHES_TOPOLOGY_H





namespace xl7 {
namespace graphics {
namespace meshes {



enum class Topology
{
    Undefined,
    PointList,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip,
};



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_TOPOLOGY_H
