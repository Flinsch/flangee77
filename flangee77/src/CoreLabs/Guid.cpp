#include "guid.h"

#ifdef _WIN32
#include <objbase.h>
#else
#error Not implemented.
#endif



namespace cl7 {


    Guid::Guid()
        : bytes({std::byte{0}})
    {
    }

    Guid::Guid(const std::array<std::byte, 16>& bytes)
        : bytes(bytes)
    {
    }

    Guid::Guid(cl7::u8string_view string)
        : bytes()
    {
        *this = parse(string);
    }

    void Guid::swap(Guid& other) noexcept
    {
        bytes.swap(other.bytes);
    }


    /**
     * Returns true if the GUID does not match 00000000-0000-0000-0000-000000000000.
     */
    bool Guid::is_valid() const
    {
        // Is this ... dirty?
        return bytes[0] != std::byte{0} || ::memcmp(bytes.data(), bytes.data() + 1, 15) != 0;
    }

    /**
     * "Stringifies" this GUID object.
     */
    cl7::u8string Guid::to_string(bool uppercase) const
    {
        cl7::u8string string(36, u8'-');

        for (size_t i = 0; i < 16; ++i)
        {
            size_t skip;
            if (i < 4)
                skip = 0;
            else if (i >= 10)
                skip = 4;
            else
                skip = (i - 2) / 2;

            auto x = static_cast<uint8_t>(bytes[i]);
            uint8_t x0 = (x & 0xf0) >> 4;
            uint8_t x1 = (x & 0x0f);
            auto ch0 = static_cast<cl7::u8char_type>((x0 >= 10 ? (uppercase ? u8'A' : u8'a') - 10 : u8'0') + x0);
            auto ch1 = static_cast<cl7::u8char_type>((x1 >= 10 ? (uppercase ? u8'A' : u8'a') - 10 : u8'0') + x1);
            string[i * 2 + skip + 0] = ch0;
            string[i * 2 + skip + 1] = ch1;
        }

        return string;
    }


    /**
     * Generates a new GUID.
     */
    Guid Guid::generate()
    {
#ifdef _WIN32
        ::GUID _guid;
        HRESULT hresult = ::CoCreateGuid(&_guid);
        assert(SUCCEEDED(hresult));

        std::array<std::byte, 16> bytes = {
            static_cast<std::byte>((_guid.Data1 >> 24) & 0xff),
            static_cast<std::byte>((_guid.Data1 >> 16) & 0xff),
            static_cast<std::byte>((_guid.Data1 >> 8) & 0xff),
            static_cast<std::byte>((_guid.Data1 >> 0) & 0xff),

            static_cast<std::byte>((_guid.Data2 >> 8) & 0xff),
            static_cast<std::byte>((_guid.Data2 >> 0) & 0xff),

            static_cast<std::byte>((_guid.Data3 >> 8) & 0xff),
            static_cast<std::byte>((_guid.Data3 >> 0) & 0xff),

            static_cast<std::byte>(_guid.Data4[0]),
            static_cast<std::byte>(_guid.Data4[1]),

            static_cast<std::byte>(_guid.Data4[2]),
            static_cast<std::byte>(_guid.Data4[3]),
            static_cast<std::byte>(_guid.Data4[4]),
            static_cast<std::byte>(_guid.Data4[5]),
            static_cast<std::byte>(_guid.Data4[6]),
            static_cast<std::byte>(_guid.Data4[7]),
        };

        return Guid(bytes);
#else
        static_assert(false, "Not implemented.");
#endif
    }

    /**
     * Parses the given GUID string.
     */
    Guid Guid::parse(cl7::u8string_view string)
    {
        std::array<std::byte, 16> bytes{{std::byte{0}}};

        if (string.length() != 36)
            return {};
        if (string[8+0] != u8'-' || string[12+1] != u8'-' || string[16+2] != u8'-' || string[20+3] != u8'-')
            return {};

        for (size_t i = 0; i < 16; ++i)
        {
            size_t skip;
            if (i < 4)
                skip = 0;
            else if (i >= 10)
                skip = 4;
            else
                skip = (i - 2) / 2;

            cl7::u8char_type ch0 = string[i * 2 + skip + 0];
            cl7::u8char_type ch1 = string[i * 2 + skip + 1];
            if ((ch0 < u8'0' || ch0 > u8'9') && ((ch0 | 32) < u8'a' || (ch0 | 32) > u8'f')) return {};
            if ((ch1 < u8'0' || ch1 > u8'9') && ((ch1 | 32) < u8'a' || (ch1 | 32) > u8'f')) return {};
            auto x0 = static_cast<uint8_t>(ch0 > u8'9' ? (ch0 | 32) - u8'a' + 10 : ch0 - u8'0');
            auto x1 = static_cast<uint8_t>(ch1 > u8'9' ? (ch1 | 32) - u8'a' + 10 : ch1 - u8'0');
            assert(x0 < 16);
            assert(x1 < 16);
            uint8_t x = (x0 << 4) | x1;
            bytes[i] = static_cast<std::byte>(x);
        }

        return Guid(bytes);
    }



} // namespace cl7
