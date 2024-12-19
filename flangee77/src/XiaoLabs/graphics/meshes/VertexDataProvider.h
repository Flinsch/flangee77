#pragma once
#ifndef XL7_GRAPHICS_MESHES_VERTEXDATAPROVIDER_H
#define XL7_GRAPHICS_MESHES_VERTEXDATAPROVIDER_H
#include "../../resources/DefaultDataProvider.h"



namespace xl7::graphics::meshes {



template <class TVertex>
    requires(sizeof(TVertex) >= 1)
class VertexDataProvider
    : public resources::DefaultDataProvider
{

public:
    VertexDataProvider(std::span<const TVertex> data, size_t offset = 0)
         : resources::DefaultDataProvider(cl7::byte_view(reinterpret_cast<const std::byte*>(data.data()), data.size_bytes()), offset)
    {
    }

    ~VertexDataProvider() override = default;

}; // class VertexDataProvider



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_VERTEXDATAPROVIDER_H
