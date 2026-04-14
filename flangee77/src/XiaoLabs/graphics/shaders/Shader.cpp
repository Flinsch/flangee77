#include "Shader.h"

#include "./ConstantBuffer.h"

#include <CoreLabs/logging.h>

#include <algorithm>



namespace xl7::graphics::shaders {



    /**
     * (Re)compiles the shader code. This tends to result in the resource having to
     * be completely recreated in the background.
     */
    bool Shader::recompile(const CompileOptions& compile_options)
    {
        if (!is_recompilable())
        {
            assert(is_precompiled());
            LOG_ERROR(u8"The (precompiled) " + get_qualified_identifier() + u8" is not recompilable.");
            return false;
        }

        // The constant buffer mappings tend to have to be rebuilt.
        // Maybe not all of them, but we don't know for sure,
        // so we'll just delete them all "across the board".
        _constant_buffer_mappings_by_constant_buffer_id.clear();

        auto bytecode = _compile(compile_options);
        auto reflection_result = _reflect_and_validate(bytecode);

        if (!_on_recompile_impl(bytecode))
        {
            LOG_ERROR(u8"The " + get_qualified_identifier() + u8" could not be recompiled.");
            return false;
        }

        _bytecode = std::move(bytecode);
        _reflection_result = std::move(reflection_result);
        return true;
    }

    /**
     * Finds or creates the constant buffer mapping for the specified constant
     * buffer based on the reflection result of this shader. Returns NULL if not
     * applicable.
     * The padded sizes of the constant declarations specified in the descriptor of
     * the constant buffer should be set correctly to minimize the resulting mapping
     * entries as much as possible.
     */
    const ConstantBufferMapping* Shader::find_or_create_constant_buffer_mapping(const ConstantBuffer* constant_buffer) const
    {
        if (!constant_buffer)
            return nullptr;

        auto it = _constant_buffer_mappings_by_constant_buffer_id.find(constant_buffer->get_id());
        if (it != _constant_buffer_mappings_by_constant_buffer_id.end())
            return &it->second;

        auto tmp = _constant_buffer_mappings_by_constant_buffer_id.emplace(constant_buffer->get_id(), ConstantBufferMapping{});
        it = tmp.first;
        ConstantBufferMapping& constant_buffer_mapping = it->second;

        for (const auto& source_constant_declaration : constant_buffer->get_desc().layout.constant_declarations)
        {
            const auto p = find_constant_buffer_and_constant_declaration(source_constant_declaration.name);
            assert(p.first && p.second || !p.first && !p.second);
            if (!p.first /*|| !p.second*/) // Either both are NULL or both are not NULL, so one check is enough.
                continue;

            const ConstantBufferDeclaration& shader_constant_buffer_declaration = *p.first;
            const ConstantDeclaration& shader_constant_declaration = *p.second;

            constant_buffer_mapping.constant_mappings.push_back({
                .slot_index = shader_constant_buffer_declaration.index,
                .constant_type = shader_constant_declaration.constant_type,
                .source_offset = source_constant_declaration.offset,
                .shader_offset = shader_constant_declaration.offset,
                .size = (std::min)(source_constant_declaration.size, shader_constant_declaration.size),
                .padded_size = (std::min)(source_constant_declaration.padded_size, shader_constant_declaration.padded_size),
            });
        } // for each to-be-mapped "source" constant declaration

        // The following action is not actually necessary, but can improve the later
        // copying process of the data. We try to combine consecutive constants for
        // this. At best, we end up with only one single effective mapping entry.
        constant_buffer_mapping.try_merge_constant_mappings();

        return &constant_buffer_mapping;
    }

    /**
     * Searches for the specified constant buffer declaration and returns it if
     * found, NULL otherwise.
     */
    const ConstantBufferDeclaration* Shader::find_constant_buffer_declaration(cl7::astring_view constant_buffer_name) const
    {
        for (const auto& constant_buffer_declaration : _reflection_result.constant_buffer_declarations)
        {
            if (constant_buffer_declaration.name == constant_buffer_name)
                return &constant_buffer_declaration;
        }

        return nullptr;
    }

    /**
     * Searches for the specified constant declaration and returns it along with the
     * associated constant buffer declaration if found, NULL twice otherwise.
     */
    std::pair<const ConstantBufferDeclaration*, const ConstantDeclaration*> Shader::find_constant_buffer_and_constant_declaration(cl7::astring_view constant_name) const
    {
        for (const auto& constant_buffer_declaration : _reflection_result.constant_buffer_declarations)
        {
            for (const auto& constant_declaration : constant_buffer_declaration.layout.constant_declarations)
            {
                if (constant_declaration.name == constant_name)
                    return {&constant_buffer_declaration, &constant_declaration};
            }
        }

        return {nullptr, nullptr};
    }

    /**
     * Searches for the specified texture/sampler declaration and returns it if
     * found, NULL otherwise.
     */
    const TextureSamplerDeclaration* Shader::find_texture_sampler_declaration(cl7::astring_view texture_sampler_name) const
    {
        for (const auto& texture_sampler_declaration : _reflection_result.texture_sampler_declarations)
        {
            if (texture_sampler_declaration.name == texture_sampler_name)
                return &texture_sampler_declaration;
        }

        return nullptr;
    }



    Shader::Shader(const CreateContext& ctx, Type type, ShaderDesc desc)
        : ResourceWithData(ctx, 0)
        , _type(type)
        , _desc(std::move(desc))
    {
    }



    /**
     * Returns the effective name of the entry point for (re)compiling the shader.
     */
    cl7::astring Shader::_cascade_entry_point(const CompileOptions& compile_options) const
    {
        if (!_desc.entry_point.empty())
            return _desc.entry_point;

        switch (_type)
        {
        case Type::VertexShader:
            return {compile_options.default_vertex_entry_point};
        case Type::PixelShader:
            return {compile_options.default_pixel_entry_point};
        default:
            assert(false);
        }

        return {};
    }

    /**
     * Performs a "reflection" on the (compiled) shader bytecode to determine
     * parameter declarations etc. and validates the result.
     */
    ReflectionResult Shader::_reflect_and_validate(cl7::byte_view bytecode)
    {
        ReflectionResult reflection_result = _reflect_impl(bytecode);

        for (auto& constant_buffer_declaration : reflection_result.constant_buffer_declarations)
            constant_buffer_declaration.layout.sort_and_adjust_padded_sizes();

        if (!_validate_reflection_result(reflection_result))
            reflection_result.success = false;

        return reflection_result;
    }

    /**
     * Validates the given reflection result.
     */
    bool Shader::_validate_reflection_result(const ReflectionResult& reflection_result)
    {
        for (const auto& constant_buffer_declaration : reflection_result.constant_buffer_declarations)
        {
            const auto& constant_declarations = constant_buffer_declaration.layout.constant_declarations;

            for (const auto& constant_declaration : constant_declarations)
            {
                const bool is_scalar = constant_declaration.constant_class == ConstantClass::Scalar;
                //const bool is_vector = constant_declaration.constant_class == ConstantClass::Vector;
                const bool is_matrix = constant_declaration.constant_class == ConstantClass::MatrixRows || constant_declaration.constant_class == ConstantClass::MatrixColumns;

                assert((constant_declaration.row_count == 1 && !is_matrix) || (constant_declaration.row_count > 1 && is_matrix));
                assert((constant_declaration.column_count == 1 && is_scalar) || (constant_declaration.column_count > 1 && !is_scalar));
                assert(constant_declaration.element_count >= 1);

                assert(constant_declaration.size <= constant_declaration.padded_size);
            } // for each constant declaration
        } // for each constant buffer declaration

        for (const auto& texture_sampler_declaration : reflection_result.texture_sampler_declarations)
        {
        } // for each texture/sampler declaration

        return true;
    }



    /**
     * Takes the given shader code and compiles it if necessary, so that bytecode is
     * available for further processing.
     */
    void Shader::_build(const ShaderCode& shader_code, const CompileOptions& compile_options)
    {
        _access_data() = shader_code.get_code_data();

        switch (shader_code.get_language())
        {
        case ShaderCode::Language::Bytecode:
            _bytecode = shader_code.get_code_data();
            break;
        case ShaderCode::Language::HighLevel:
            _bytecode = _compile(compile_options);
            break;
        default:
            assert(false);
        }

        _reflection_result = _reflect_and_validate(_bytecode);
    }

    /**
     * Compiles the high-level shader code (taken from the shader's data) and
     * returns the resulting bytecode.
     */
    cl7::byte_vector Shader::_compile(const CompileOptions& compile_options)
    {
        auto bytecode = _compile_impl(get_data(), compile_options);
        if (bytecode.empty())
            LOG_ERROR(u8"The " + get_qualified_identifier() + u8" could not be compiled.");
        return std::move(bytecode);
    }



} // namespace xl7::graphics::shaders
