#pragma once
#ifndef XL7_GRAPHICS_MESHES_INDEXDATAPROVIDER_H
#define XL7_GRAPHICS_MESHES_INDEXDATAPROVIDER_H
#include "../../resources/DefaultDataProvider.h"



namespace xl7 {
namespace graphics {
namespace meshes {



template <class TIndex>
    requires( sizeof(TIndex) == 2 || sizeof(TIndex) == 4 )
class IndexDataProvider
    : public resources::DefaultDataProvider
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Explicit constructor.
     */
    IndexDataProvider(std::span<const TIndex> data, size_t offset = 0)
         : resources::DefaultDataProvider( cl7::byte_view( reinterpret_cast<const std::byte*>( data.data() ), data.size_bytes() ), offset )
    {
    }

    /**
     * Destructor.
     */
    virtual ~IndexDataProvider() = default;

}; // class IndexDataProvider



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_INDEXDATAPROVIDER_H
