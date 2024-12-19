#pragma once
#ifndef XL7_GRAPHICS_MESHES_TOPOLOGY_H
#define XL7_GRAPHICS_MESHES_TOPOLOGY_H

#include <CoreLabs/root.h>



namespace xl7::graphics::meshes {



enum class Topology
{
    Undefined,
    PointList,
    LineList,
    LineStrip,
    TriangleList,
    TriangleStrip,
};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_TOPOLOGY_H
