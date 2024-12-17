#pragma once
#ifndef XL7_GRAPHICS_H
#define XL7_GRAPHICS_H

#include "./graphics/GraphicsSystem.h"
#include "./graphics/RenderingDevice.h"
#include "./graphics/RenderingContext.h"



namespace xl7::graphics {



    inline GraphicsSystem& graphics_system() { return GraphicsSystem::instance(); }
    inline RenderingDevice* rendering_device() { return graphics_system().get_rendering_device(); }
    inline RenderingContext* primary_context() { return rendering_device()->get_primary_context(); }
    inline RenderingContext* rendering_context(unsigned index = 0) { return rendering_device()->get_rendering_context(index); }

    inline surfaces::SurfaceManager* surface_manager() { return rendering_device()->get_surface_manager(); }
    inline textures::TextureManager* texture_manager() { return rendering_device()->get_texture_manager(); }
    inline meshes::MeshManager* mesh_manager() { return rendering_device()->get_mesh_manager(); }
    inline shaders::ShaderManager* shader_manager() { return rendering_device()->get_shader_manager(); }
    inline states::StateManager* state_manager() { return rendering_device()->get_state_manager(); }



} // namespace xl7::graphics

#endif // XL7_GRAPHICS_H
