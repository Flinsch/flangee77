#pragma once
#ifndef XL7_GRAPHICS_H
#define XL7_GRAPHICS_H

#include "./graphics/GraphicsSystem.h"
#include "./graphics/RenderingDevice.h"



namespace xl7 {



    graphics::GraphicsSystem& graphics_system() { return graphics::GraphicsSystem::instance(); }
    graphics::RenderingDevice* rendering_device() { return graphics_system().get_rendering_device(); }



} // namespace xl7

#endif // XL7_GRAPHICS_H
