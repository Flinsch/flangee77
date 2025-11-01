#include "D3DShaderReflection.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::impl::direct3d9::shaders {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Performs a "reflection" on the (compiled) shader bytecode to determine
     * parameter declarations etc.
     */
    bool D3DShaderReflection::reflect(const xl7::graphics::shaders::ShaderCode& bytecode, xl7::graphics::shaders::ReflectionResult& reflection_result_out)
    {
        if (bytecode.get_language() != xl7::graphics::shaders::ShaderCode::Language::Bytecode)
        {
            LOG_ERROR(u8"The given code does not appear to be bytecode.");
            return false;
        }

        if (bytecode.get_code_data().empty())
        {
            LOG_ERROR(u8"The given bytecode is empty.");
            return false;
        }

        auto& constant_buffer_declarations_out = reflection_result_out.constant_buffer_declarations;
        auto& texture_sampler_declarations_out = reflection_result_out.texture_sampler_declarations;

        // Inspired by:
        // https://www.gamedev.net/forums/topic/648016-replacement-for-id3dxconstanttable/

        struct Header
        {
            uint32_t Size;
            uint32_t Creator;
            uint32_t Version;
            uint32_t Constants;
            uint32_t ConstantInfo;
            uint32_t Flags;
            uint32_t Target;
        };

        struct Info
        {
            uint32_t Name;
            uint16_t RegisterSet;
            uint16_t RegisterIndex;
            uint16_t RegisterCount;
            uint16_t Reserved;
            uint32_t TypeInfo;
            uint32_t DefaultValue;
        };

        struct Type
        {
            uint16_t Class;
            uint16_t Type;
            uint16_t Rows;
            uint16_t Columns;
            uint16_t Elements;
            uint16_t StructMembers;
            uint32_t StructMemberInfo;
        };

        constexpr uint32_t SIO_COMMENT = 0x0000fffe;
        constexpr uint32_t SIO_END = 0x0000ffff;
        constexpr uint32_t SI_OPCODE_MASK = 0x0000ffff;
        constexpr uint32_t SI_COMMENTSIZE_MASK = 0x7fff0000;
        constexpr uint32_t CTAB_CONSTANT = 0x42415443;

        assert(bytecode.get_code_data().size() % 4 == 0);
        const auto* const base_ptr = reinterpret_cast<const uint32_t*>(bytecode.get_code_data().data());
        for (size_t ofs = 0; ofs < bytecode.get_code_data().size(); ++ofs)
        {
            const uint32_t* const ptr = base_ptr + ofs;

            // Break if end of code.
            if (*ptr == SIO_END)
                break;

            // Skip if no comment.
            if ((*ptr & SI_OPCODE_MASK) != SIO_COMMENT)
                continue;

            // Skip comment if not constant/parameter table.
            const auto comment_size = static_cast<size_t>((*ptr & SI_COMMENTSIZE_MASK) >> 16);
            if (*(ptr + 1) != CTAB_CONSTANT)
            {
                ofs += comment_size;
                continue;
            }

            const auto* const ctab_ptr = reinterpret_cast<const char*>(ptr + 2);
            const auto ctab_size = static_cast<size_t>(comment_size - 1) * 4;

            const auto* const cheader = reinterpret_cast<const Header*>(ctab_ptr);
            if (ctab_size < sizeof(Header) || cheader->Size != sizeof(Header))
            {
                LOG_ERROR(u8"Bad shader constant/parameter table header size.");
                return false;
            }

            const auto* const cinfo = reinterpret_cast<const Info*>(ctab_ptr + cheader->ConstantInfo);

            constant_buffer_declarations_out.emplace_back(xl7::graphics::shaders::ConstantBufferDeclaration{.name = "", .index = 0, .layout = {}});
            auto& constant_declarations_out = constant_buffer_declarations_out.back().layout.constant_declarations;

            for (uint32_t i = 0; i < cheader->Constants; ++i)
            {
                const auto* const ctype = reinterpret_cast<const Type*>(ctab_ptr + cinfo[i].TypeInfo);

                xl7::graphics::shaders::ConstantType constant_type;
                switch (ctype->Type)
                {
                    case 1: // D3DXPT_BOOL
                    case 2: // D3DXPT_INT
                    case 3: // D3DXPT_FLOAT
                        static_assert(static_cast<unsigned>(xl7::graphics::shaders::ConstantType::Bool) == 1);
                        static_assert(static_cast<unsigned>(xl7::graphics::shaders::ConstantType::Int) == 2);
                        static_assert(static_cast<unsigned>(xl7::graphics::shaders::ConstantType::Float) == 3);
                        constant_type = static_cast<xl7::graphics::shaders::ConstantType>(ctype->Type);
                        break;
                    case 10: // D3DXPT_SAMPLER
                    case 11: // D3DXPT_SAMPLER1D
                    case 12: // D3DXPT_SAMPLER2D
                    case 13: // D3DXPT_SAMPLER3D
                    case 14: // D3DXPT_SAMPLERCUBE
                        continue;
                    default:
                        assert(false);
                        continue;
                } // switch parameter type

                xl7::graphics::shaders::ConstantClass constant_class;
                switch (ctype->Class)
                {
                    case 0: // D3DXPC_SCALAR
                    case 1: // D3DXPC_VECTOR
                    case 2: // D3DXPC_MATRIX_ROWS
                    case 3: // D3DXPC_MATRIX_COLUMNS
                        static_assert(static_cast<unsigned>(xl7::graphics::shaders::ConstantClass::Scalar) == 0);
                        static_assert(static_cast<unsigned>(xl7::graphics::shaders::ConstantClass::Vector) == 1);
                        static_assert(static_cast<unsigned>(xl7::graphics::shaders::ConstantClass::MatrixRows) == 2);
                        static_assert(static_cast<unsigned>(xl7::graphics::shaders::ConstantClass::MatrixColumns) == 3);
                        constant_class = static_cast<xl7::graphics::shaders::ConstantClass>(ctype->Class);
                        break;
                    default:
                        assert(false);
                        continue;
                } // switch parameter class

                xl7::graphics::shaders::ConstantDeclaration constant_declaration;
                constant_declaration.name = cl7::astring{ctab_ptr + cinfo[i].Name};
                constant_declaration.constant_type = constant_type;
                constant_declaration.constant_class = constant_class;
                constant_declaration.row_count = static_cast<unsigned>(ctype->Rows);
                constant_declaration.column_count = static_cast<unsigned>(ctype->Columns);
                constant_declaration.element_count = static_cast<unsigned>(ctype->Elements);
                constant_declaration.offset = static_cast<unsigned>(cinfo[i].RegisterIndex) * 16;
                constant_declaration.size = static_cast<unsigned>(cinfo[i].RegisterCount) * 16;

                constant_declarations_out.emplace_back(std::move(constant_declaration));
            } // for each "real" constant

            constant_buffer_declarations_out.back().layout.sort_and_adjust_padded_sizes();

            for (uint32_t i = 0; i < cheader->Constants; ++i)
            {
                const auto* const ctype = reinterpret_cast<const Type*>(ctab_ptr + cinfo[i].TypeInfo);

                switch (ctype->Type)
                {
                    case 1: // D3DXPT_BOOL
                    case 2: // D3DXPT_INT
                    case 3: // D3DXPT_FLOAT
                        continue;
                    case 10: // D3DXPT_SAMPLER
                    case 11: // D3DXPT_SAMPLER1D
                    case 12: // D3DXPT_SAMPLER2D
                    case 13: // D3DXPT_SAMPLER3D
                    case 14: // D3DXPT_SAMPLERCUBE
                        break;
                    default:
                        assert(false);
                } // switch parameter type

                xl7::graphics::shaders::TextureSamplerDeclaration texture_sampler_declaration;
                texture_sampler_declaration.name = cl7::astring{ctab_ptr + cinfo[i].Name};
                texture_sampler_declaration.index = static_cast<unsigned>(cinfo[i].RegisterIndex);
                texture_sampler_declaration.element_count = static_cast<unsigned>(ctype->Elements);

                texture_sampler_declarations_out.emplace_back(std::move(texture_sampler_declaration));
            } // for each texture/sampler "constant"

            return true;
        } // for ...

        // End of code reached without hitting comments sections.
        // This should be considered an error (or at least a warning).
        //LOG_ERROR(u8"The shader parameter table could not be created.");
        LOG_WARNING(u8"The shader parameter table could not be created.");
        return false;
    }



} // namespace xl7::graphics::impl::direct3d9::shaders
