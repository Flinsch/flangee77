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
        if (!_check_is_usable())
            return false;

        if (!is_recompilable())
        {
            assert(is_precompiled());
            LOG_ERROR(u8"The (precompiled) " + get_typed_identifier_string() + u8" is not recompilable.");
            return false;
        }

        // The constant buffer mappings tend to have to be rebuilt.
        // Maybe not all of them, but we don't know for sure,
        // so we'll just delete them all "across the board".
        _constant_buffer_mappings_by_constant_buffer_id.clear();

        if (!_recompile_impl(compile_options, _bytecode))
        {
            LOG_ERROR(u8"The " + get_typed_identifier_string() + u8" could not be recompiled.");
            return false;
        }

        return _reflect_and_validate(_bytecode, _reflection_result);
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

        auto p = _constant_buffer_mappings_by_constant_buffer_id.emplace(constant_buffer->get_id(), ConstantBufferMapping{});
        it = p.first;
        ConstantBufferMapping& constant_buffer_mapping = it->second;

        for (const auto& source_constant_declaration : constant_buffer->get_desc().layout.constant_declarations)
        {
            const auto p = find_constant_buffer_and_constant_declaration(source_constant_declaration.name);
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



    /**
     * Explicit constructor.
     */
    Shader::Shader(Type type, const CreateParams<Desc>& params)
        : Resource(params)
        , _type(type)
        , _desc(params.desc)
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
    bool Shader::_reflect_and_validate(const ShaderCode& bytecode, ReflectionResult& reflection_result_out)
    {
        if (!_reflect_impl(bytecode, reflection_result_out))
            return false;

        for (auto& constant_buffer_declaration : reflection_result_out.constant_buffer_declarations)
            constant_buffer_declaration.layout.sort_and_adjust_padded_sizes();

        return _validate_reflection_result(reflection_result_out);
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
                const bool is_scalar = constant_declaration.constant_class == xl7::graphics::shaders::ConstantClass::Scalar;
                //const bool is_vector = constant_declaration.constant_class == xl7::graphics::shaders::ConstantClass::Vector;
                const bool is_matrix = constant_declaration.constant_class == xl7::graphics::shaders::ConstantClass::MatrixRows || constant_declaration.constant_class == xl7::graphics::shaders::ConstantClass::MatrixColumns;

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
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool Shader::_check_data_impl(const resources::DataProvider& data_provider)
    {
        assert(typeid(data_provider) == typeid(const CodeDataProvider&));
        auto code_data_provider = static_cast<const CodeDataProvider&>(data_provider); // NOLINT(*-pro-type-static-cast-downcast)

        if (code_data_provider.get_language() == ShaderCode::Language::Unknown)
        {
            LOG_ERROR(u8"The language of the provided code for " + get_typed_identifier_string() + u8" is unknown.");
            return false;
        }

        if (is_precompiled() && code_data_provider.get_language() != ShaderCode::Language::Bytecode)
        {
            LOG_ERROR(u8"The provided code for the precompiled " + get_typed_identifier_string() + u8" has to be in bytecode.");
            return false;
        }

        if (is_recompilable() && code_data_provider.get_language() != ShaderCode::Language::HighLevel)
        {
            LOG_ERROR(u8"The provided code for the recompilable " + get_typed_identifier_string() + u8" has to be in high-level language.");
            return false;
        }

        if (code_data_provider.get_code_data().empty())
        {
            LOG_ERROR(u8"The provided code for " + get_typed_identifier_string() + u8" is empty.");
            return false;
        }

        if (is_recompilable() && _cascade_entry_point(code_data_provider.get_compile_options()).empty())
        {
            LOG_ERROR(u8"The entry point for the recompilable " + get_typed_identifier_string() + u8" was not specified.");
            return false;
        }

        return true;
    }

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool Shader::_acquire_impl(const resources::DataProvider& data_provider)
    {
        assert(typeid(data_provider) == typeid(const CodeDataProvider&));
        auto code_data_provider = static_cast<const CodeDataProvider&>(data_provider); // NOLINT(*-pro-type-static-cast-downcast)

        if (is_precompiled())
        {
            assert(code_data_provider.get_shader_code().get_language() == ShaderCode::Language::Bytecode);
            _bytecode = code_data_provider.get_shader_code();

            return _acquire_precompiled_impl(code_data_provider) &&
                _reflect_and_validate(_bytecode, _reflection_result);
        }
        if (is_recompilable())
        {
            return _acquire_recompilable_impl(code_data_provider, _bytecode) &&
                _reflect_and_validate(_bytecode, _reflection_result);
        }

        assert(false);
        return false;
    }



} // namespace xl7::graphics::shaders
