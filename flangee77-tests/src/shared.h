#pragma once
#ifndef F77_TESTS_SHARED_H
#define F77_TESTS_SHARED_H

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/string.h>

#include <MathLabs/Vector2.h>
#include <MathLabs/Vector3.h>
#include <MathLabs/Vector4.h>



namespace tl7::internals {
    inline
    cl7::u8string to_string(const cl7::byte_vector& bytes)
    {
        cl7::u8string s(bytes.size() * 4 + 2, u8' ');
        s[0] = u8'{';
        for (size_t i = 0; i < bytes.size(); ++i)
        {
            const cl7::u8char_type byte = static_cast<cl7::u8char_type>(bytes[i]) & 0xff;
            const cl7::u8char_type lo = byte & 0xf;
            const cl7::u8char_type hi = (byte >> 4) & 0xf;
            s[i * 4 + 1] = u8'\\';
            s[i * 4 + 2] = u8'x';
            s[i * 4 + 3] = hi < 10 ? u8'0' + hi : u8'a' + hi - 10;
            s[i * 4 + 4] = lo < 10 ? u8'0' + lo : u8'a' + lo - 10;
        }
        s[bytes.size() * 4 + 1] = u8'}';
        return s;
    }
}



namespace ml7 {
    inline ml7::Vector2 round(ml7::Vector2 v, unsigned num_decimals) { for (float& k : v.data) k = ml7::round(k, num_decimals); return v; }
    inline ml7::Vector3 round(ml7::Vector3 v, unsigned num_decimals) { for (float& k : v.data) k = ml7::round(k, num_decimals); return v; }
    inline ml7::Vector4 round(ml7::Vector4 v, unsigned num_decimals) { for (float& k : v.data) k = ml7::round(k, num_decimals); return v; }
}



#endif // F77_TESTS_SHARED_H
