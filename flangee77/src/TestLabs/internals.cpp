#include "internals.h"

#include <CoreLabs/sstream.h>

#include <iomanip>



namespace tl7 {
namespace internals {



    cl7::string to_string(const std::nullptr_t&) { return {}; }

    template <class Tstring>
    static cl7::string _to_string(const Tstring& val)
    {
        cl7::osstream oss;
        oss << std::hex << std::setfill(TEXT('0'));

        oss << '"';
        for ( const auto c : val )
        {
            constexpr unsigned long long mask = (1Ui64 << (sizeof(decltype(c)) * 8)) - 1;
            const unsigned long long ull = static_cast<unsigned long long>( c ) & mask;

            if ( c < 0x20 )
                oss << "\\x" << std::setw(2) << ull;
            else if ( c < 0x7f )
                oss << cl7::char_type( c );
            else if ( c <= 0xff )
                oss << "\\x" << std::setw(2) << ull;
            else if ( c <= 0xffff )
                oss << "u+" << std::setw(4) << ull;
            else
                oss << "u+" << std::setw(8) << ull;
        }
        oss << '"';

        return oss.str();
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
