#ifndef XL7_GRAPHICS_SHADERS_CONSTANTDATAPROVIDER_H
#define XL7_GRAPHICS_SHADERS_CONSTANTDATAPROVIDER_H
#include "../../resources/DefaultDataProvider.h"



namespace xl7::graphics::shaders {



class ConstantDataProvider
    : public resources::DefaultDataProvider
{

public:
    ConstantDataProvider() = default;

    template <class TData>
    explicit ConstantDataProvider(const TData* data)
         : resources::DefaultDataProvider(cl7::byte_view(reinterpret_cast<const std::byte*>(data), sizeof(TData)), 0)
    {
    }

    ~ConstantDataProvider() override = default;

}; // class ConstantDataProvider



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_CONSTANTDATAPROVIDER_H
