#include "internals.h"

#include <CoreLabs/sstream.h>

#include <iomanip>



namespace tl7::internals {



    cl7::u8string to_string(const std::nullptr_t& null) { return {}; }

    template <class Tstring_or_view>
    static cl7::u8string _to_string(const Tstring_or_view& val)
    {
        cl7::u8osstream oss;
        oss << std::hex << std::setfill('0');

        oss << '"';
        for (const auto c : val)
        {
            constexpr unsigned long long mask = (1ULL << (sizeof(decltype(c)) * 8)) - 1;
            const unsigned long long ull = static_cast<unsigned long long>(c) & mask;

            if (c < 0x20)
                oss << "\\x" << std::setw(2) << ull; // NOLINT(bugprone-branch-clone)
            else if (c < 0x7f)
                oss << static_cast<cl7::u8char_t>(c);
            else if (c <= 0xff)
                oss << "\\x" << std::setw(2) << ull;
            else if (c <= 0xffff)
                oss << "u+" << std::setw(4) << ull;
            else
                oss << "u+" << std::setw(8) << ull;
        }
        oss << '"';

        return oss.str();
    }

    cl7::u8string to_string(const std::string& val) { return _to_string(val); }
    cl7::u8string to_string(const std::wstring& val) { return _to_string(val); }
    cl7::u8string to_string(const std::u8string& val) { return _to_string(val); }
    cl7::u8string to_string(const std::u16string& val) { return _to_string(val); }
    cl7::u8string to_string(const std::u32string& val) { return _to_string(val); }

    cl7::u8string to_string(std::string_view val) { return _to_string(val); }
    cl7::u8string to_string(std::wstring_view val) { return _to_string(val); }
    cl7::u8string to_string(std::u8string_view val) { return _to_string(val); }
    cl7::u8string to_string(std::u16string_view val) { return _to_string(val); }
    cl7::u8string to_string(std::u32string_view val) { return _to_string(val); }

    cl7::u8string to_string(const char* val) { return _to_string(std::string_view(val)); }
    cl7::u8string to_string(const wchar_t* val) { return _to_string(std::wstring_view(val)); }
    cl7::u8string to_string(const char8_t* val) { return _to_string(std::u8string_view(val)); }
    cl7::u8string to_string(const char16_t* val) { return _to_string(std::u16string_view(val)); }
    cl7::u8string to_string(const char32_t* val) { return _to_string(std::u32string_view(val)); }



} // namespace tl7::internals
