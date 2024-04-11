#pragma once
#ifndef XL7_GRAPHICS_SHADERS_CONSTANTDATAPROVIDER_H
#define XL7_GRAPHICS_SHADERS_CONSTANTDATAPROVIDER_H
#include "../../resources/DefaultDataProvider.h"



namespace xl7 {
namespace graphics {
namespace shaders {



class ConstantDataProvider
    : public resources::DefaultDataProvider
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Explicit constructor.
     */
    template <class TData>
    ConstantDataProvider(const TData* data)
         : resources::DefaultDataProvider( cl7::byte_view( reinterpret_cast<const std::byte*>( data ), sizeof(TData) ), 0 )
    {
    }

    /**
     * Destructor.
     */
    virtual ~ConstantDataProvider() = default;

}; // class ConstantDataProvider



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_CONSTANTDATAPROVIDER_H
