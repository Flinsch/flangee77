#include "internals.h"



namespace tl7 {
namespace internals {



    template <class Tstring>
    cl7::string _to_string(const Tstring& val)
    {
        cl7::string s( val.size() + 2, TEXT('"') );

        for ( size_t i = 0; i < val.size(); ++i )
        {
            // We don't give a damn that there could
            // be "invalid" characters at this point.
            // But we have to consider negative values.
            // So we pass the bits "unchanged" instead
            // of "regular" casting.
            const auto c = val[ i ];
            constexpr unsigned long long mask0 = (1Ui64 << (sizeof(decltype(c)) * 8)) - 1;
            constexpr unsigned long long mask1 = (1Ui64 << (sizeof(cl7::char_type) * 8)) - 1;
            constexpr unsigned long long mask = mask0 & mask1;
            s[ i + 1 ] = c & mask;
        }

        return s;
    }

    cl7::string to_string(const std::string& val) { return _to_string( val ); }
    cl7::string to_string(const std::wstring& val) { return _to_string( val ); }
    cl7::string to_string(const std::u8string& val) { return _to_string( val ); }
    cl7::string to_string(const std::u16string& val) { return _to_string( val ); }
    cl7::string to_string(const std::u32string& val) { return _to_string( val ); }

    cl7::string to_string(const std::string_view& val) { return _to_string( val ); }
    cl7::string to_string(const std::wstring_view& val) { return _to_string( val ); }
    cl7::string to_string(const std::u8string_view& val) { return _to_string( val ); }
    cl7::string to_string(const std::u16string_view& val) { return _to_string( val ); }
    cl7::string to_string(const std::u32string_view& val) { return _to_string( val ); }



} // namespace internals
} // namespace tl7
