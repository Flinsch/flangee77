#pragma once
#ifndef XL7_GRAPHICS_H
#define XL7_GRAPHICS_H

#include "./graphics/GraphicsSystem.h"
#include "./graphics/RenderingDevice.h"



namespace xl7 {
namespace graphics {



    inline GraphicsSystem& graphics_system() { return GraphicsSystem::instance(); }
    inline RenderingDevice* rendering_device() { return graphics_system().get_rendering_device(); }
    inline surfaces::SurfaceManager* surface_manager() { return rendering_device()->get_surface_manager(); }
    inline textures::TextureManager* texture_manager() { return rendering_device()->get_texture_manager(); }
    inline meshes::MeshManager* mesh_manager() { return rendering_device()->get_mesh_manager(); }
    inline shaders::ShaderManager* shader_manager() { return rendering_device()->get_shader_manager(); }



} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_H
