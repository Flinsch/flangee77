#include "BooleanMatcher.h"

#include <regex>



namespace dl7::ini::detail {



    size_t BooleanMatcher::operator()(cl7::u8string_view source) const
    {
        if (source.empty())
            return 0;

        const char* const data = reinterpret_cast<const char*>(source.data());

        std::cmatch m;

        if (mode != Mode::FalsyOnly && std::regex_match(data, data + source.length(), m, std::regex(R"(^(y|Y|yes|Yes|YES|true|True|TRUE|on|On|ON)(?:\W|$))")))
        {
            assert(m.position() == 0);
            return m.length();
        }

        if (mode != Mode::TruthyOnly && std::regex_match(data, data + source.length(), m, std::regex(R"(^(n|N|no|No|NO|false|False|FALSE|off|Off|OFF)(?:\W|$))")))
        {
            assert(m.position() == 0);
            return m.length();
        }

        return 0;
    }



} // namespace dl7::ini::detail
