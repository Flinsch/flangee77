#ifndef XL7_GRAPHICS_MESHES_INDEXDATAPROVIDER_H
#define XL7_GRAPHICS_MESHES_INDEXDATAPROVIDER_H
#include "../../resources/DefaultDataProvider.h"



namespace xl7::graphics::meshes {



template <class TIndex>
    requires(sizeof(TIndex) == 2 || sizeof(TIndex) == 4)
class IndexDataProvider
    : public resources::DefaultDataProvider
{

public:
    IndexDataProvider(std::span<const TIndex> data, size_t offset = 0)
        : resources::DefaultDataProvider(cl7::byte_view(reinterpret_cast<const std::byte*>(data.data()), data.size_bytes()), offset)
    {
    }

    ~IndexDataProvider() override = default;

}; // class IndexDataProvider



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_INDEXDATAPROVIDER_H
