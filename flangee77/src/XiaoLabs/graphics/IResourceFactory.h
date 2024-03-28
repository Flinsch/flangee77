#pragma once
#ifndef XL7_GRAPHICS_IRESOURCEFACTORY_H
#define XL7_GRAPHICS_IRESOURCEFACTORY_H
#include "./surfaces/ISurfaceFactory.h"
#include "./textures/ITextureFactory.h"
#include "./meshes/IMeshFactory.h"
#include "./shaders/IShaderFactory.h"
#include "./states/IStateFactory.h"



namespace xl7 {
namespace graphics {



class IResourceFactory
    : public surfaces::ISurfaceFactory
    , public textures::ITextureFactory
    , public meshes::IMeshFactory
    , public shaders::IShaderFactory
    , public states::IStateFactory
{
public:
    virtual ~IResourceFactory() = default;

}; // class IResourceFactory



} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IRESOURCEFACTORY_H
