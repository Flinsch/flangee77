#pragma once
#ifndef XL7_GRAPHICS_SHADERS_SHADERCODE_H
#define XL7_GRAPHICS_SHADERS_SHADERCODE_H

#include <CoreLabs/byte_view.h>
#include <CoreLabs/byte_vector.h>
#include <CoreLabs/string.h>



namespace xl7::graphics::shaders {



class ShaderCode
{

public:
    enum class Language
    {
        Unknown,
        Bytecode,
        //Assembly, // Not really possible anymore, therefore not supported.
        HighLevel,
    };



    ShaderCode() = default;
    ShaderCode(Language language, cl7::byte_view code_data);
    ShaderCode(cl7::u8string_view high_level_code);



    /**
     * Returns the language in which the code is given.
     */
    Language get_language() const { return _language; }

    /**
     * Returns the actual code data.
     */
    const cl7::byte_vector& get_code_data() const { return _code_data; }



private:
    /**
     * The language in which the code is given.
     */
    Language _language = Language::Unknown;

    /**
     * The actual code data.
     */
    cl7::byte_vector _code_data;

}; // class ShaderCode



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_SHADERCODE_H
