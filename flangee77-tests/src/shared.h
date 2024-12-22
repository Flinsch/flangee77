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
    cl7::string to_string(const cl7::byte_vector& bytes)
    {
        cl7::string s(bytes.size() * 4 + 2, TEXT(' '));
        s[0] = TEXT('{');
        for (size_t i = 0; i < bytes.size(); ++i)
        {
            const cl7::char_type byte = static_cast<cl7::char_type>(bytes[i]) & 0xff;
            const cl7::char_type lo = byte & 0xf;
            const cl7::char_type hi = (byte >> 4) & 0xf;
            s[i * 4 + 1] = TEXT('\\');
            s[i * 4 + 2] = TEXT('x');
            s[i * 4 + 3] = hi < 10 ? TEXT('0') + hi : TEXT('a') + hi - 10;
            s[i * 4 + 4] = lo < 10 ? TEXT('0') + lo : TEXT('a') + lo - 10;
        }
        s[bytes.size() * 4 + 1] = TEXT('}');
        return s;
    }
}



namespace ml7 {
    inline ml7::Vector2 round(ml7::Vector2 v, unsigned num_decimals) { for (float& k : v.data) k = ml7::round(k, num_decimals); return v; }
    inline ml7::Vector3 round(ml7::Vector3 v, unsigned num_decimals) { for (float& k : v.data) k = ml7::round(k, num_decimals); return v; }
    inline ml7::Vector4 round(ml7::Vector4 v, unsigned num_decimals) { for (float& k : v.data) k = ml7::round(k, num_decimals); return v; }
}



#endif // F77_TESTS_SHARED_H
