#include "ShaderCode.h"

#include <CoreLabs/strings.h>



namespace xl7::graphics::shaders {



    ShaderCode::ShaderCode(Language language, cl7::byte_view code_data)
        : _language(language)
        , _code_data(code_data.begin(), code_data.end())
    {
    }

    ShaderCode::ShaderCode(cl7::u8string_view high_level_code)
        : _language(Language::HighLevel)
        , _code_data(cl7::strings::to_bytes(high_level_code))
    {
    }



} // namespace xl7::graphics::shaders
