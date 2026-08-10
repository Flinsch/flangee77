#include "MyApp.h"

    #include <FaceLabs/fonts/detail/ttf/TrueTypeFontLoader.h>
    #include <FaceLabs/fonts/TextLayout.h>
    #include <FaceLabs/fonts/render/Markup.h>

    #include <FaceLabs/gui/faces/Button.h>
    #include <FaceLabs/gui/faces/Frame.h>
    #include <FaceLabs/gui/faces/Label.h>
    #include <FaceLabs/gui/faces/Panel.h>

#include <XiaoLabs/graphics.h>
    #include <XiaoLabs/graphics/images/codecs/targa/Reader.h>
    #include <XiaoLabs/graphics/images/codecs/png/Reader.h>
    #include <XiaoLabs/graphics/images/codecs/netpbm/Reader.h>
#include <XiaoLabs/graphics/images/ImageConverter.h>

#include <MathLabs/math.h>

#include <CoreLabs/platform/filesystem.h>
#include <CoreLabs/io/File.h>
#include <CoreLabs/io/Utf8Reader.h>

#include <CoreLabs/profiling.h>
#include <CoreLabs/logging.h>
#include <CoreLabs/text/codec.h>

#include <algorithm>
#include <filesystem>
#include <fstream>



namespace helloworld {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    MyApp::MyApp(int argc, wchar_t* argv[])
        : pl7::Application({argc, argv})
        , _glyph_raster_cache(cl7::platform::filesystem::get_cache_directory() + u8"flangee77/glyphcache/", u8"HelloWorld-")
    {
    }



    // #############################################################################
    // Application Implementations
    // #############################################################################

    /**
     * Initializes the actual application instance before initializing the framework
     * components. The passed configuration object might be manipulated to influence
     * the following initialization process accordingly. In the event of an error,
     * false is returned and the application exits without continuing initialization
     * let alone entering the main loop.
     */
    bool MyApp::_pre_init_impl(xl7::Config& config)
    {
        config.general.use_hardware_cursor = true;

        return true;
    }

    /**
     * Initializes the actual application instance after the framework components
     * have (successfully) been initialized. In the event of an error, false is
     * returned and the application exits without even entering the main loop.
     */
    bool MyApp::_post_init_impl()
    {
        struct Vertex
        {
            float x, y, z;
            unsigned color;
            float u, v;
        };

        xl7::graphics::meshes::VertexLayout vertex_layout;
        vertex_layout.elements.push_back({.semantic = xl7::graphics::meshes::VertexLayout::Semantic::POSITION,  .semantic_index = 0,  .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT3});
        vertex_layout.elements.push_back({.semantic = xl7::graphics::meshes::VertexLayout::Semantic::COLOR,     .semantic_index = 0,  .data_type = xl7::graphics::meshes::VertexLayout::DataType::COLOR});
        vertex_layout.elements.push_back({.semantic = xl7::graphics::meshes::VertexLayout::Semantic::TEXCOORD,  .semantic_index = 0,  .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT2});
        assert(vertex_layout.calculate_size() == sizeof(Vertex));

        xl7::graphics::meshes::VertexBufferDesc vertex_buffer_desc{
            .usage = xl7::graphics::meshes::MeshBufferUsage::Immutable,
            .topology = xl7::graphics::meshes::Topology::PointList,
            .vertex_count = 4,
            .vertex_stride = sizeof(Vertex),
            .vertex_layout = vertex_layout,
        };

        xl7::graphics::meshes::IndexBufferDesc index_buffer_desc{
            .usage = xl7::graphics::meshes::MeshBufferUsage::Immutable,
            .topology = xl7::graphics::meshes::Topology::TriangleStrip,
            .index_count = 4,
            .index_type = xl7::graphics::meshes::IndexType::UINT16,
        };

        // NOLINTBEGIN(*-use-designated-initializers)
        constexpr Vertex vertices[]{
            {-0.5f, -0.5f,  0.5f, 0xffff0000, -0.5, 1.5},
            {-0.5f, +0.5f,  0.5f, 0xff00ff00, -0.5, -0.5},
            {+0.5f, -0.5f,  0.5f, 0xff0000ff, 1.5, 1.5},
            {+0.5f, +0.5f,  0.5f, 0xffffff00, 1.5, -0.5},
        };
        // NOLINTEND(*-use-designated-initializers)
        constexpr unsigned short indices[]{0, 1, 2, 3};

        auto vertex_buffer_write = xl7::graphics::meshes::VertexBufferWrite::from_vertices<Vertex>(vertices);
        auto index_buffer_write = xl7::graphics::meshes::IndexBufferWrite::from_indices<unsigned short>(indices);

        _vertex_buffer_id = xl7::graphics::mesh_manager()->create_vertex_buffer(u8"My Vertex Buffer", vertex_buffer_desc, &vertex_buffer_write);
        _index_buffer_id = xl7::graphics::mesh_manager()->create_index_buffer(u8"My Index Buffer", index_buffer_desc, &index_buffer_write);


        xl7::graphics::shaders::ShaderDesc vertex_shader_desc{
            .language = xl7::graphics::shaders::ShaderCode::Language::HighLevel,
            .entry_point = "",
        };
        xl7::graphics::shaders::ShaderDesc pixel_shader_desc{
            .language = xl7::graphics::shaders::ShaderCode::Language::HighLevel,
            .entry_point = "",
        };

        cl7::io::File file(cl7::platform::filesystem::get_working_directory() + u8"assets/shaders/shader.hlsl");
        cl7::io::Utf8Reader utf8_reader(&file);
        cl7::u8string high_level_code = utf8_reader.read_all();
        xl7::graphics::shaders::ShaderCode shader_code{high_level_code};
        xl7::graphics::shaders::CompileOptions compile_options;
        compile_options.include_directories.push_back(file.get_path());
        xl7::graphics::shaders::ShaderWrite shader_write{.shader_code = &shader_code, .compile_options = &compile_options};

        _vertex_shader_id = xl7::graphics::shader_manager()->create_vertex_shader(u8"My Vertex Shader", vertex_shader_desc, shader_write);
        _pixel_shader_id = xl7::graphics::shader_manager()->create_pixel_shader(u8"My Pixel Shader", pixel_shader_desc, shader_write);


        xl7::graphics::shaders::ConstantBufferDesc constant_buffer_desc;
        // NOLINTBEGIN(*-use-designated-initializers)
        constant_buffer_desc.layout.constant_declarations = {
            {"VertexOffset", xl7::graphics::shaders::ConstantType::Float, xl7::graphics::shaders::ConstantClass::Vector, 1, 3, 1, 0, 12, 12},
            {"PixelBaseColor", xl7::graphics::shaders::ConstantType::Float, xl7::graphics::shaders::ConstantClass::Vector, 1, 4, 1, 12, 16, 16},
        };
        // NOLINTEND(*-use-designated-initializers)

        _constant_buffer_id = xl7::graphics::shader_manager()->create_constant_buffer(u8"My Constant Buffer", constant_buffer_desc);


        xl7::graphics::images::Image image;
        xl7::graphics::images::codecs::targa::Reader targa_image_reader;
        xl7::graphics::images::codecs::png::Reader png_image_reader;
        xl7::graphics::images::codecs::netpbm::Reader netpbm_image_reader;
        targa_image_reader.load_from_file(cl7::platform::filesystem::get_working_directory() + u8"assets/gfx/dummy.tga", image);
        //targa_image_reader.load_from_file(cl7::platform::filesystem::get_working_directory() + u8"assets/gfx/dummy-compressed.tga", image);
        //png_image_reader.load_from_file(cl7::platform::filesystem::get_working_directory() + u8"assets/gfx/dummy.png", image);
        //png_image_reader.load_from_file(cl7::platform::filesystem::get_working_directory() + u8"assets/gfx/dummy-indexed.png", image);
        //netpbm_image_reader.load_from_file(cl7::platform::filesystem::get_working_directory() + u8"assets/gfx/dummy-p1.pbm", image);
        //netpbm_image_reader.load_from_file(cl7::platform::filesystem::get_working_directory() + u8"assets/gfx/dummy-p2.pgm", image);
        //netpbm_image_reader.load_from_file(cl7::platform::filesystem::get_working_directory() + u8"assets/gfx/dummy-p3.ppm", image);
        //netpbm_image_reader.load_from_file(cl7::platform::filesystem::get_working_directory() + u8"assets/gfx/dummy-p4.pbm", image);
        //netpbm_image_reader.load_from_file(cl7::platform::filesystem::get_working_directory() + u8"assets/gfx/dummy-p5.pgm", image);
        //netpbm_image_reader.load_from_file(cl7::platform::filesystem::get_working_directory() + u8"assets/gfx/dummy-p6.ppm", image);
        //netpbm_image_reader.load_from_file(cl7::platform::filesystem::get_working_directory() + u8"assets/gfx/dummy-p7.pam", image);

        xl7::graphics::textures::Texture2DDesc texture_desc{
            .usage = xl7::graphics::textures::TextureUsage::Immutable,
            .pixel_format = xl7::graphics::PixelFormat::R8G8B8A8_UNORM,
            .preferred_channel_order = xl7::graphics::ChannelOrder::RGBA,
            .mip_levels = 0,
            .extent = {
                .width = image.get_width(),
                .height = image.get_height(),
            },
        };

        xl7::graphics::states::SamplerStateDesc sampler_desc;

        auto texture_image = xl7::graphics::images::ImageConverter::convert_image(image, texture_desc.pixel_format, texture_desc.preferred_channel_order);
        auto texture_write = xl7::graphics::textures::Texture2DWrite::from_image(texture_image);

        _texture_id = xl7::graphics::texture_manager()->create_texture_2d(u8"My Texture", texture_desc, &texture_write);
        _sampler_state_id = xl7::graphics::state_manager()->ensure_sampler_state(sampler_desc);

        // 9-slicing demo chrome: no dedicated chrome asset yet, so this just
        // reuses the dummy texture above (whole image as the sprite, insets a
        // quarter of its (smaller) extent) to exercise the slicing mechanism.
        _gui_chrome.texture_id = xl7::resources::id_cast<xl7::graphics::textures::Texture2D::Id>(_texture_id);
        _gui_chrome.sprite_size = {static_cast<float>(image.get_width()), static_cast<float>(image.get_height())};
        _gui_chrome.inset_left = _gui_chrome.inset_right = std::min(_gui_chrome.sprite_size.x, _gui_chrome.sprite_size.y) * 0.25f;
        _gui_chrome.inset_top = _gui_chrome.inset_bottom = _gui_chrome.inset_left;


        xl7::graphics::states::RasterizerStateDesc rasterizer_desc;
        rasterizer_desc.cull_mode = xl7::graphics::states::CullMode::None;

        _rasterizer_state_id = xl7::graphics::state_manager()->ensure_rasterizer_state(rasterizer_desc);


        xl7::graphics::states::DepthStencilStateDesc depth_stencil_desc;
        depth_stencil_desc.is_depth_testing_enabled = true;
        depth_stencil_desc.is_depth_writing_enabled = true;

        _depth_stencil_state_id = xl7::graphics::state_manager()->ensure_depth_stencil_state(depth_stencil_desc);


        xl7::graphics::states::BlendStateDesc blend_desc;
        blend_desc.is_blending_enabled = true;
        blend_desc.src_color_factor = blend_desc.src_alpha_factor = xl7::graphics::states::BlendFactor::SrcAlpha;
        blend_desc.dest_color_factor = blend_desc.dest_alpha_factor = xl7::graphics::states::BlendFactor::InvSrcAlpha;

        _blend_state_id = xl7::graphics::state_manager()->ensure_blend_state(blend_desc);



        for (size_t i = 0; i < xl7::graphics::shader_manager()->get_resource_count(); ++i)
        {
            const auto* resource = xl7::graphics::shader_manager()->get_resource(i);
            const auto* shader = dynamic_cast<const xl7::graphics::shaders::Shader*>(resource);
            if (!shader)
                continue;

            LOG_TYPE(u8"Parameters of " + shader->get_qualified_identifier() + u8':', cl7::logging::LogType::Caption);

            std::vector<const xl7::graphics::shaders::ConstantBufferDeclaration*> constant_buffer_declarations;
            for (const auto& constant_buffer_declaration : shader->get_reflection_result().constant_buffer_declarations)
                constant_buffer_declarations.push_back(&constant_buffer_declaration);
            std::ranges::sort(constant_buffer_declarations, [](const auto& a, const auto& b) {
                return a->index < b->index;
            });

            for (const auto* cb : constant_buffer_declarations)
            {
                if (!cb->name.empty())
                    LOG_TYPE(cl7::text::codec::to_utf8(cb->name) + u8"\tcb" + cl7::to_string(cb->index) + u8" (" + cl7::to_string(cb->layout.calculate_size()) + u8")", cl7::logging::LogType::Item);

                std::vector<const xl7::graphics::shaders::ConstantDeclaration*> constant_declarations;
                constant_declarations.reserve(cb->layout.constant_declarations.size());
                for (const auto& constant_declaration : cb->layout.constant_declarations)
                    constant_declarations.push_back(&constant_declaration);
                std::ranges::sort(constant_declarations, [](const auto& a, const auto& b) {
                    return a->offset < b->offset;
                });

                for (const auto* c : constant_declarations)
                {
                    assert(c->offset % 16 == 0);
                    if (cb->name.empty())
                        LOG_TYPE(cl7::text::codec::to_utf8(c->name) + u8"\tc" + cl7::to_string(c->offset / 16) + u8" (" + cl7::to_string((c->size + 15) / 16) + u8")", cl7::logging::LogType::Item);
                    else
                        LOG_TYPE(u8"." + cl7::text::codec::to_utf8(c->name) + u8"\tc" + cl7::to_string(c->offset / 16) + u8" (" + cl7::to_string((c->size + 15) / 16) + u8")", cl7::logging::LogType::Item);
                } // for each constant "variable"
            } // for each cbuffer

            std::vector<const xl7::graphics::shaders::TextureSamplerDeclaration*> sampler_declarations;
            for (const auto& texture_sampler_declaration : shader->get_reflection_result().texture_sampler_declarations)
            {
                if (texture_sampler_declaration.type == xl7::graphics::shaders::TextureSamplerDeclaration::Type::Sampler)
                    sampler_declarations.push_back(&texture_sampler_declaration);
            }
            std::ranges::sort(sampler_declarations, [](const auto& a, const auto& b) {
                return a->index < b->index;
            });

            for (const auto* s : sampler_declarations)
            {
                LOG_TYPE(cl7::text::codec::to_utf8(s->name) + u8"\ts" + cl7::to_string(s->index), cl7::logging::LogType::Item);
            } // for each sampler
        }


        auto font_loader = std::make_unique<fl7::fonts::detail::ttf::TrueTypeFontLoader>(cl7::platform::filesystem::get_working_directory() + u8"assets/fonts/Noto/NotoSans-Regular.ttf");
        //auto font_loader = std::make_unique<fl7::fonts::detail::ttf::TrueTypeFontLoader>(cl7::platform::filesystem::get_working_directory() + u8"assets/fonts/Noto/NotoSerif-Regular.ttf");
        _font = std::make_unique<fl7::fonts::Font>(std::move(font_loader));

        _sdf_rasterizer.set_cache(&_glyph_raster_cache);
        _msdf_rasterizer.set_cache(&_glyph_raster_cache);

        _bitmap_renderer = std::make_unique<fl7::fonts::render::BitmapRenderer>(&_bitmap_rasterizer);
        _sdf_renderer = std::make_unique<fl7::fonts::render::SdfRenderer>(&_sdf_rasterizer);
        _msdf_renderer = std::make_unique<fl7::fonts::render::MsdfRenderer>(&_msdf_rasterizer);
        _test_renderer = std::make_unique<fl7::fonts::render::TestRenderer>();

        _gui_renderer = std::make_unique<fl7::gui::render::DefaultRenderer>(_bitmap_renderer.get());
        _gui_shell = std::make_unique<fl7::gui::Shell>(_gui_renderer.get());

        _gui_theme.get_default_level().font = _font.get();
        _gui_theme.get_level(u8"window").chrome = &_gui_chrome;
        _gui_shell->set_theme(&_gui_theme);

        auto& frame = _gui_shell->add_face<fl7::gui::faces::Frame>(U"Demo Frame");
        frame.set_position({600.0f, 210.0f});
        frame.set_size({220.0f, 224.0f});

        auto& panel = frame.get_content_area().add_child<fl7::gui::faces::Panel>();
        panel.set_position({20.0f, 20.0f});
        panel.set_size({180.0f, 120.0f});
        fl7::gui::Style panel_style = panel.get_effective_style();
        panel_style.background_color = {0.4f, 0.4f, 0.5f, 1.0f};
        panel.set_style_override(panel_style);

        auto& label = panel.add_child<fl7::gui::faces::Label>(U"Hello, Panel!");
        label.set_position({10.0f, 10.0f});
        label.set_size({160.0f, 24.0f});

        auto& button = frame.get_content_area().add_child<fl7::gui::faces::Button>(U"Click me");
        button.set_position({20.0f, 150.0f});
        button.set_size({100.0f, 30.0f});
        button.get_clicked().connect([&button]() {
            LOG_INFO(u8"Button clicked!");
            button.set_text(button.get_text() == U"Click me" ? U"Clicked!" : U"Click me");
        });



        LOG_INFO(u8"Please note the following: The quick brown fox jumps over the lazy dog.");
        LOG_SUCCESS(u8"Great things have been done!");
        LOG_WARNING(u8"Brace yourselves! Winter is coming.");
        LOG_ERROR(u8"Something went terribly wrong!");

        LOG_TYPE(u8"Caption", cl7::logging::LogType::Caption);
        LOG_TYPE(u8"Section", cl7::logging::LogType::Section);
        LOG_TYPE(u8"Item A\tItem", cl7::logging::LogType::Item);
        LOG_TYPE(u8"Item B\tPass", cl7::logging::LogType::ItemPass);
        LOG_TYPE(u8"Item C\tFail", cl7::logging::LogType::ItemFail);

        LOG_TYPE(u8"print(\"Hello, World!\");", cl7::logging::LogType::Code);
        LOG_TYPE(u8"This is so meta", cl7::logging::LogType::Meta);
        LOG_TYPE(u8"The best comment is the one you don't have to write.", cl7::logging::LogType::Comment);
        LOG_TYPE(u8"This is something completely different.", cl7::logging::LogType::Other);

        return true;
    }

    /**
     * Shuts down the application instance before the framework components are also
     * shut down. If false is returned, the application will then be terminated in
     * any case, even if it should actually be restarted.
     */
    bool MyApp::_shutdown_impl()
    {
        _gui_shell.reset();
        _gui_renderer.reset();

        _bitmap_renderer.reset();
        _sdf_renderer.reset();
        _msdf_renderer.reset();
        _test_renderer.reset();

        _font.reset();


        xl7::graphics::state_manager()->release_resource_and_invalidate(_blend_state_id);
        xl7::graphics::state_manager()->release_resource_and_invalidate(_depth_stencil_state_id);
        xl7::graphics::state_manager()->release_resource_and_invalidate(_rasterizer_state_id);

        xl7::graphics::state_manager()->release_resource_and_invalidate(_sampler_state_id);
        xl7::graphics::texture_manager()->release_resource_and_invalidate(_texture_id);

        xl7::graphics::shader_manager()->release_resource_and_invalidate(_constant_buffer_id);

        xl7::graphics::shader_manager()->release_resource_and_invalidate(_vertex_shader_id);
        xl7::graphics::shader_manager()->release_resource_and_invalidate(_pixel_shader_id);

        xl7::graphics::mesh_manager()->release_resource_and_invalidate(_vertex_buffer_id);
        xl7::graphics::mesh_manager()->release_resource_and_invalidate(_index_buffer_id);

        return true;
    }

    /**
     * Performs the actual rendering.
     */
    void MyApp::_render_impl()
    {
        cl7::profiling::Profiler profiler(u8"MyApp::_render_impl");

        unsigned fps = xl7::graphics::graphics_system().get_config().video.refresh_rate;
        if (!fps)
            fps = 60;
        static float a = 0.0f;
        a += ml7::constantsf::pi2 / static_cast<float>(fps);
        float sn = std::sin(a) * 0.01f;
        float cs = std::cos(a) * 0.01f;

        struct ConstantBufferData
        {
            float x, y, z;
            xl7::graphics::Color color;
        } constant_buffer_data = {cs, sn, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f}}; // NOLINT(*-use-designated-initializers)

        auto constant_buffer_write = xl7::graphics::shaders::ConstantBufferWrite::from_data_ptr(&constant_buffer_data);

        auto* constant_buffer = xl7::graphics::shader_manager()->find_resource<xl7::graphics::shaders::ConstantBuffer>(_constant_buffer_id);
        assert(constant_buffer);
        constant_buffer->edit().write(constant_buffer_write);


        auto* rendering_context = xl7::graphics::primary_context();

        rendering_context->clear(xl7::graphics::ClearFlags::All, {1.0f, 0.333f, 0.75f}, 1.0f, 0x0);

        rendering_context->pipeline.ia.set_vertex_buffer_id(_vertex_buffer_id);
        rendering_context->pipeline.ia.set_index_buffer_id(_index_buffer_id);

        rendering_context->pipeline.vs.set_vertex_shader_id(_vertex_shader_id);
        rendering_context->pipeline.vs.set_constant_buffer_id(0, _constant_buffer_id);

        rendering_context->pipeline.ps.set_pixel_shader_id(_pixel_shader_id);
        rendering_context->pipeline.ps.set_constant_buffer_id(0, _constant_buffer_id);
        rendering_context->pipeline.ps.set_texture_id(0, _texture_id);
        rendering_context->pipeline.ps.set_sampler_state_id(0, _sampler_state_id);

        rendering_context->pipeline.rs.set_rasterizer_state_id(_rasterizer_state_id);

        rendering_context->pipeline.om.set_depth_stencil_state_id(_depth_stencil_state_id);
        rendering_context->pipeline.om.set_blend_state_id(_blend_state_id);

        rendering_context->draw_indexed();


        _gui_shell->draw();


        fl7::fonts::TextStyle text_style;
        text_style.font_size = 16.0f;
        text_style.scaling = {2.0f, 2.0f};
        text_style.background_padding = {2.0f, 0.0f};
        text_style.background_color = {0.1f, 0.2f, 0.4f, 0.7f};
        cl7::u8string text = u8"Hello, World!";
        text.reserve(text.size() + (0x7e - 0x20) + 1);
        for (cl7::u8char_t c = 0x20; c <= 0x7e; ++c)
            text.append(1, c);

        // Draw the same text with each renderer, stacked vertically, for comparison.
        const float line_height = text_style.font_size * text_style.scaling.y + 20.0f;
        float y = text_style.font_size * text_style.scaling.y + 10.0f;

        _test_renderer->draw_text(text, _font.get(), &text_style, {10.0f, y});
        y += line_height;
        _bitmap_renderer->draw_text(text, _font.get(), &text_style, {10.0f, y});
        y += line_height;
        _sdf_renderer->draw_text(text, _font.get(), &text_style, {10.0f, y});
        y += line_height;
        _msdf_renderer->draw_text(text, _font.get(), &text_style, {10.0f, y});
        y += line_height;

        // Point-based draw_text with alignment: `position` is treated as the
        // anchor point matching the given alignment (here: horizontally
        // centered on, and starting right below, the given point).
        fl7::fonts::TextStyle anchored_text_style = text_style;
        anchored_text_style.horizontal_align = fl7::fonts::TextStyle::HorizontalAlign::Center;
        anchored_text_style.vertical_align = fl7::fonts::TextStyle::VerticalAlign::Top;
        _bitmap_renderer->draw_text(
            u8"centered on this point",
            _font.get(), &anchored_text_style, {400.0f, y});
        y += line_height;

        // Box-based draw_text_in_box with word-wrapping.
        fl7::fonts::TextStyle box_text_style = text_style;
        box_text_style.wrap_mode = fl7::fonts::TextStyle::WrapMode::Word;
        box_text_style.horizontal_align = fl7::fonts::TextStyle::HorizontalAlign::Center;
        box_text_style.vertical_align = fl7::fonts::TextStyle::VerticalAlign::Top;
        _bitmap_renderer->draw_text_in_box(
            u8"This is a longer sentence that should wrap across multiple lines within the given box width.",
            _font.get(), &box_text_style, {10.0f, y}, {300.0f, 200.0f});
        y += 200.0f + line_height;

        // Cropping demo: WrapMode::None (so the long line isn't wrapped) and a
        // box height smaller than the line itself, so the rendered text is
        // clipped at the box edges on all four sides instead of spilling out.
        fl7::fonts::TextStyle crop_text_style = text_style;
        crop_text_style.wrap_mode = fl7::fonts::TextStyle::WrapMode::None;
        crop_text_style.vertical_align = fl7::fonts::TextStyle::VerticalAlign::Middle;
        _bitmap_renderer->draw_text_in_box(
            u8"This single unwrapped line is intentionally far too long and tall to fit, so it must be cropped at the box edges.",
            _font.get(), &crop_text_style, {10.0f, y}, {300.0f, 24.0f});
        y += 24.0f + line_height;

        // Justify demo: WrapMode::Word wraps this into several lines; every
        // line except the last should stretch flush to both box edges, while
        // the last line stays left-aligned.
        fl7::fonts::TextStyle justify_text_style = text_style;
        justify_text_style.wrap_mode = fl7::fonts::TextStyle::WrapMode::Word;
        justify_text_style.horizontal_align = fl7::fonts::TextStyle::HorizontalAlign::Justify;
        _bitmap_renderer->draw_text_in_box(
            u8"This text is justified, so every line except the last one should stretch flush to both edges of the box.",
            _font.get(), &justify_text_style, {10.0f, y}, {300.0f, 200.0f});
        y += 200.0f + line_height;

        // Letter/word spacing demo: extra spacing between characters and
        // between words, exercised both point-based and box-based (wrapped).
        fl7::fonts::TextStyle spacing_text_style = text_style;
        spacing_text_style.letter_spacing = 4.0f;
        spacing_text_style.word_spacing = 20.0f;
        _bitmap_renderer->draw_text(
            u8"Letter and word spacing demo",
            _font.get(), &spacing_text_style, {10.0f, y});
        y += line_height;

        spacing_text_style.wrap_mode = fl7::fonts::TextStyle::WrapMode::Word;
        _bitmap_renderer->draw_text_in_box(
            u8"This text has extra letter and word spacing, which should also shift its word-wrap boundaries.",
            _font.get(), &spacing_text_style, {10.0f, y}, {300.0f, 200.0f});
        y += 200.0f + line_height;

        // From here on, continue in a second column to the right: the first
        // column's demos already fill the window's full height on their own.
        // Positioned past the right edge of the first four (very wide,
        // full-ASCII) comparison lines, measured (rather than hardcoded) so
        // it doesn't matter that this column starts back at the very top.
        const float x2 = 10.0f + fl7::fonts::TextLayout::measure_width(text, *_font, text_style) + 40.0f;
        float y2 = text_style.font_size * text_style.scaling.y + 10.0f;

        // Style-run demo: "World" is colored, italicized, and bolded
        // differently than the rest of the text, via a StyleRun override on
        // top of the base TextStyle. Codepoint indices are plain string
        // offsets here since the text is pure ASCII. Bold is a pseudo-bold
        // SDF/MSDF edge bias, so it has no effect on the bitmap renderer.
        const cl7::u8string style_run_text = u8"Hello, World!";
        const fl7::fonts::render::StyleRun style_runs[] = {
            {.codepoint_begin = 7, .codepoint_end = 12, .style = {.weight = 1.0f, .italic_intensity = 1.0f, .text_color = xl7::graphics::Color::YELLOW}},
        };
        _bitmap_renderer->draw_text(style_run_text, _font.get(), &text_style, {x2, y2}, style_runs);
        y2 += line_height;
        _sdf_renderer->draw_text(style_run_text, _font.get(), &text_style, {x2, y2}, style_runs);
        y2 += line_height;
        _msdf_renderer->draw_text(style_run_text, _font.get(), &text_style, {x2, y2}, style_runs);
        y2 += line_height;

        // Whole-line italic demo: TextStyle::italic_intensity applies a
        // pseudo-italic geometric skew (not a true italic typeface).
        fl7::fonts::TextStyle italic_text_style = text_style;
        italic_text_style.italic_intensity = 1.0f;
        _bitmap_renderer->draw_text(u8"Italic text demo", _font.get(), &italic_text_style, {x2, y2});
        y2 += line_height;
        _sdf_renderer->draw_text(u8"Italic text demo", _font.get(), &italic_text_style, {x2, y2});
        y2 += line_height;
        _msdf_renderer->draw_text(u8"Italic text demo", _font.get(), &italic_text_style, {x2, y2});
        y2 += line_height;

        // Whole-line bold demo: TextStyle::weight biases the SDF/MSDF edge
        // threshold to thicken strokes. Bitmap has no distance field to bias,
        // so it's skipped here (weight is simply ignored there).
        fl7::fonts::TextStyle bold_text_style = text_style;
        bold_text_style.weight = 1.0f;
        _sdf_renderer->draw_text(u8"Bold text demo", _font.get(), &bold_text_style, {x2, y2});
        y2 += line_height;
        _msdf_renderer->draw_text(u8"Bold text demo", _font.get(), &bold_text_style, {x2, y2});
        y2 += line_height;

        // Whole-line outline demo: GlyphStyle::outline_color/outline_width add
        // a stroke around the glyph shape via an SDF/MSDF dual-threshold blend.
        // Bitmap has no distance field to blend against, so it's skipped here.
        fl7::fonts::TextStyle outline_text_style = text_style;
        outline_text_style.outline_color = xl7::graphics::Color::BLACK;
        outline_text_style.outline_width = 2.0f;
        _sdf_renderer->draw_text(u8"Outline text demo", _font.get(), &outline_text_style, {x2, y2});
        y2 += line_height;
        _msdf_renderer->draw_text(u8"Outline text demo", _font.get(), &outline_text_style, {x2, y2});
        y2 += line_height;

        // Icon demo: an inline icon (reusing the dummy texture already loaded
        // for the spinning quad above) placed via an IconRun at the '*'
        // placeholder's code point index. Icons keep their own texture colors
        // (not tinted like glyphs) and render identically across all three
        // atlas renderers, since icon rendering doesn't depend on the
        // rasterization technique used for text.
        const fl7::fonts::Icon icon{
            .texture_id = xl7::resources::id_cast<xl7::graphics::textures::Texture2D::Id>(_texture_id),
            .size = {24.0f, 24.0f},
            .offset = {2.0f, -20.0f},
        };
        const cl7::u8string icon_text = u8"Look: * an icon";
        const fl7::fonts::IconRun icon_runs[] = {
            {.codepoint_index = 6, .icon = &icon},
        };
        _bitmap_renderer->draw_text(icon_text, _font.get(), &text_style, {x2, y2}, {}, icon_runs);
        y2 += line_height;
        _sdf_renderer->draw_text(icon_text, _font.get(), &text_style, {x2, y2}, {}, icon_runs);
        y2 += line_height;
        _msdf_renderer->draw_text(icon_text, _font.get(), &text_style, {x2, y2}, {}, icon_runs);
        y2 += line_height;

        // Markup demo: the same bold/color/outline/icon capabilities as the
        // manual StyleRun/IconRun demos above, but authored as a single
        // marked-up string via fl7::fonts::render::parse_markup instead of
        // hand-computed code point indices. Reuses the same icon declared
        // above. [outline=...]/[outline-width=...] only have a visible effect
        // on SDF/MSDF, same as the standalone outline demo. [outline-width]
        // nests independently of [outline], here overriding just the width
        // of an outline whose color was set by the enclosing tag.
        const fl7::fonts::render::NamedIcon named_icons[] = {
            {.name = u8"gem", .icon = &icon},
        };
        const auto markup = fl7::fonts::render::parse_markup(
            u8"[b]Bold[/b] and [color=#ff0000]red[/color] and [icon=gem] and [b][color=#00ff00]both[/color][/b] and [outline=#000000]outlined[/outline] and [outline=#000000][outline-width=3]thick[/outline-width][/outline]!",
            text_style, named_icons);
        _bitmap_renderer->draw_text(markup.text, _font.get(), &text_style, {x2 - 178.0f, y2}, markup.style_runs, markup.icon_runs);
        y2 += line_height;
        _sdf_renderer->draw_text(markup.text, _font.get(), &text_style, {x2 - 178.0f, y2}, markup.style_runs, markup.icon_runs);
        y2 += line_height;
        _msdf_renderer->draw_text(markup.text, _font.get(), &text_style, {x2 - 178.0f, y2}, markup.style_runs, markup.icon_runs);
        y2 += line_height;
    }

    /**
     * Performs the application logic.
     */
    void MyApp::_move_impl()
    {
        cl7::profiling::Profiler profiler(u8"MyApp::_move_impl");

        _gui_shell->update();
    }



} // namespace helloworld
