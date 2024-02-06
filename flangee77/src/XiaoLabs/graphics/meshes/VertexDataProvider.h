#pragma once
#ifndef XL7_GRAPHICS_MESHES_VERTEXDATAPROVIDER_H
#define XL7_GRAPHICS_MESHES_VERTEXDATAPROVIDER_H
#include "../../resources/DefaultDataProvider.h"



namespace xl7 {
namespace graphics {
namespace meshes {



template <class TVertex>
    requires( sizeof(TVertex) > 0 )
class VertexDataProvider
    : public resources::DefaultDataProvider
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Explicit constructor.
     */
    VertexDataProvider(std::span<const TVertex> data, size_t offset = 0)
         : resources::DefaultDataProvider( cl7::byte_view( reinterpret_cast<const std::byte*>( data.data() ), data.size_bytes() ), offset )
    {
    }

    /**
     * Destructor.
     */
    virtual ~VertexDataProvider() = default;

}; // class VertexDataProvider



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_VERTEXDATAPROVIDER_H
