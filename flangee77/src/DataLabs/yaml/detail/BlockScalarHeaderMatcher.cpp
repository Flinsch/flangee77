#include "BlockScalarHeaderMatcher.h"



namespace dl7::yaml::detail {



    size_t BlockScalarHeaderMatcher::operator()(cl7::u8string_view source) const
    {
        if (source.empty() || (source[0] != u8'|' && source[0] != u8'>'))
            return 0;

        size_t i = 1;

        bool indentation_indicator = false;
        bool chomping_indicator = false;

        for (; i < source.size(); ++i)
        {
            const auto ch = source[i];

            if (ch >= u8'1' && ch <= u8'9')
            {
                if (indentation_indicator)
                    break;
                indentation_indicator = true;
            }
            else if (ch == u8'+' || ch == u8'-')
            {
                if (chomping_indicator)
                    break;
                chomping_indicator = true;
            }
            else
            {
                break;
            }
        }

        return i;
    }



} // namespace dl7::yaml::detail
