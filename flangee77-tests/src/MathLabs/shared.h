#ifndef F77_TESTS_ML7_SHARED_H
#define F77_TESTS_ML7_SHARED_H

#include "../shared.h"

#include <CoreLabs/string.h>

#include <MathLabs/Vector2.h>
#include <MathLabs/Vector3.h>
#include <MathLabs/Vector4.h>

#include <MathLabs/Matrix2x2.h>
#include <MathLabs/Matrix2x3.h>
#include <MathLabs/Matrix3x3.h>
#include <MathLabs/Matrix3x4.h>
#include <MathLabs/Matrix4x4.h>



namespace tl7::internals {
    inline
    cl7::u8string to_string(const ml7::Vector2& v) { return u8"{ " + cl7::to_string(v.x) + u8", " + cl7::to_string(v.y) + u8" }"; }
    inline
    cl7::u8string to_string(const ml7::Vector3& v) { return u8"{ " + cl7::to_string(v.x) + u8", " + cl7::to_string(v.y) + u8", " + cl7::to_string(v.z) + u8" }"; }
    inline
    cl7::u8string to_string(const ml7::Vector4& v) { return u8"{ " + cl7::to_string(v.x) + u8", " + cl7::to_string(v.y) + u8", " + cl7::to_string(v.z) + u8", " + cl7::to_string(v.w) + u8" }"; }

    inline
    cl7::u8string to_string(const ml7::Matrix2x2& m) { return u8"{ " + cl7::to_string(m.a) + u8", " + cl7::to_string(m.b) + u8"; " + cl7::to_string(m.c) + u8", " + cl7::to_string(m.d) + u8" }"; }
    inline
    cl7::u8string to_string(const ml7::Matrix2x3& m) { return u8"{ " + cl7::to_string(m.a) + u8", " + cl7::to_string(m.b) + u8", " + cl7::to_string(m.c) + u8"; " + cl7::to_string(m.d) + u8", " + cl7::to_string(m.e) + u8", " + cl7::to_string(m.f) + u8" }"; }
    inline
    cl7::u8string to_string(const ml7::Matrix3x3& m) { return u8"{ " + cl7::to_string(m.a) + u8", " + cl7::to_string(m.b) + u8", " + cl7::to_string(m.c) + u8"; " + cl7::to_string(m.d) + u8", " + cl7::to_string(m.e) + u8", " + cl7::to_string(m.f) + u8"; " + cl7::to_string(m.g) + u8", " + cl7::to_string(m.h) + u8", " + cl7::to_string(m.i) + u8" }"; }
    inline
    cl7::u8string to_string(const ml7::Matrix3x4& m) { return u8"{ " + cl7::to_string(m._11) + u8", " + cl7::to_string(m._12) + u8", " + cl7::to_string(m._13) + u8", " + cl7::to_string(m._14) + u8"; " + cl7::to_string(m._21) + u8", " + cl7::to_string(m._22) + u8", " + cl7::to_string(m._23) + u8", " + cl7::to_string(m._24) + u8"; " + cl7::to_string(m._31) + u8", " + cl7::to_string(m._32) + u8", " + cl7::to_string(m._33) + u8", " + cl7::to_string(m._34) + u8" }"; }
    inline
    cl7::u8string to_string(const ml7::Matrix4x4& m) { return u8"{ " + cl7::to_string(m._11) + u8", " + cl7::to_string(m._12) + u8", " + cl7::to_string(m._13) + u8", " + cl7::to_string(m._14) + u8"; " + cl7::to_string(m._21) + u8", " + cl7::to_string(m._22) + u8", " + cl7::to_string(m._23) + u8", " + cl7::to_string(m._24) + u8"; " + cl7::to_string(m._31) + u8", " + cl7::to_string(m._32) + u8", " + cl7::to_string(m._33) + u8", " + cl7::to_string(m._34) + u8"; " + cl7::to_string(m._41) + u8", " + cl7::to_string(m._42) + u8", " + cl7::to_string(m._43) + u8", " + cl7::to_string(m._44) + u8" }"; }
}



namespace ml7 {
    inline
    ml7::Vector2 round(ml7::Vector2 v, unsigned num_decimals) { for (float& k : v.data) k = ml7::round(k, num_decimals); return v; }
    inline
    ml7::Vector3 round(ml7::Vector3 v, unsigned num_decimals) { for (float& k : v.data) k = ml7::round(k, num_decimals); return v; }
    inline
    ml7::Vector4 round(ml7::Vector4 v, unsigned num_decimals) { for (float& k : v.data) k = ml7::round(k, num_decimals); return v; }

    inline
    ml7::Matrix2x2 round(ml7::Matrix2x2 m, unsigned num_decimals) { for ( unsigned k = 0; k < 4; ++k ) m.data[k] = ml7::round(m.data[k], num_decimals); return m; }
    inline
    ml7::Matrix2x3 round(ml7::Matrix2x3 m, unsigned num_decimals) { for ( unsigned k = 0; k < 6; ++k ) m.data[k] = ml7::round(m.data[k], num_decimals); return m; }
    inline
    ml7::Matrix3x3 round(ml7::Matrix3x3 m, unsigned num_decimals) { for ( unsigned k = 0; k < 9; ++k ) m.data[k] = ml7::round(m.data[k], num_decimals); return m; }
    inline
    ml7::Matrix3x4 round(ml7::Matrix3x4 m, unsigned num_decimals) { for ( unsigned k = 0; k < 12; ++k ) m.data[k] = ml7::round(m.data[k], num_decimals); return m; }
    inline
    ml7::Matrix4x4 round(ml7::Matrix4x4 m, unsigned num_decimals) { for ( unsigned k = 0; k < 16; ++k ) m.data[k] = ml7::round(m.data[k], num_decimals); return m; }
}



#endif // F77_TESTS_ML7_SHARED_H
