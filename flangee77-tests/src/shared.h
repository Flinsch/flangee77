#ifndef F77_TESTS_SHARED_H
#define F77_TESTS_SHARED_H

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/string.h>



namespace tl7::internals {
    inline
    cl7::u8string to_string(std::byte byte)
    {
        cl7::u8string s(4, u8' ');
        const cl7::u8char_t lo = static_cast<cl7::u8char_t>(byte) & 0xf;
        const cl7::u8char_t hi = (static_cast<cl7::u8char_t>(byte) >> 4) & 0xf;
        s[0] = u8'\\';
        s[1] = u8'x';
        s[2] = hi < 10 ? u8'0' + hi : u8'a' + hi - 10;
        s[3] = lo < 10 ? u8'0' + lo : u8'a' + lo - 10;
        return s;
    }

    inline
    cl7::u8string to_string(const cl7::byte_vector& bytes)
    {
        cl7::u8string s(bytes.size() * 4 + 2, u8' ');
        s[0] = u8'{';
        for (size_t i = 0; i < bytes.size(); ++i)
        {
            const cl7::u8char_t byte = static_cast<cl7::u8char_t>(bytes[i]) & 0xff;
            const cl7::u8char_t lo = byte & 0xf;
            const cl7::u8char_t hi = (byte >> 4) & 0xf;
            s[i * 4 + 1] = u8'\\';
            s[i * 4 + 2] = u8'x';
            s[i * 4 + 3] = hi < 10 ? u8'0' + hi : u8'a' + hi - 10;
            s[i * 4 + 4] = lo < 10 ? u8'0' + lo : u8'a' + lo - 10;
        }
        s[bytes.size() * 4 + 1] = u8'}';
        return s;
    }
}



#endif // F77_TESTS_SHARED_H
