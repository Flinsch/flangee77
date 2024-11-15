#pragma once
#ifndef F77_TESTS_SHARED_H
#define F77_TESTS_SHARED_H

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/string.h>

#include <MathLabs/Vector2.h>
#include <MathLabs/Vector3.h>
#include <MathLabs/Vector4.h>



namespace cl7 {
    template <> inline
    cl7::string to_string(const byte_vector& bytes)
    {
        cl7::string s( bytes.size() * 4 + 2, TEXT(' ') );
        s[ 0 ] = TEXT('{');
        for ( size_t i = 0; i < bytes.size(); ++i )
        {
            const cl7::char_type byte = static_cast<cl7::char_type>( bytes[ i ] ) & 0xff;
            const cl7::char_type lo = byte & 0xf;
            const cl7::char_type hi = (byte >> 4) & 0xf;
            s[ i * 4 + 1 ] = TEXT('\\');
            s[ i * 4 + 2 ] = TEXT('x');
            s[ i * 4 + 3 ] = hi < 10 ? TEXT('0') + hi : TEXT('a') + hi - 10;
            s[ i * 4 + 4 ] = lo < 10 ? TEXT('0') + lo : TEXT('a') + lo - 10;
        }
        s[ bytes.size() * 4 + 1 ] = TEXT('}');
        return s;
    }
}



namespace _ {
    float round(float f, unsigned num_decimals) { return ml7::round(f, num_decimals); }

    ml7::Vector2 round(ml7::Vector2 v, unsigned num_decimals) { for ( unsigned k = 0; k < 2; ++k ) v.data[k] = ml7::round(v.data[k], num_decimals); return v; }
    ml7::Vector3 round(ml7::Vector3 v, unsigned num_decimals) { for ( unsigned k = 0; k < 3; ++k ) v.data[k] = ml7::round(v.data[k], num_decimals); return v; }
    ml7::Vector4 round(ml7::Vector4 v, unsigned num_decimals) { for ( unsigned k = 0; k < 4; ++k ) v.data[k] = ml7::round(v.data[k], num_decimals); return v; }
}



#endif // F77_TESTS_SHARED_H
